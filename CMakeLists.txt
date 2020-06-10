cmake_minimum_required(VERSION 3.10.2)
project(Deku VERSION 1.0)

add_subdirectory(src/mysql)

add_executable(Deku src/start.cxx)

target_link_libraries(Deku PUBLIC mysql_object)

configure_file(version_control.h.in version_control.h)

target_include_directories(Deku PUBLIC 
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/src/mysql"
)
