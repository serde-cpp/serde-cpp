#########################################################################################
# serde
#########################################################################################
add_library(serde INTERFACE)
target_include_directories(serde INTERFACE include)
install(TARGETS serde)

