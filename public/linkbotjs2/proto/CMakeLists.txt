cmake_minimum_required(VERSION 3.5)
project(linkbot-interfaces VERSION 0.1.0 LANGUAGES C CXX)

set(AVR OFF)
if(CMAKE_SYSTEM_NAME MATCHES "AVR")
    set(AVR ON)
    message(STATUS "Configuring for AVR")
endif()

if(NOT AVR)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost 1.54.0 REQUIRED system)
endif()
find_package(ribbon-bridge 0.3.0 REQUIRED)

set(commontypesSources commontypes.proto)
if(NOT AVR)
    list(APPEND commontypesSources src/system_error.cpp)
endif()

nanopb_add_proto(commontypes-proto ${commontypesSources})

nanopb_add_proto(robot-interface robot.proto gen-robot.pb.cpp)
nanopb_add_proto(dongle-interface dongle.proto gen-dongle.pb.cpp)
nanopb_add_proto(daemon-interface daemon.proto gen-daemon.pb.cpp)

set_target_properties(commontypes-proto robot-interface dongle-interface daemon-interface
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF  # c++11, not gnu++11
        POSITION_INDEPENDENT_CODE ON
)

if(MSVC)
	# Without this, the PB_STATIC_ASSERT line generated in robot.pb.c will
	# cause MSVC to choke on a 4K+ long token (error C1064).
	target_compile_definitions(robot-interface PRIVATE PB_NO_STATIC_ASSERT)
	message(WARNING "Disabling PB_STATIC_ASSERT for robot-interface due to Visual C++ token length limit")
endif()

target_link_libraries(commontypes-proto PUBLIC barobo::rpc-proto)
if(NOT AVR)
    target_link_libraries(commontypes-proto PUBLIC Boost::system)
endif()
target_include_directories(commontypes-proto PRIVATE include)

target_link_libraries(robot-interface commontypes-proto barobo::rpc)
target_link_libraries(dongle-interface commontypes-proto barobo::rpc)
target_link_libraries(daemon-interface commontypes-proto barobo::rpc)


install(TARGETS commontypes-proto robot-interface dongle-interface daemon-interface EXPORT barobo
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES
    gen-robot.pb.hpp gen-dongle.pb.hpp gen-daemon.pb.hpp
    ${PROJECT_BINARY_DIR}/commontypes.pb.h
    ${PROJECT_BINARY_DIR}/robot.pb.h
    ${PROJECT_BINARY_DIR}/dongle.pb.h
    ${PROJECT_BINARY_DIR}/daemon.pb.h
    DESTINATION include)

install(DIRECTORY include/ DESTINATION include)

# Boilerplate follows

export(EXPORT barobo
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake"
    NAMESPACE barobo::
)

install(EXPORT barobo
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE barobo::
    DESTINATION cmake
)

file(WRITE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "include(\"\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}-targets.cmake\")"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-configVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-configVersion.cmake"
    DESTINATION cmake
)
