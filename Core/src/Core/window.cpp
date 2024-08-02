#include "Core/window.hpp"
#include "Core/Log.hpp"
#include "Core/Rendering/OpenGL/ShaderProgram.hpp"
#include <Core/Rendering/OpenGL/VertexBuffer.hpp>
#include <Core/Rendering/OpenGL/VertexArray.hpp>
#include <Core/Rendering/OpenGL/IndexBuffer.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace msen {
    

    
    static bool s_GLFW_initialized = false;

    GLfloat position_colors [] = {
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,    1.0f ,0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.0f, 1.0f,0.0,       0.0f,0.0f,0.0f, 
         0.0f, -1.0f,-5.0,       1.0f,1.0f,1.0f, 

    };

    GLuint indices[] = {
        0,1,2,3,2,1,0,2,4,1,3,5
    };



    const char* vertex_shader =
        "#version 460\n"
        "layout (location = 0) in vec3 vertex_position;"
        "layout (location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position,1.0);"
        "}";

    

    const char* fragment_shader = 
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color,1.0);"
        "}";



    std::unique_ptr<ShaderProgram> p_shader_program;
    
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer> p_index_buffer;


    std::unique_ptr<VertexArray> p_vao;
    

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data({std::move(title),width,height}){
		int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow,true);
	}
	Window::~Window() {
		shutdown();
	}

	int Window::init() {
        GLFWwindow* window;

        if (!s_GLFW_initialized){
            if (!glfwInit())
            {
                LOG_CRITICAL("failed to initialize GlFW");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        LOG_INFO("GlFW successfully initialized");

        /* Create a windowed mode window and its OpenGL context */
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("failed to create window");
            glfwTerminate();
            return -1;
        }
        LOG_INFO("window successfully created");

        /* Make the window's context current */
        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("failed to initialize GLAD");
            return -1;
        }
        LOG_INFO("GLAD successfully initialized ");

        glfwSetWindowUserPointer(m_pWindow,&m_data);

        glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow,int width,int height) {
            WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(pWindow));

            data.width = width,
            data.height = height;

            EventWindowResize event(width,height);
            data.eventCallbackFn(event);
            });

        glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
            WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(pWindow));


            EventMouseMoved event(x, y);
            data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
            WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(pWindow));

            EventWindowClose event;
            data.eventCallbackFn(event);
            }
        );

        glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* pWindow, int button, int action, int mods) {
            WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(pWindow));

            EventMouseButtonPressed event(button,action,mods);
            data.eventCallbackFn(event);

            }
        );

        glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width,int height) {
            glViewport(0, 0, width, height);
            }
        );
        
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }



        BufferLayout buffer_layout_2_vec3{
            ShaderDataType::Float3 ,
            ShaderDataType::Float3
        };

        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(position_colors,sizeof(position_colors), buffer_layout_2_vec3);
        p_index_buffer = std::make_unique<IndexBuffer>(indices,sizeof(indices)/sizeof(GLuint));
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buffer);

        return 0;
	
	}
	void Window::shutdown() { 
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {
        auto [r, g, b, a] = m_background_color;


        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        p_shader_program->bind();
        p_vao->bind();
        
        glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(p_vao->get_indeces_count()), GL_UNSIGNED_INT,nullptr);

        

        ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("background color");
        ImGui::ColorEdit4("background color",m_background_color);


        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
}