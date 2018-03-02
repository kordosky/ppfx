include_directories( ${CMAKE_SOURCE_DIR}/include )

MESSAGE("set include directories for the source to a relative path")

FILE( GLOB cxx_files *.cpp )

art_make_library(LIBRARY_NAME ppfx
                 SOURCE       ${cxx_files}
                 LIBRARIES    dk2nuTree
                              ${ROOT_BASIC_LIB_LIST}
         
)


install_source()
