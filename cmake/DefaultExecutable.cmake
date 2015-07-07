cmake_minimum_required(VERSION 2.8)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(
    ${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
	${ASSIMP_INCLUDE_PATH}
	${STB_INCLUDE_PATH}
	${OPENAL_INCLUDE_PATH}
	${IMGUI_INCLUDE_PATH}
	${TinyXML_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${CMAKE_SOURCE_DIR}/src/libraries/
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLFW_INCLUDE_GLCOREARB)
add_definitions(-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

add_executable(${ProjectId} ${SOURCES} ${HEADER})

target_link_libraries(
    ${ProjectId}
    ${ALL_LIBRARIES}
    ${GLFW3_LIB}
    ${GLEW_LIB}
    ${OpenGL3_LIB}
	${ASSIMP_LIB}
	${OPENAL_LIB}
	${IMGUI_LIB}
	${TinyXML_LIB}
)

#used to delay in build order
add_dependencies(
	${ProjectId}
	glew
	glfw
	glm
	OpenAL
	imgui
	TinyXML
)

IF (MINGW)

	#Copy needed dll files to current bin folder
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_BINARY_DIR}/dependencies/glew/src/glew-build/bin/libglew.dll      
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_SOURCE_DIR}/dependencies/assimp/libMinGW/libassimp.dll   
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_BINARY_DIR}/dependencies/OpenAL/src/OpenAL-build/OpenAL32.dll   
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_BINARY_DIR}/dependencies/TinyXML/src/TinyXML-build/libtinyxml2.dll   
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	
		

ELSEIF (MSVC)

	#Copy needed dll files to current bin folder
	
	IF(RELEASE_BUILD)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_BINARY_DIR}/dependencies/glew/src/glew-build/bin/Release/glew.dll      
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	ELSE()
		#Copy needed dll files to current bin folder
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  
				${CMAKE_BINARY_DIR}/dependencies/glew/src/glew-build/bin/Debug/glewd.dll      
				$<TARGET_FILE_DIR:${PROJECT_NAME}>
			)	
	ENDIF()
		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/assimp_release-dll_win32/Assimp32.dll   
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	
	#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	#	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
	#		${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/debug/32bit/Assimp32d.dll   
	#		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	#	)
	#	
	#	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
	#		${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/release/32bit/Assimp32.dll   
	#		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	#	)
	#	
	#else()
	#	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
	#		${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/debug/64bit/Assimp64d.dll   
	#		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	#	)
	#	
	#	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E copy_if_different  
	#		${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/release/64bit/Assimp64.dll   
	#		$<TARGET_FILE_DIR:${PROJECT_NAME}>
	#	)
	#endif()
	
	
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_BINARY_DIR}/dependencies/OpenAL/src/OpenAL-build/$<CONFIGURATION>/OpenAL32.dll   
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  
			${CMAKE_BINARY_DIR}/dependencies/TinyXML/src/TinyXML-build/$<CONFIGURATION>/tinyxml2.dll   
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
		
ENDIF()
