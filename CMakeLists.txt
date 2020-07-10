cmake_minimum_required(VERSION 3.10)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(ENV{TRAVIS_BUILD_NUMBER} 1) #plug

configure_file(version.vers version.h)

project(experimental_fs VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.$ENV{TRAVIS_BUILD_NUMBER})

file(GLOB_RECURSE DATA_1 proc/p1/*)
file(GLOB_RECURSE DATA_2 proc/p2/*)
file(GLOB_RECURSE DATA_3 proc/p3/*)


find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

#add library
add_library(experim SHARED 
                ${PROJECT_SOURCE_DIR}/path_worker/path_worker.h
                ${PROJECT_SOURCE_DIR}/path_worker/path_worker.cpp
                ${PROJECT_SOURCE_DIR}/packer/Packer.h)

#add executable
add_executable(experimental_fs.out 
                ${PROJECT_SOURCE_DIR}/src/main.cpp ${DATA_1} ${DATA_2} ${DATA_3})

file(COPY ${DATA_1} DESTINATION proc/p1)
file(COPY ${DATA_2} DESTINATION proc/p2)
file(COPY ${DATA_3} DESTINATION proc/p3)


target_include_directories(experim PUBLIC ${PROJECT_BINARY_DIR} 
                                          ${PROJECT_SOURCE_DIR} 
                          )

set_target_properties(experimental_fs.out experim PROPERTIES
                        CXX_STANDARD 17
                        LINKER_LANGUAGE CXX
                        CXX_STANDARD_REQUIRED ON
                        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
                    )

target_link_libraries(experim ${Boost_LIBRARIES})

target_link_libraries(experimental_fs.out experim)