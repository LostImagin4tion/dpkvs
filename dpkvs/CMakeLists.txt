add_library(dpkvs_lib STATIC
        engine/store_engine.cpp
        engine/store_engine.h
        engine/storable_value.h
        engine/storable_value.cpp
        append_only_log/operations.h
        append_only_log/append_only_log.h
        append_only_log/append_only_log.cpp
        append_only_log/log_serializer.h
        append_only_log/log_serializer.cpp
        store_controller/store_controller.h
        store_controller/store_controller.cpp
)

target_include_directories(dpkvs_lib PRIVATE
        ${CMAKE_SOURCE_DIR}
)
