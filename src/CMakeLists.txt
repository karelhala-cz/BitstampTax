# Properties->C/C++->General->Additional Include Directories
include_directories ("${PROJECT_SOURCE_DIR}/src")
 
add_subdirectory(app)
add_subdirectory(currency)
add_subdirectory(input)
add_subdirectory(tax)
add_subdirectory(trade_book)
add_subdirectory(types)
add_subdirectory(utils)