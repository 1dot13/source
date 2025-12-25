function(CopyUserPresetTemplate)
	if(
		NOT DEFINED Languages AND
		NOT DEFINED Applications AND
		NOT EXISTS "${CMAKE_SOURCE_DIR}/CMakePresets.json" AND
		NOT EXISTS "${CMAKE_SOURCE_DIR}/CMakeUserPresets.json"
	)
		file(COPY "${CMAKE_SOURCE_DIR}/cmake/presets/CMakePresets.json" DESTINATION "${CMAKE_SOURCE_DIR}")
		message(FATAL_ERROR "No existing preset was found, copied a preset template to ${CMAKE_SOURCE_DIR}/CMakePresets.json.")
	endif()
endfunction()
