cmake_minimum_required(VERSION 3.5)
project(linkbot-interfaces VERSION 0.1.0 LANGUAGES C CXX)

##############################################################################
# Set up nanopb

set(oldCfrpmi ${CMAKE_FIND_ROOT_PATH_MODE_INCLUDE})
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NO_CMAKE_FIND_ROOT_PATH)
# The CMAKE_FIND_ROOT_PATH_MODE_INCLUDE grotesquery is required to make the find_path(... pb.h)
# call succeed in FindNanopb.cmake under cross-compilation. Resetting it via the oldCfrpmi
# temporary variable might not strictly be required.
find_package(Nanopb REQUIRED)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ${oldCfrpmi})

# nanopb_add_proto(<target-name> <proto-filepath> [<other-source-files>...])
#
# Define a target to represent a nanopb protobuf library. You can use
# <target-name> in the rest of your code as a static library target (but
# containing position-independent code, and include its headers by using the
#
#   $<TARGET_PROPERTY:<target-name>,INCLUDE_DIRECTORIES>
#
# generator expression in a suitable command's arguments.
function(nanopb_add_proto _targetName _protoFile)
    list(APPEND NANOPB_IMPORT_DIRS ${RPC_PROTO_INCLUDE_DIR})
    nanopb_generate_cpp(sources headers ${_protoFile})

    if(MSVC)
        # Disable warning C4127: conditional expression is constant
        set_source_files_properties(${sources}
            PROPERTIES COMPILE_FLAGS /wd4127
        )
    endif()

    add_library(${_targetName} STATIC ${sources} ${headers} ${ARGN})
    set(headerDirs)
    foreach(header ${headers})
        get_filename_component(headerDir ${header} DIRECTORY)
        list(APPEND headerDirs ${headerDir})
    endforeach()
    list(REMOVE_DUPLICATES headerDirs)
    target_include_directories(${_targetName}
        PUBLIC
            ${NANOPB_INCLUDE_DIRS}
        PRIVATE
            ${headerDirs}
            .
    )
    if(NOT CMAKE_SYSTEM_NAME MATCHES "AVR")
        set_target_properties(${_targetName} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()
endfunction()

################################################################################
# Build proto files

nanopb_add_proto(bumpconnect-proto bumpconnect.proto)
nanopb_add_proto(commontypes-proto commontypes.proto)
nanopb_add_proto(daemon-proto daemon.proto)
nanopb_add_proto(dongle-proto dongle.proto)
nanopb_add_proto(radio-proto radio.proto)
nanopb_add_proto(robot-proto robot.proto)

set(picProperty)
if(NOT CMAKE_SYSTEM_NAME MATCHES "AVR")
    set(picProperty POSITION_INDEPENDENT_CODE ON)
endif()

set_target_properties(
    bumpconnect-proto
    commontypes-proto
    daemon-proto
    dongle-proto
    radio-proto
    robot-proto
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF  # c++11, not gnu++11
        ${picProperty}
)

if(MSVC)
	# Without this, the PB_STATIC_ASSERT line generated in robot.pb.c will
	# cause MSVC to choke on a 4K+ long token (error C1064).
	target_compile_definitions(robot-proto PRIVATE PB_NO_STATIC_ASSERT)
	message(WARNING "Disabling PB_STATIC_ASSERT for robot-proto due to Visual C++ token length limit")
endif()

target_link_libraries(bumpconnect-proto PUBLIC commontypes-proto)
target_link_libraries(daemon-proto PUBLIC commontypes-proto robot-proto)
target_link_libraries(dongle-proto PUBLIC radio-proto)
target_link_libraries(radio-proto PUBLIC commontypes-proto bumpconnect-proto robot-proto)

install(
    TARGETS
        bumpconnect-proto
        commontypes-proto
        daemon-proto
        dongle-proto
        radio-proto
        robot-proto
    EXPORT barobo
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES
    ${PROJECT_BINARY_DIR}/bumpconnect.pb.h
    ${PROJECT_BINARY_DIR}/bumpconnect.pb.hpp
    ${PROJECT_BINARY_DIR}/commontypes.pb.h
    ${PROJECT_BINARY_DIR}/commontypes.pb.hpp
    ${PROJECT_BINARY_DIR}/daemon.pb.h
    ${PROJECT_BINARY_DIR}/daemon.pb.hpp
    ${PROJECT_BINARY_DIR}/dongle.pb.h
    ${PROJECT_BINARY_DIR}/dongle.pb.hpp
    ${PROJECT_BINARY_DIR}/radio.pb.h
    ${PROJECT_BINARY_DIR}/radio.pb.hpp
    ${PROJECT_BINARY_DIR}/robot.pb.h
    ${PROJECT_BINARY_DIR}/robot.pb.hpp
    DESTINATION include
)

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
