add_library(dpkvs_core STATIC
        # ==== Core Interfaces ===
        store_record/store_record.h
        store_record/store_record.cpp
        engines/runtime_store.h
        engines/operations.h

        # ==== Hash Map store engine ====
        # store engine
        engines/hash_map/engine/store_engine.h
        engines/hash_map/engine/store_engine.cpp

        # runtime
        engines/hash_map/runtime/hash_map_store.h
        engines/hash_map/runtime/hash_map_store.cpp

        # persistence (append only log)
        engines/hash_map/persistence/append_only_log.h
        engines/hash_map/persistence/append_only_log.cpp
        engines/hash_map/persistence/log_serializer.h
        engines/hash_map/persistence/log_serializer.cpp
)

target_include_directories(dpkvs_core PUBLIC
        ${CMAKE_SOURCE_DIR}
)
