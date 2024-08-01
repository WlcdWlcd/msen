#include "Core/window.hpp"
#include "Core/Log.hpp"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace msen {
    
    static void check_shader(GLuint shader) {
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr,infoLog);
            LOG_CRITICAL(infoLog);
        }
    }
    
    static bool s_GLFW_initialized = false;

    GLfloat points[] = {
        0.0,0.5,0.0,
        0.5,-0.5,0.0,
        -0.5,-0.5,0.0,
    };

    GLfloat colors[] = {
        1.0,0.0,0.0,
        0.0,1.0,0.0,
        0.0,0.0,1.0,
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout (location = 0) in vec3 vertex_position;"
        "layout (location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position,2.0);"
        "}";

    

    const char* fragment_shader = 
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color,1.0);"
        "}";

    GLuint shader_program;
    GLuint vao;


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

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);
        check_shader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);
        check_shader(fs);
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);        
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
	
	}
	void Window::shutdown() { 
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
    }

    void Window::on_update() {
        auto [r, g, b, a] = m_background_color;
        glClearColor( r,g,b,a);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,3);

        

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