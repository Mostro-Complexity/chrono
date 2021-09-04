#=============================================================================
# CMake configuration file for demos of various Chrono::Vehicle models.
# These example programs require Irrlicht run-time visualization.
#=============================================================================

if(NOT ENABLE_MODULE_IRRLICHT)
    return()
endif()

#--------------------------------------------------------------
# List all demos

set(DEMOS
    demo_VEH_M113
    demo_VEH_M113_DefSoil
    demo_VEH_Marder
    demo_VEH_MarderRide
    demo_VEH_MarderObstacle
    demo_VEH_MarderObstacleTool
    demo_VEH_MarderShock
    demo_VEH_MarderShockTool
    demo_VEH_TrackedJSON
    demo_VEH_TrackedJSON_Band
)

if(ENABLE_MODULE_PARDISO_MKL OR ENABLE_MODULE_MUMPS)
    set(DEMOS ${DEMOS}
        demo_VEH_M113_Band
    )
endif()

#--------------------------------------------------------------
# Include directories, compiler flags, libraries

include_directories(${CH_IRRLICHTINC})
set(COMPILER_FLAGS "${CH_CXX_FLAGS}  ${CH_IRRLICHT_CXX_FLAGS}")
set(LINKER_FLAGS "${CH_LINKERFLAG_EXE}")
list(APPEND LIBS "")

if(ENABLE_MODULE_PARDISO_MKL)
    include_directories(${CH_MKL_INCLUDES})
    set(COMPILER_FLAGS "${COMPILER_FLAGS} ${CH_MKL_CXX_FLAGS}")
    set(LINKER_FLAGS "${LINKER_FLAGS} ${CH_MKL_LINK_FLAGS}")
    list(APPEND LIBS "ChronoEngine_pardisomkl")
endif()

if(ENABLE_MODULE_MUMPS)
    include_directories(${CH_MUMPS_INCLUDES})
    list(APPEND LIBS "ChronoEngine_mumps")
endif()

#--------------------------------------------------------------
# Create the executables

foreach(PROGRAM ${DEMOS})
    message(STATUS "...add ${PROGRAM}")

    add_executable(${PROGRAM} ${PROGRAM}.cpp)
    source_group("" FILES ${PROGRAM}.cpp)

    set_target_properties(${PROGRAM} PROPERTIES
                          COMPILE_FLAGS "${COMPILER_FLAGS}"
                          LINK_FLAGS "${LINKER_FLAGS}")
    set_property(TARGET ${PROGRAM} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROGRAM}>")
    target_link_libraries(${PROGRAM}
                          ChronoEngine
                          ChronoEngine_irrlicht
                          ChronoEngine_vehicle
                          ChronoEngine_vehicle_irrlicht
                          ChronoModels_vehicle
                          ${LIBS})

    install(TARGETS ${PROGRAM} DESTINATION ${CH_INSTALL_DEMO})
endforeach()