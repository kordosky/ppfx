cmake_minimum_required(VERSION 3.10)
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

project(ppfx VERSION 1.0 LANGUAGES CXX C) #Put some versioning system to keep track of changes.

#In case I need the local version
set( GCC_Flags_For_CXX " -fPIC -DLINUX -O0 -g ${ROOT_CXX_FLAGS}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_Flags_For_CXX}" )
#set(CMAKE_CXX_FLAGS "-Wno-format -Wno-unused-value" )  # get rid of errors from Form and assert
set( CMAKE_CXX_COMPILER "g++")

#Need to specify the CXX Standards
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ROOT REQUIRED COMPONENTS)
if( ${ROOT_FOUND})
MESSAGE("ROOT IS FOUND ${ROOT_VERSION}")
endif()

find_package(BOOST REQUIRED)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} $ENV{BOOSTDIR})

#list(APPEND EXTRA_LIBS "$ENV{PWD}/dk2nu/")
list(APPEND EXTRA_INCLUDES "$ENV{PWD}")

list(APPEND EXTRA_INCLUDES "$ENV{PWD}/include")

list(APPEND EXTRA_INCLUDES "$ENV{ROOTSYS}/include")

#MAC makes the dylib otherwise....
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
set(CMAKE_MACOSX_RPATH OFF)
set(SOURCE_FILES
src/AbsorptionDPIPReweighter.cpp
src/AbsorptionDVOLReweighter.cpp
src/AbsorptionICReweighter.cpp
src/AttenuationMC.cpp
src/CentralValuesAndUncertainties.cpp
src/InteractionChainData.cpp
src/InteractionData.cpp
src/MIPPNumiKaonYieldsReweighter.cpp
src/MIPPNumiMC.cpp
src/MIPPNumiPionYieldsReweighter.cpp
src/MIPPNumiYieldsBins.cpp
src/MakeReweight.cpp
src/NucleonAbsorptionOutOfTargetReweighter.cpp
src/Numi2Pdg.cpp
src/OtherAbsorptionOutOfTargetReweighter.cpp
src/OtherReweighter.cpp
src/ParameterTable.cpp
src/ParticlesThroughVolumesData.cpp
src/ReweightDriver.cpp
src/TargetAttenuationReweighter.cpp
src/TargetData.cpp
src/ThinTargetBins.cpp
src/ThinTargetMC.cpp
src/ThinTargetMesonIncidentReweighter.cpp
src/ThinTargetnCPionReweighter.cpp
src/ThinTargetnucleonAReweighter.cpp
src/ThinTargetpCKaonReweighter.cpp
src/ThinTargetpCNucleonReweighter.cpp
src/ThinTargetpCPionReweighter.cpp
src/nu_g4numi.cpp

)


set(HEADER_FILES
include/AbsorptionDPIPReweighter.h
include/AbsorptionDVOLReweighter.h
include/AbsorptionICReweighter.h
include/AttenuationMC.h
include/CentralValuesAndUncertainties.h
include/CommonIMapIncludes.h
include/Exceptions.h
include/FillIMapHists.h
include/IInteractionChainReweighting.h
include/IInteractionReweighting.h
include/InteractionChainData.h
include/InteractionData.h
include/MIPPNumiKaonYieldsReweighter.h
include/MIPPNumiMC.h
include/MIPPNumiPionYieldsReweighter.h
include/MIPPNumiYieldsBins.h
include/MakeReweight.h
include/NucleonAbsorptionOutOfTargetReweighter.h
include/Numi2Pdg.h
include/OtherAbsorptionOutOfTargetReweighter.h
include/OtherReweighter.h
include/ParameterTable.h
include/ParticlesThroughVolumesData.h
include/ReweightDriver.h
include/TargetAttenuationReweighter.h
include/TargetData.h
include/ThinTargetBins.h
include/ThinTargetMC.h
include/ThinTargetMesonIncidentReweighter.h
include/ThinTargetnCPionReweighter.h
include/ThinTargetnucleonAReweighter.h
include/ThinTargetpCKaonReweighter.h
include/ThinTargetpCNucleonReweighter.h
include/ThinTargetpCPionReweighter.h
include/nu_g4numi.h
)


include(${ROOT_USE_FILE})

set(libraries ${libraries}
              ${ROOT_LIBRARIES}
	      $ENV{ROOTSYS}/lib/libEG.so   #${ROOT_LIBRARIES} doesnt include this library..need to add manually
	      ${PROJECT_SOURCE_DIR}/dk2nu/lib/libdk.so
	      )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${ROOT_INCLUDE_DIR}>
    #$<INSTALL_INTERFACE:include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE ${libraries})

install(TARGETS ${PROJECT_NAME} EXPORT PPFX
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})



install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT PPFX DESTINATION share/PPFX/cmake)


export(TARGETS ${PROJECT_NAME} FILE PPFX.cmake)

add_executable(doReweight_dk2nu ${PROJECT_SOURCE_DIR}/src/doReweight_dk2nu.C)
#target_link_libraries(doReweight_dk2nu PRIVATE ${PROJECT_NAME})
target_link_libraries(doReweight_dk2nu PRIVATE ${PROJECT_NAME} ${libraries})


#MAKE SURE THAT IF YOU ARE RUNNING IN THE MAC DYLD_LIBRARY_PATH points to the path to dk2nu library
