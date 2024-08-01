#pragma once

#include <Core/Event.hpp>

#include <memory>
namespace msen {
	class App {
		public:
			App();
			virtual ~App();

			App(const App&) = delete;
			App& operator=(const App&) = delete;
			App& operator+(const App&&) = delete;

			virtual int start(
				unsigned int window_width,
				unsigned int window_height,
				const char* window_title
			);

			virtual void on_update() {};

	private:
		std::unique_ptr<class Window> m_pWindow;

		EventDispatcher m_event_dispatcher;

		bool m_bCloseWindow = false;

	};
}