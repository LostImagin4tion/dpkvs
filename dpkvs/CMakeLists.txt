add_library(dpkvs_master_lib INTERFACE)

add_subdirectory(core)
add_subdirectory(network)

target_link_libraries(dpkvs_master_lib INTERFACE
        dpkvs_core
        dpkvs_network
)
