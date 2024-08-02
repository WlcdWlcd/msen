#pragma once

#include <string>

#include <Core/Event.hpp>
#include <functional>

struct GLFWwindow;

namespace msen {
	class Window {
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title,const unsigned int width,const unsigned int height);
		virtual ~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator+(const Window&&) = delete;

		void on_update();
		unsigned int get_width()  const	{ return m_data.width; };
		unsigned int get_height() const { return m_data.width; };

		void set_event_callback(const EventCallbackFn& callback) {
			m_data.eventCallbackFn = callback;
		}

	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		float m_background_color[4] = { 0.5f,0.5f,0.5f,0.5f };
		

		WindowData m_data;
		GLFWwindow* m_pWindow = nullptr;

		int init();
		void shutdown();

	};



};


