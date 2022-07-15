# Collect sources into the variable UTILS_SOURCES without
# having to explicitly list each header and source file.
#
# CMake documentation states "We do not recommend using GLOB to collect a
# list of source files from your source tree. If no CMakeLists.txt file
# changes when a source is added or removed then the generated build system
# cannot know when to ask CMake to regenerate".
file (GLOB UTILS_SOURCES
      "*.h"
	  "*.cpp")
 
# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("" FILES ${UTILS_SOURCES})
 
# Properties->C/C++->General->Additional Include Directories
include_directories ($ENV{WXWIN}/include $ENV{WXWIN}/include/msvc .)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

 
add_library(utils STATIC ${UTILS_SOURCES})
 
# Creates folder "libraries" and adds target project (math.vcproj)
set_property(TARGET utils PROPERTY FOLDER "libs")
 
# Properties->General->Output Directory
set_target_properties(utils PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
 
# Adds logic to INSTALL.vcproj to copy math.dll to the destination directory
install (TARGETS utils
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install)