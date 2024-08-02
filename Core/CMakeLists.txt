cmake_minimum_required(VERSION 3.12)

set(CORE_PROJECT_NAME msenCore)

set(CORE_PUBLIC_INCLUIDES 
	includes/Core/App.hpp
	includes/Core/Log.hpp
	includes/Core/Event.hpp
	
)

set(CORE_PRIVATE_INCLUIDES 
	src/Core/window.hpp
	src/Core/window.cpp
	src/Core/Rendering/OpenGL/ShaderProgram.hpp
	src/Core/Rendering/OpenGL/VertexBuffer.hpp
	src/Core/Rendering/OpenGL/IndexBuffer.hpp
	src/Core/Rendering/OpenGL/BufferLayout.hpp
	src/Core/Rendering/OpenGL/BufferElement.hpp
	src/Core/Rendering/OpenGL/ShaderDataType.hpp
	src/Core/Rendering/OpenGL/VertexArray.hpp
	
	
)


set(CORE_PRIVATE_SOURCES
	src/Core/App.cpp
	src/Core/Rendering/OpenGL/ShaderProgram.cpp
	src/Core/Rendering/OpenGL/VertexBuffer.cpp
	src/Core/Rendering/OpenGL/IndexBuffer.cpp
	src/Core/Rendering/OpenGL/BufferLayout.cpp
	src/Core/Rendering/OpenGL/BufferElement.cpp
	src/Core/Rendering/OpenGL/ShaderDataType.cpp
	src/Core/Rendering/OpenGL/VertexArray.cpp
	
)

set(CORE_ALL_SOUCES 
	${CORE_PUBLIC_INCLUIDES} 
	${CORE_PRIVATE_INCLUIDES}
	${CORE_PRIVATE_SOURCES} 
)
add_library(${CORE_PROJECT_NAME} STATIC
	${CORE_ALL_SOUCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${CORE_ALL_SOUCES}
)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE src)

target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)


add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glad)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_demo.cpp
)

add_library(ImGui STATIC 
	${IMGUI_INCLUDES} 
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/ImGui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE ImGui)
