add_library(dpkvs_core STATIC
        # ==== Core Interfaces ===
        store_record/store_record.h
        store_record/store_record.cpp
        engines/store_engine.h
        engines/operations.h

        # ==== Hash Map store engine ====
        engines/hash_map/impl/hash_map_store_engine.h
        engines/hash_map/impl/hash_map_store_engine.cpp

        # append only log
        engines/hash_map/append_only_log/append_only_log.h
        engines/hash_map/append_only_log/append_only_log.cpp
        engines/hash_map/append_only_log/log_serializer.h
        engines/hash_map/append_only_log/log_serializer.cpp

        # store controller
        engines/hash_map/controller/store_controller.h
        engines/hash_map/controller/store_controller.cpp
)

target_include_directories(dpkvs_core PUBLIC
        ${CMAKE_SOURCE_DIR}
)
