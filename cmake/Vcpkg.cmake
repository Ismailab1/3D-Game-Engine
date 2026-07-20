# cmake/Vcpkg.cmake
#
# Auto-discovers or bootstraps vcpkg and wires up CMAKE_TOOLCHAIN_FILE.
# Include this BEFORE project() in the root CMakeLists.txt so that the
# toolchain file is loaded during CMake's language-detection step:
#
#   cmake_minimum_required(VERSION 3.20)
#   include(cmake/Vcpkg.cmake)
#   project(MyProject LANGUAGES CXX)
#
# Discovery order
#   1. CMAKE_TOOLCHAIN_FILE already defined  → skip entirely (user override)
#   2. VCPKG_ROOT environment variable       → use that root
#   3. Well-known installation paths         → first valid match wins
#   4. Project-local  external/vcpkg/        → used by CI on fresh runners
#   5. Auto-clone + bootstrap                → last resort, fully automated

cmake_minimum_required(VERSION 3.20)

# ─── 1. User already provided a toolchain ────────────────────────────────────
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "[vcpkg] CMAKE_TOOLCHAIN_FILE already set — skipping discovery.")
    return()
endif()

# ─── Helper: validate a candidate vcpkg root ─────────────────────────────────
function(_vcpkg_validate_root candidate out_var)
    if(EXISTS "${candidate}/scripts/buildsystems/vcpkg.cmake")
        set(${out_var} "${candidate}" PARENT_SCOPE)
    endif()
endfunction()

set(_VCPKG_ROOT "")

# ─── 2. Well-known installation paths (checked first so a proper standalone  ───
#        vcpkg is preferred over the VS-bundled one that vcvars64.bat injects) ──
if(NOT _VCPKG_ROOT)
    if(WIN32)
        set(_vcpkg_candidates
            "C:/vcpkg"
            "D:/vcpkg"
            "C:/dev/vcpkg"
            "$ENV{USERPROFILE}/vcpkg"
            "$ENV{LOCALAPPDATA}/vcpkg"
        )
    else()
        set(_vcpkg_candidates
            "$ENV{HOME}/vcpkg"
            "/usr/local/vcpkg"
            "/opt/vcpkg"
        )
    endif()

    foreach(_cand IN LISTS _vcpkg_candidates)
        _vcpkg_validate_root("${_cand}" _VCPKG_ROOT)
        if(_VCPKG_ROOT)
            message(STATUS "[vcpkg] Found at well-known path: ${_VCPKG_ROOT}")
            break()
        endif()
    endforeach()
endif()

# ─── 3. VCPKG_ROOT environment variable (fallback — may be VS-bundled) ────────
if(NOT _VCPKG_ROOT)
    if(DEFINED ENV{VCPKG_ROOT})
        _vcpkg_validate_root("$ENV{VCPKG_ROOT}" _VCPKG_ROOT)
        if(_VCPKG_ROOT)
            message(STATUS "[vcpkg] Found via VCPKG_ROOT env: ${_VCPKG_ROOT}")
        endif()
    endif()
endif()

# ─── 4. Project-local external/vcpkg ─────────────────────────────────────────
if(NOT _VCPKG_ROOT)
    _vcpkg_validate_root("${CMAKE_SOURCE_DIR}/external/vcpkg" _VCPKG_ROOT)
    if(_VCPKG_ROOT)
        message(STATUS "[vcpkg] Found project-local copy: ${_VCPKG_ROOT}")
    endif()
endif()

# ─── 5. Auto-clone + bootstrap ───────────────────────────────────────────────
if(NOT _VCPKG_ROOT)
    set(_clone_dest "${CMAKE_SOURCE_DIR}/external/vcpkg")
    message(STATUS "[vcpkg] Not found anywhere — cloning into ${_clone_dest} ...")

    find_program(GIT_EXECUTABLE git REQUIRED)
    if(NOT GIT_EXECUTABLE)
        message(FATAL_ERROR
            "[vcpkg] git not found. Install git and re-run CMake,\n"
            "        or set VCPKG_ROOT to an existing vcpkg installation.")
    endif()

    if(NOT EXISTS "${_clone_dest}/.git")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" clone --depth 1
                    "https://github.com/microsoft/vcpkg.git"
                    "${_clone_dest}"
            RESULT_VARIABLE _clone_rc
        )
        if(NOT _clone_rc EQUAL 0)
            message(FATAL_ERROR "[vcpkg] git clone failed (exit code ${_clone_rc}).")
        endif()
    endif()

    message(STATUS "[vcpkg] Bootstrapping vcpkg ...")
    if(WIN32)
        execute_process(
            COMMAND cmd /c "${_clone_dest}/bootstrap-vcpkg.bat" -disableMetrics
            WORKING_DIRECTORY "${_clone_dest}"
            RESULT_VARIABLE _bootstrap_rc
        )
    else()
        execute_process(
            COMMAND bash "${_clone_dest}/bootstrap-vcpkg.sh" -disableMetrics
            WORKING_DIRECTORY "${_clone_dest}"
            RESULT_VARIABLE _bootstrap_rc
        )
    endif()

    if(NOT _bootstrap_rc EQUAL 0)
        message(FATAL_ERROR "[vcpkg] Bootstrap failed (exit code ${_bootstrap_rc}).")
    endif()

    message(STATUS "[vcpkg] Bootstrap complete.")
    set(_VCPKG_ROOT "${_clone_dest}")
endif()

# ─── Apply ────────────────────────────────────────────────────────────────────
set(VCPKG_ROOT "${_VCPKG_ROOT}"
    CACHE PATH "vcpkg root directory (set by cmake/Vcpkg.cmake)" FORCE)

set(CMAKE_TOOLCHAIN_FILE "${_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "vcpkg toolchain (set by cmake/Vcpkg.cmake)" FORCE)

message(STATUS "[vcpkg] Toolchain → ${CMAKE_TOOLCHAIN_FILE}")
