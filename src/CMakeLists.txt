

include_directories( ${CMAKE_SOURCE_DIR}/include )

FILE( GLOB cxx_files *.cpp )

cet_find_library( DK2NUTREE  NAMES dk2nuTree  PATHS ENV DK2NUDATA_LIB )

art_make_library(LIBRARY_NAME ppfx
                 SOURCE       ${cxx_files}
                 LIBRARIES    dk2nuTree
                              ${ROOT_BASIC_LIB_LIST}
                              ${ROOT_EG}

         
)


install_source()
