# Properties->C/C++->General->Additional Include Directories
include_directories ("${PROJECT_SOURCE_DIR}/src")
 
# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("" FILES ${PROGRAM_SOURCES})
 
# Set Properties->General->Configuration Type to Application(.exe)
# Creates BitstampTax.exe with the listed sources (main.cpp)
# Adds sources to the Solution Explorer
add_executable (BitstampTax main.cpp InputParams.h InputParams.cpp)
 
# Properties->Linker->Input->Additional Dependencies
target_link_libraries (BitstampTax utils input types currency trade_book tax)
 
# Creates a folder "executables" and adds target 
# project (BitstampTax.vcproj) under it
set_property(TARGET BitstampTax PROPERTY FOLDER "exe")
 
# Properties->General->Output Directory
set_target_properties(BitstampTax PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
 
# Adds logic to INSTALL.vcproj to copy BitstampTax.exe to destination directory
install (TARGETS BitstampTax
         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install)