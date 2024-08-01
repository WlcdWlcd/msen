#include "Core/window.hpp"
#include "Core/Log.hpp"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace msen {
    
    static bool s_GLFW_initialized = false;

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