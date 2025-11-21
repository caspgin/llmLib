cmake_minimum_required(VERSION 3.15...3.29)

project(llmLib CXX)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT_DIR}")
set(OPENSSL_CRYPTO_LIBRARY "$ENV{OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "$ENV{OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib")

#Check if the variable is set
if(NOT OPENSSL_ROOT_DIR)
    message(FATAL_ERROR "OPENSSL_ROOT_DIR environment variable is not set.")
endif()


add_subdirectory(extern/json)
add_subdirectory(extern/cpp-httplib EXCLUDE_FROM_ALL)

add_library(llm_client STATIC src/llm_api_client.cpp src/llm_api_client.h)

target_compile_definitions(llm_client PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

target_link_libraries(llm_client PRIVATE nlohmann_json::nlohmann_json
${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY}
)

target_include_directories(llm_client PUBLIC
	$<BUILD_INTERFACE:${OPENSSL_ROOT_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/cpp-httplib> 
    $<INSTALL_INTERFACE:include> # For installation if you go that route
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(LLMLIB_BUILD_TESTS "Build the testing tree." ON)
else()
    option(LLMLIB_BUILD_TESTS "Build the testing tree." OFF)
endif()

if(LLMLIB_BUILD_TESTS)
    add_executable(llm_test_app src/test_api_client.cpp)
    target_link_libraries(llm_test_app PRIVATE llm_client)
endif()

