cmake_minimum_required(VERSION 3.15)
project(NexoidCPP VERSION 1.0 LANGUAGES C CXX)

# --- General Compiler Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Setup threading cross-platform
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# --- Warnings and Compiler Flags ---
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(
        -Wall -Wextra -Wno-unused-parameter
        -Wformat -Wformat-security
        -Wconversion -Wsign-conversion
        -fstack-protector-all
        -fpermissive
    )
    # Equivalent to LDFLAGS += -latomic
    link_libraries(atomic)
endif()

# --- ASN.1 Generation ---
set(ASN_SOURCES 
    asn1/Scapi.asn1 
    asn1/ScapiSocketClient.asn1 
    asn1/ScapiNngClient.asn1
)
set(ASN1_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/asn1c-generated")

# Generate the directory safely
file(MAKE_DIRECTORY ${ASN1_GENERATED_DIR})

# Custom command to run asn1c compiler
add_custom_command(
    OUTPUT ${ASN1_GENERATED_DIR}/asn1_stamp
    COMMAND asn1c -fincludes-quoted -pdu=auto -D ${ASN1_GENERATED_DIR} -no-gen-example -no-gen-OER -no-gen-PER ${CMAKE_CURRENT_SOURCE_DIR}/${ASN_SOURCES}
    COMMAND ${CMAKE_COMMAND} -E touch ${ASN1_GENERATED_DIR}/asn1_stamp
    DEPENDS ${ASN_SOURCES}
    COMMENT "Generating ASN.1 C sources..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- Subdirectories and Dependencies ---
# Assuming nexoid-ed has been converted to CMake as well. 
# If not, this would need to be built via ExternalProject_Add or custom target.
add_subdirectory(nexoid-ed)

# --- Main Executable ---
# Collect all source files (Note: CMake best practice is to list explicitly, 
# but globbing matches the Makefile's "wildcard *.cpp" behavior).
file(GLOB SOURCES "*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/randapi.cpp") # Exclude UT file

add_executable(nexoid-cpp 
    ${SOURCES} 
    nexoconf.c 
    ${ASN1_GENERATED_DIR}/asn1_stamp
)

# Include directories
target_include_directories(nexoid-cpp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/nexoid-ed/src
    ${CMAKE_CURRENT_SOURCE_DIR}/nexoid-ed/include
    ${ASN1_GENERATED_DIR}
)

# Cross-platform library linking
if(WIN32)
    # Windows fallback for asn1c headers if not in PATH/system
    # target_include_directories(nexoid-cpp PRIVATE "C:/path/to/asn1c/share")
    
    # Windows networking
    target_link_libraries(nexoid-cpp PRIVATE 
        nexoid_ed # Assuming target name from add_subdirectory
        nng
        ws2_32    # Windows socket library replacing socket++
        Threads::Threads
    )
else()
    target_include_directories(nexoid-cpp PRIVATE /usr/share/asn1c)
    target_link_libraries(nexoid-cpp PRIVATE 
        nexoid_ed 
        socket++ 
        nng 
        Threads::Threads
    )
endif()

# Additional definitions
target_compile_definitions(nexoid-cpp PRIVATE ASN_EMIT_DEBUG=1)

# --- Unit Tests ---
enable_testing()

file(GLOB UT_SOURCES "ut/*.cpp")
add_executable(ut_nexofat ${UT_SOURCES} randapi.cpp)

target_include_directories(ut_nexofat PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/nexoid-ed/include
)

if(WIN32)
    target_link_libraries(ut_nexofat PRIVATE nng ws2_32)
else()
    target_link_libraries(ut_nexofat PRIVATE nng socket++)
endif()

add_test(NAME UnitTests COMMAND ut_nexofat)

# --- Installation ---
install(TARGETS nexoid-cpp DESTINATION bin)