# Demo programs for sequential Viper simulations
# ------------------------------------------------------------------------------

if(NOT ENABLE_MODULE_VEHICLE)
  message(STATUS "  Warning: the ViperRover projects require Chrono::Vehicle!")
  return()
endif()

if(NOT ENABLE_MODULE_IRRLICHT)
  message(STATUS "  Warning: the ViperRover projects require Chrono::Irrlicht!")
  return()
endif()


# ------------------------------------------------------------------------------
# Additional include paths and libraries
# ------------------------------------------------------------------------------

include_directories(${CH_IRRLICHTINC})

# ------------------------------------------------------------------------------

set(DEMOS
    demo_ROBOT_Viper_Rigid
    demo_ROBOT_Viper_SCM
)

# ------------------------------------------------------------------------------
# Add all executables
# ------------------------------------------------------------------------------

foreach(PROGRAM ${DEMOS})
    message(STATUS "...add ${PROGRAM}")
    add_executable(${PROGRAM} "${PROGRAM}.cpp")
    source_group(""  FILES "${PROGRAM}.cpp")

    set_target_properties(${PROGRAM} PROPERTIES
        FOLDER tests
        COMPILE_FLAGS "${CH_CXX_FLAGS} ${CH_IRRLICHT_CXX_FLAGS}"
        LINK_FLAGS "${CH_LINKERFLAG_EXE}")
    set_property(TARGET ${PROGRAM} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROGRAM}>")

    target_link_libraries(${PROGRAM}
                          ChronoEngine
                          ChronoEngine_vehicle
                          ChronoModels_robot
                          ChronoEngine_irrlicht
                        )

    install(TARGETS ${PROGRAM} DESTINATION ${CH_INSTALL_DEMO})
endforeach()