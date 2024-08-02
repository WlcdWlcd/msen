#include <iostream>

#include "Core/app.hpp"
#include "Core/Log.hpp"
#include "Core/window.hpp"

#include <Core/Event.hpp>

namespace msen {
	App::App() {
        LOG_INFO("starting application");
    };

	App:: ~App() {
        LOG_INFO("closing application");
    };

	int App::start(
		unsigned int window_width,
		unsigned int window_height,
		const char* window_title
	) {
		m_pWindow = std::make_unique<Window>(window_title, window_width, window_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event) {
				//LOG_INFO("[EVENT: MOUSE MOVED] mouse moved to {0}x{1}", event.x, event.y);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event) {
				LOG_INFO("[EVENT: WINDOW RESIZED] window resized to {0}x{1}", event.width, event.height);
			}
		);

		m_event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event) {
				LOG_INFO("[EVENT: WINDOW CLOSE] window close requested");
				m_bCloseWindow = true;
				
			}
		);

		m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
			[&](EventMouseButtonPressed& event) {
				LOG_INFO("[EVENT: MOUSE] mouse button pressed/released");
			}
		);

		m_pWindow->set_event_callback(
			[&](BaseEvent& event) {
				m_event_dispatcher.dispatch(event);
			}
		);

		while (!m_bCloseWindow) {
			m_pWindow->on_update();
			on_update();
		}

		return 0;
	}
}