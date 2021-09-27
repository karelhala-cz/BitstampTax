SET(WX_ROOT_DIR ${PROJECT_SOURCE_DIR}/3rd/wxWidgets)
SET(wxWidgets_CONFIGURATION mswu)
SET(wxWidgets_ROOT_DIR "${PROJECT_SOURCE_DIR}/3rd/wxWidgets")
SET(wxWidgets_LIB_DIR "${PROJECT_SOURCE_DIR}/3rd/wxWidgets/lib/vc_lib")
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
set(wxWidgets_USE_STATIC 1)

include_directories ("${PROJECT_SOURCE_DIR}/src")
 
# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("" FILES ${PROGRAM_SOURCES})
 
 # Define a variable containing a list of source files for the project
set(SRC_FILES
    main.cpp
	BitstampTax.h
	BitstampTax.cpp
	Ids.h
	MainFrame.h
	MainFrame.cpp
	InputParams.h
	InputParams.cpp
    )
	
if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES default.rc)
endif()
	
# Set Properties->General->Configuration Type to Application(.exe)
# Creates BitstampTax.exe with the listed sources (main.cpp)
# Adds sources to the Solution Explorer
add_executable (BitstampTax WIN32 ${SRC_FILES})
 
# Properties->Linker->Input->Additional Dependencies
target_link_libraries (BitstampTax utils input types currency trade_book tax ${wxWidgets_LIBRARIES})
 
# Creates a folder "executables" and adds target 
# project (BitstampTax.vcproj) under it
set_property(TARGET PROPERTY FOLDER "exe")
set_property(TARGET BitstampTax PROPERTY VS_DPI_AWARE "PerMonitor")
 
# Properties->General->Output Directory
set_target_properties(BitstampTax PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
 
# Adds logic to INSTALL.vcproj to copy BitstampTax.exe to destination directory
install (TARGETS BitstampTax
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install)