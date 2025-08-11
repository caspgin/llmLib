cmake_minimum_required(VERSION 3.15...3.29)

project(llmLib CXX)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
find_package(OpenSSL REQUIRED)

add_subdirectory(extern/json)
add_subdirectory(extern/cpp-httplib EXCLUDE_FROM_ALL)

add_library(llm_client STATIC src/llm_api_client.cpp src/llm_api_client.h)

target_compile_definitions(llm_client PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

target_link_libraries(llm_client PRIVATE nlohmann_json::nlohmann_json
${OpenSSL_LIBRARIES}
)

target_include_directories(llm_client PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src> # Public for users of llm_client
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/cpp-httplib> 
    $<INSTALL_INTERFACE:include> # For installation if you go that route
	${OpenSSL_INCLUDE_DIRS}
)

add_executable(llm_test_app src/test_api_client.cpp)

target_link_libraries(llm_test_app PRIVATE
    llm_client
)
