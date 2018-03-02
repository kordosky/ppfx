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


# use cmake 2.8 or later
cmake_minimum_required (VERSION 3.3)

project(ppfx)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH $ENV{CANVAS_ROOT_IO_DIR}/Modules
                      $ENV{CETBUILDTOOLS_DIR}/Modules
                      ${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

# -D_GLIBCXX_USE_NANOSLEEP is needed to make std::this_thread::sleep_for available.
cet_set_compiler_flags(DIAGS CAUTIOUS WERROR
                       NO_UNDEFINED
                       EXTRA_FLAGS -pedantic
                       EXTRA_CXX_FLAGS -D_GLIBCXX_USE_NANOSLEEP
                                       -Wno-unused-local-typedefs -Wno-variadic-macros)
cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
# is is used for getting the art cmake modules
find_ups_product( art        )
find_ups_product( genie      )
find_ups_product( dk2nudata  )
find_ups_product( dk2nugenie )
find_ups_product( fftw       )
find_ups_product( libwda     )
find_ups_product( postgresql )
find_ups_product( boost )
find_ups_geant4( )
find_ups_root( )
find_ups_product( cetbuildtools )

message(STATUS "ROOTSYS is ${ROOTSYS}")

## find_library directives
# other dependencies
cet_find_library( PYTHIA6 NAMES Pythia6 PATHS ENV PYLIB NO_DEFAULT_PATH )
cet_find_library( LHAPDF NAMES LHAPDF PATHS ENV LHAPDF_LIB NO_DEFAULT_PATH )
cet_find_library( LOG4CPP NAMES log4cpp PATHS ENV LOG4CPP_LIB NO_DEFAULT_PATH )
cet_find_library( XML2 NAMES xml2 PATHS $ENV{LIBXML2_FQ_DIR}/lib NO_DEFAULT_PATH )
# genie
cet_find_library( GALGORITHM NAMES GAlgorithm PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GBARYONRESONANCE NAMES GBaryonResonance PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GBASE NAMES GBase PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GBODEKYANG NAMES GBodekYang PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GCHARM NAMES GCharm PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GCOH NAMES GCoh PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GDFRC NAMES GDfrc PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GDIS NAMES GDIS PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GCROSSSECTIONS NAMES GCrossSections PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GDECAY NAMES GDecay PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GELAS NAMES GElas PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GELFF NAMES GElFF PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GHEP NAMES GHEP PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GEVGCORE NAMES GEVGCore  PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GEVGMODULES NAMES GEVGModules PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GEVGDRIVERS NAMES GEVGDrivers PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GGIBUU NAMES GGiBUU PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GHADRONTRANSP NAMES GHadronTransp PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GFRAGMENTATION NAMES GFragmentation PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GINTERACTION NAMES GInteraction PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GLLEWELLYNSMITH NAMES GLlewellynSmith  PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GMEC NAMES GMEC PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GMESSENGER NAMES GMessenger PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GNUGAMMA NAMES GNuGamma PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GNUE NAMES GNuE PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GNTUPLE NAMES GNtuple PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GNUCLEAR NAMES GNuclear PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GNUMERICAL NAMES GNumerical PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GQPM NAMES GQPM PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GPDG NAMES GPDG PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GPDF NAMES GPDF PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GQEL NAMES GQEL PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GRES NAMES GRES PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GREGISTRY NAMES GRegistry PATHS ENV GENIE_LIB NO_DEFAULT_PATH )

# prior to GENIE R-2_9_0 Sehgal's name was spelled incorrectly
# so if minor version # is 8 or less use GReinSeghal, otherwise GReinSehgal
# parse_ups_version returns VMAJ, VMIN, and VPRJ
parse_ups_version( ${GENIE_VERSION} )
set ( GRSSPELLING GReinSehgal )
if ( ${VMAJ} EQUAL 2 AND ${VMIN} LESS 9 )
  set ( GRSSPELLING GReinSeghal )
endif ( ${VMAJ} EQUAL 2 AND ${VMIN} LESS 9 )
message(STATUS "GENIE version ${GENIE_VERSION} major ${VMAJ} minor ${VMIN} ${GRSSPELLING}")
cet_find_library( GREINSEHGAL NAMES ${GRSSPELLING} PATHS ENV GENIE_LIB NO_DEFAULT_PATH )

cet_find_library( GUTILS NAMES GUtils PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GGEO NAMES GGeo PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GFLUXDRIVERS NAMES GFluxDrivers PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GMUELOSS NAMES GMuELoss PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GREWEIGHT NAMES GReWeight PATHS ENV GENIE_LIB NO_DEFAULT_PATH )
cet_find_library( GNUCLEONDECAY NAMES GNucleonDecay PATHS ENV GENIE_LIB NO_DEFAULT_PATH )

set(GENIE_LIB_LIST  ${GALGORITHM}
                    ${GBARYONRESONANCE}
                    ${GBASE}
                    ${GBODEKYANG}
                    ${GCHARM}
                    ${GCOH}
                    ${GDFRC}
                    ${GDIS}
                    ${GCROSSSECTIONS}
                    ${GDECAY}
                    ${GELAS}
                    ${GELFF}
                    ${GHEP}
                    ${GEVGCORE}
                    ${GEVGMODULES}
                    ${GEVGDRIVERS}
                    ${GGIBUU}
                    ${GHADRONTRANSP}
                    ${GFRAGMENTATION}
                    ${GINTERACTION}
                    ${GLLEWELLYNSMITH}
                    ${GMEC}
                    ${GMESSENGER}
                    ${GNUGAMMA}
                    ${GNUE}
                    ${GNTUPLE}
                    ${GNUCLEAR}
                    ${GNUMERICAL}
                    ${GQPM}
                    ${GPDG}
                    ${GPDF}
                    ${GQEL}
                    ${GRES}
                    ${GREGISTRY}
                    ${GREINSEHGAL}
                    ${GUTILS}
                    ${GGEO}
                    ${GFLUXDRIVERS}
                    ${GMUELOSS}
                    ${GREWEIGHT}
                    ${GNUCLEONDECAY}
                    )

# dk2nu
cet_find_library( DK2NU_TREE  NAMES dk2nuTree  PATHS ENV DK2NUDATA_LIB NO_DEFAULT_PATH )
cet_find_library( DK2NU_GENIE NAMES dk2nuGenie PATHS ENV DK2NUGENIE_LIB NO_DEFAULT_PATH )
include_directories( $ENV{DK2NUDATA_INC} $ENV{DK2NUGENIE_INC} )

# ifdhc
cet_find_library( IFDH NAMES ifdh PATHS ENV IFDHC_LIB NO_DEFAULT_PATH )
include_directories( $ENV{IFDHC_FQ_DIR}/inc )

# ifdhc_art  # as a service
cet_find_library( IFDH_SERVICE NAMES IFDH_service PATHS ENV IFDH_ART_LIB NO_DEFAULT_PATH )
include_directories( $ENV{IFDH_ART_FQ_DIR}/inc )

# macros
include(ArtMake)
include(ArtDictionary)
include(BuildPlugins)
include(CetRootCint)

# include search path
include_directories ( $ENV{GENIE_INC}/GENIE )
include_directories ( $ENV{LIBXML2_INC} )
include_directories ( $ENV{G4INCLUDE} )
include_directories ( $ENV{XERCES_C_INC} )
include_directories ( $ENV{CRYHOME}/src )
include_directories ( $ENV{LOG4CPP_INC} )

# included for Mac OSX using XQuartz for X11
if(APPLE)
  include_directories ( /opt/X11/include )
endif()

# source
add_subdirectory (src)
#add_subdirectory (data)
#add_subdirectory (scripts)
#add_subdirectory (uncertainties)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)

