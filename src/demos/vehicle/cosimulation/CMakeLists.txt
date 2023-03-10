# ------------------------------------------------------------------------------
# Co-simulation framework demos
# ------------------------------------------------------------------------------

if(NOT MPI_FOUND)
  return()
endif()

#message(STATUS "COSIM CXX: ${CH_VEHCOSIM_CXX_FLAGS}")
#message(STATUS "COSIM LINK: ${CH_VEHCOSIM_LINKER_FLAGS}")
#message(STATUS "COSIM INCLUDES: ${CH_VEHCOSIM_INCLUDES}")
#message(STATUS "COSIM LIBS: ${CH_VEHCOSIM_LIBRARIES}")

# ------------------------------------------------------------------------------

set(PROGRAMS 
    demo_VEH_Cosim_WheelRig
    demo_VEH_Cosim_WheelRig_CustomTerrain
    demo_VEH_Cosim_WheeledVehicle
    demo_VEH_Cosim_TrackedVehicle
    demo_VEH_Cosim_Viper
    demo_VEH_Cosim_Curiosity
    )

#if(ENABLE_MODULE_DISTRIBUTED)
#  set(PROGRAMS ${PROGRAMS}
#      demo_VEH_Cosim_WheelRig_DistributedTerrain
#      )
#endif()

include_directories(${CH_VEHCOSIM_INCLUDES})

FOREACH(PROGRAM ${PROGRAMS})
  message(STATUS "...add ${PROGRAM}")

  add_executable(${PROGRAM} ${PROGRAM}.cpp)
  source_group("" FILES ${PROGRAM}.cpp)

  set_target_properties(${PROGRAM} PROPERTIES 
      COMPILE_FLAGS "${CH_VEHCOSIM_CXX_FLAGS}"
      LINK_FLAGS "${CH_VEHCOSIM_LINKER_FLAGS}")

  target_link_libraries(${PROGRAM} ChronoEngine_vehicle_cosim ${CH_VEHCOSIM_LIBRARIES})
ENDFOREACH()
