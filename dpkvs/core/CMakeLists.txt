add_library(dpkvs_core STATIC
        # ==== Core Interfaces ===
        engines/store_engine.h
        engines/runtime_store.h

        # store value protos
        store_value/generated/store_value.pb.h
        store_value/generated/store_value.pb.cc

        # serializer
        store_record/generated/store_record.pb.h
        store_record/generated/store_record.pb.cc

        store_record/serializer/record_serializer.h
        store_record/serializer/record_serializer.cpp

        store_record/factory/factory.h
        store_record/factory/factory.cpp

        # ==== Hash Map store engine ====
        # store engine
        engines/hash_map/engine/hash_map_store_engine.h
        engines/hash_map/engine/hash_map_store_engine.cpp

        # runtime
        engines/hash_map/runtime/hash_map_store.h
        engines/hash_map/runtime/hash_map_store.cpp

        # persistence (append only log)
        engines/hash_map/persistence/append_only_log.h
        engines/hash_map/persistence/append_only_log.cpp

        # ==== Loggers ====
        logger/console/console_logger.h
        logger/console/console_logger.cpp
)

target_include_directories(dpkvs_core
        PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/store_value/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/store_record/generated
)

target_link_libraries(dpkvs_core
        PUBLIC
        protobuf::libprotobuf
        spdlog::spdlog_header_only
        PRIVATE
        absl::crc32c
)

