include(FetchContent)

set(USE_VCPKG_AS_SUBMODULE ON CACHE BOOL "Whether to use vcpkg as a submodule")
set(vcpkg_ROOT_DIR ${CMAKE_SOURCE_DIR}/third_party/vcpkg)
set(vcpkg_TAG 2022.10.19)
get_filename_component(
    VCPKG_INSTALL_BINARY_SOURCE_DIR "${PROJECT_TOPLEVEL_DIR}/vcpkg_binary_source" REALPATH
)
file(MAKE_DIRECTORY ${VCPKG_INSTALL_BINARY_SOURCE_DIR})
message("Maked a binary source directory: ${VCPKG_INSTALL_BINARY_SOURCE_DIR}")

if(USE_VCPKG_AS_SUBMODULE)
    message(STATUS "Fetching vcpkg as submodule")
    if(NOT EXISTS ${vcpkg_ROOT_DIR})
        execute_process(
            COMMAND git clone https://github.com/microsoft/vcpkg --branch=${vcpkg_TAG}
                    ${vcpkg_ROOT_DIR}
        )
    else()
        execute_process(COMMAND git fetch WORKING_DIRECTORY ${vcpkg_ROOT_DIR})
        execute_process(COMMAND git checkout ${vcpkg_TAG} WORKING_DIRECTORY ${vcpkg_ROOT_DIR})
    endif()
    message(STATUS "Fetched vcpkg as submodule")
    set(CMAKE_TOOLCHAIN_FILE ${vcpkg_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake
        CACHE STRING "vcpkg toolchain file"
    )
    set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_LIST_DIR}/custom-triplets)
    set(VCPKG_TARGET_TRIPLET "x64-windows-static-md" CACHE STRING "vcpkg triplet")
    set(VCPKG_INSTALLED_DIR ${CMAKE_SOURCE_DIR}/third_party/vcpkg_installed
        CACHE STRING "vcpkg install root"
    )
    set(VCPKG_MAX_CONCURRENCY 20)
    set(VCPKG_BINARY "${vcpkg_ROOT_DIR}/vcpkg")

    set(VCPKG_INSTALL_OPTIONS "--binarysource=files,${VCPKG_INSTALL_BINARY_SOURCE_DIR},write")
    set(VCPKG_BOOTSTRAP_SCRIPT "${vcpkg_ROOT_DIR}/bootstrap-vcpkg.sh")
    if(NOT EXISTS ${VCPKG_BINARY})
        message(STATUS "Bootstraping vcpkg")
        execute_process(COMMAND "${VCPKG_BOOTSTRAP_SCRIPT}")
    endif()

endif()
