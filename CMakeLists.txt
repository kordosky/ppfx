# ======================================================================
#  ppfx main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/ppfx/ups/setup_for_development
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

find_package(cetmodules REQUIRED)
project(ppfx VERSION 3.00.04 LANGUAGES CXX)

# cetbuildtools contains our cmake modules

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS WERROR
                       NO_UNDEFINED
                       EXTRA_FLAGS -pedantic -Wno-unused-local-typedefs -Wno-variadic-macros
                                   -Wno-vla
                                   $<$<CXX_COMPILER_ID:GNU>:-Wno-maybe-uninitialized>
)
cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

find_package(nufinder REQUIRED EXPORT)

find_package( dk2nudata  REQUIRED )
find_package(Boost COMPONENTS headers filesystem REQUIRED EXPORT)
find_package( ROOT REQUIRED COMPONENTS Tree EG EXPORT)

message(STATUS "ROOTSYS is ${ROOTSYS}")

# macros
##include(ArtMake)
##include(BuildPlugins)
##include(CetRootCint)

# hackery
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )
# source
add_subdirectory (include)
add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (scripts)
add_subdirectory (uncertainties)

# ups - table and config files

# packaging utility

cet_cmake_config()
