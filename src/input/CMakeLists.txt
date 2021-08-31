# Collect sources into the variable INPUT_SOURCES without
# having to explicitly list each header and source file.
#
# CMake documentation states "We do not recommend using GLOB to collect a
# list of source files from your source tree. If no CMakeLists.txt file
# changes when a source is added or removed then the generated build system
# cannot know when to ask CMake to regenerate".
file (GLOB INPUT_SOURCES
      "*.h"
	  "*.cpp")
 
# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("" FILES ${INPUT_SOURCES})
 
# Properties->C/C++->General->Additional Include Directories
include_directories (.)
 
add_library(input STATIC ${INPUT_SOURCES})
 
# Creates folder "libraries" and adds target project (math.vcproj)
set_property(TARGET input PROPERTY FOLDER "libs")
 
# Properties->General->Output Directory
set_target_properties(input PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
 
# Adds logic to INSTALL.vcproj to copy math.dll to the destination directory
install (TARGETS input
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install)