#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(neutrondetector)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Neutron neutron.cc ${sources} ${headers})
target_link_libraries(Neutron ${Geant4_LIBRARIES})
target_compile_definitions(Neutron PRIVATE
  AMBE_OUTPUT_FILE="${PROJECT_SOURCE_DIR}/../macro/ambe/ambe.root")

# Source-only audit: reuse the production generator and physics without the
# detector/EventAction selection or its one-row-per-decay neutron-energy field.
find_package(ROOT REQUIRED COMPONENTS RIO Tree Hist)
add_executable(AmBeSourceCheck ../macro/ambe/source_check.cc
  src/PrimaryGeneratorAction.cc src/PhysicsList.cc
  src/HadronElasticPhysicsHP.cc src/ElectromagneticPhysics.cc
  src/GammaNuclearPhysics.cc src/RadioactiveDecayPhysics.cc)
target_link_libraries(AmBeSourceCheck ${Geant4_LIBRARIES} ROOT::RIO ROOT::Tree ROOT::Hist)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
  #init_vis.mac
  #vis.mac
  vis2.mac
  run.mac
  )

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(neutrondetector DEPENDS Neutron)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Neutron DESTINATION bin)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
