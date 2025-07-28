add_library(dpkvs_lib
        engine/store_engine.cpp
        engine/store_engine.h
        engine/storable_value.h
        engine/storable_value.cpp
)

target_include_directories(dpkvs_lib PRIVATE
        ${CMAKE_SOURCE_DIR}
)
