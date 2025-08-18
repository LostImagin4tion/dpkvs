add_library(dpkvs_core STATIC
        # ==== Core Interfaces ===
        store_record/store_record.h
        store_record/store_record.cpp
        engines/runtime_store.h
        engines/operations.h

        # ==== Hash Map store engine ====
        engines/hash_map/runtime/hash_map_store.h
        engines/hash_map/runtime/hash_map_store.cpp

        # append only log
        engines/hash_map/persistence/append_only_log.h
        engines/hash_map/persistence/append_only_log.cpp
        engines/hash_map/persistence/log_serializer.h
        engines/hash_map/persistence/log_serializer.cpp

        # store controller
        engines/hash_map/controller/store_controller.h
        engines/hash_map/controller/store_controller.cpp
)

target_include_directories(dpkvs_core PUBLIC
        ${CMAKE_SOURCE_DIR}
)
