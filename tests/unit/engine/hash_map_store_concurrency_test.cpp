#include <gtest/gtest.h>

#include <dpkvs/core/engines/hash_map/runtime/hash_map_store.h>
#include <dpkvs/core/store_value/generated/store_value.pb.h>

#include <atomic>
#include <barrier>
#include <random>
#include <string>
#include <thread>
#include <vector>

using NKVStore::NCore::NEngine::NRuntime::THashMapStore;
using NKVStore::NCore::NRecord::TStoreValue;

class HashMapStoreConcurrencyTest
    : public ::testing::Test {
protected:
    TStoreValue MakeValue(const std::string& payload) {
        TStoreValue v;
        v.set_data(payload);
        return v;
    }

    THashMapStore store;
};

TEST_F(HashMapStoreConcurrencyTest, PutAndReadDistinctKeysConcurrently) {
    const int numThreads = 8;
    const int keysPerThread = 500;

    std::barrier startBarrier(numThreads);
    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i] {
            startBarrier.arrive_and_wait();

            for (int j = 0; j < keysPerThread; ++j) {
                std::string key = "k_" + std::to_string(i) + "_" + std::to_string(j);
                auto val = MakeValue("v_" + std::to_string(i) + "_" + std::to_string(j));

                store.Put(key, std::move(val));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    ASSERT_EQ(store.Size(), static_cast<size_t>(numThreads * keysPerThread));

    for (int i = 0; i < numThreads; ++i) {
        for (int j : {0, keysPerThread - 1}) {
            std::string key = "k_" + std::to_string(i) + "_" + std::to_string(j);
            
            auto value = store.Get(key);
            ASSERT_TRUE(value) << key;
            
            auto expectedValue = "v_" + std::to_string(i) + "_" + std::to_string(j);
            ASSERT_EQ(value->data(), expectedValue);
        }
    }
}

TEST_F(HashMapStoreConcurrencyTest, ReadersSeeValidValuesDuringConcurrentPuts) {
    const int numKeys = 128;
    const int numWriterThreads = 4;
    const int numReaderThreads = 4;
    const int updatesPerWriter = 2000;

    std::vector<std::string> keys;
    keys.reserve(numKeys);
    
    for (int i = 0; i < numKeys; ++i) {
        keys.emplace_back("key_" + std::to_string(i));
        store.Put(keys.back(), MakeValue("init_" + std::to_string(i)));
    }

    std::atomic<bool> stopReadersCmd{false};
    std::atomic<uint64_t> nullReadsCounter{0};

    std::barrier startBarrier(numWriterThreads + numReaderThreads);

    std::vector<std::thread> readerThreads;
    readerThreads.reserve(numReaderThreads);
    
    for (int i = 0; i < numReaderThreads; ++i) {
        readerThreads.emplace_back([&, i] {
            std::mt19937_64 randomGenerator(static_cast<uint64_t>(i) + 0xC0FFEE);
            std::uniform_int_distribution<int> dist(0, numKeys - 1);
            
            startBarrier.arrive_and_wait();
            
            while (!stopReadersCmd.load(std::memory_order_relaxed)) {
                const auto& key = keys[dist(randomGenerator)];
                auto value = store.Get(key);
                
                if (!value) {
                    nullReadsCounter.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });
    }

    std::vector<std::thread> writerThreads;
    writerThreads.reserve(numWriterThreads);
    
    for (int i = 0; i < numWriterThreads; ++i) {
        writerThreads.emplace_back([&, i] {
            startBarrier.arrive_and_wait();
            
            for (int j = 0; j < updatesPerWriter; ++j) {
                int idx = (i + j) % numKeys;
                auto val = MakeValue("upd_" + std::to_string(i) + "_" + std::to_string(j));
                
                store.Put(keys[idx], std::move(val));
            }
        });
    }

    for (auto& writer : writerThreads) {
        writer.join();
    }
    stopReadersCmd.store(true, std::memory_order_relaxed);
    for (auto& reader : readerThreads) {
        reader.join();
    }

    ASSERT_EQ(nullReadsCounter.load(), 0u);
    
    for (int i = 0; i < numKeys; ++i) {
        auto value = store.Get(keys[i]);

        ASSERT_TRUE(value);
        ASSERT_FALSE(value->data().empty());
    }
}

TEST_F(HashMapStoreConcurrencyTest, ReadersSeeConcurrentRemoves) {
    const int numKeys = 400;
    const int numRemovers = 4;
    const int numReaders = 4;

    std::vector<std::string> keys;
    keys.reserve(numKeys);
    
    for (int i = 0; i < numKeys; ++i) {
        keys.emplace_back("rmk_" + std::to_string(i));
        store.Put(keys.back(), MakeValue("v_" + std::to_string(i)));
    }

    std::atomic<bool> stopReadersCommand{false};
    std::barrier startBarrier(numRemovers + numReaders);

    std::vector<std::thread> readerThreads;
    readerThreads.reserve(numReaders);

    for (int i = 0; i < numReaders; ++i) {
        readerThreads.emplace_back([&, i] {
            std::mt19937_64 generator(static_cast<uint64_t>(i) + 0xBEEF);
            std::uniform_int_distribution<int> dist(0, numKeys - 1);

            startBarrier.arrive_and_wait();

            while (!stopReadersCommand.load(std::memory_order_relaxed)) {
                const auto& k = keys[dist(generator)];
                auto value = store.Get(k);

                if (value) {
                    ASSERT_FALSE(value->data().empty());
                }
            }
        });
    }

    std::vector<std::thread> removerThreads;
    removerThreads.reserve(numRemovers);

    for (int i = 0; i < numRemovers; ++i) {
        removerThreads.emplace_back([&, i] {
            startBarrier.arrive_and_wait();

            int begin = (numKeys * i) / numRemovers;
            int end = (numKeys * (i + 1)) / numRemovers;

            for (int j = begin; j < end; ++j) {
                auto removed = store.Remove(keys[j]);
                ASSERT_TRUE(removed);
            }
        });
    }

    for (auto& remover : removerThreads) {
        remover.join();
    }
    stopReadersCommand.store(true, std::memory_order_relaxed);
    for (auto& reader : readerThreads) {
        reader.join();
    }

    ASSERT_EQ(store.Size(), 0u);
    for (int i = 0; i < numKeys; ++i) {
        ASSERT_FALSE(store.Get(keys[i]));
    }
}
