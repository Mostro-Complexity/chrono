#=============================================================================
# CMake configuration file for the Chrono postprocess module
# 
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#=============================================================================

option(ENABLE_MODULE_POSTPROCESS "Enable the Chrono Postprocess module" ON)

# Return now if this module is not enabled
IF(NOT ENABLE_MODULE_POSTPROCESS)
	RETURN()
ENDIF()

MESSAGE(STATUS "\n==== Chrono Postprocess module ====\n")

#-----------------------------------------------------------------------------
# LIST THE FILES THAT MAKE THE POSTPROCESS LIBRARY

SET(ChronoEngine_POSTPROCESS_SOURCES 
    ChPovRay.cpp
)

SET(ChronoEngine_POSTPROCESS_HEADERS
    ChApiPostProcess.h 
    ChPostProcessBase.h
    ChGnuPlot.h
    ChPovRay.h
)

SOURCE_GROUP("" FILES 
            ${ChronoEngine_POSTPROCESS_SOURCES} 
            ${ChronoEngine_POSTPROCESS_HEADERS})

#-----------------------------------------------------------------------------	
# In most cases, you do not need to edit the lines below.

# The POSTPROCESS library is added to the project,
# and some custom properties of this target are set.

ADD_LIBRARY(ChronoEngine_postprocess 
			${ChronoEngine_POSTPROCESS_SOURCES}
			${ChronoEngine_POSTPROCESS_HEADERS})

SET_TARGET_PROPERTIES(ChronoEngine_postprocess PROPERTIES
                      COMPILE_FLAGS "${CH_CXX_FLAGS}"
                      LINK_FLAGS "${CH_LINKERFLAG_SHARED}")
                          
target_compile_definitions(ChronoEngine_postprocess PRIVATE "CH_API_COMPILE_POSTPROCESS")
target_compile_definitions(ChronoEngine_postprocess PRIVATE "CH_IGNORE_DEPRECATED")

TARGET_LINK_LIBRARIES(ChronoEngine_postprocess 
	                  ChronoEngine)
	
ADD_DEPENDENCIES(ChronoEngine_postprocess ChronoEngine)
	
INSTALL(TARGETS ChronoEngine_postprocess
    EXPORT Postprocess
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)		

export(EXPORT Postprocess
    NAMESPACE Chrono::
    FILE ChronoPostprocessTargets.cmake
)

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ChronoPostprocessTargets.cmake
#     DESTINATION cmake
# )

install(
    EXPORT Postprocess
    FILE ChronoPostprocessConfig.cmake
    NAMESPACE Chrono::
    DESTINATION cmake
)

# Install all header files for the main ChronoEngine library
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include/chrono_postprocess
    FILES_MATCHING REGEX ".*\\.(h|inl)$")