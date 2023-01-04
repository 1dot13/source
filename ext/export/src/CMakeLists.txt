option(BUILD_JA2EXPORT "Build the Ja2Export tool." ON)

if(BUILD_JA2EXPORT)
	message(STATUS "Configuring Ja2Export")

	add_executable(Ja2Export
		init_vfs.cpp
		main.cpp
		progress_bar.cpp
		ja2/himage.cpp
		ja2/XMLWriter.cpp
		export/jsd/export_jsd.cpp
		export/jsd/structure.cpp
		export/slf/export_slf.cpp
		export/sti/export_sti.cpp
		export/sti/Image.cpp
		export/sti/stci_image_utils.cpp
		export/sti/STCI_lib.cpp
	)
	target_include_directories(Ja2Export PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}"
	)
	target_link_libraries(Ja2Export PUBLIC bfVFS libpng)
	set_target_properties(Ja2Export PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
else()
	message(STATUS "BUILD_JA2EXPORT set to \"OFF\", not configuring Ja2Export by default")
endif()
