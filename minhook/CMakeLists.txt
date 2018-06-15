FILE(GLOB_RECURSE SOURCE_FILES include/*.h src/*.h src/*.c)

add_library(minhook ${SOURCE_FILES})
target_include_directories(minhook INTERFACE include/MinHook.h)
