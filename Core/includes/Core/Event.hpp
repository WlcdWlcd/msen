#pragma once

#include <array>
#include <functional>

namespace msen {

	enum class EventType 
	{
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventsCount

	};

	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	class EventDispatcher {
		public:
			template<typename  EventType>
			void add_event_listener(std::function<void(EventType&)> callback) {
				auto baseCallback = [func = std::move(callback)](BaseEvent& e)
					{
						if (e.get_type() == EventType::type)
						{
							func(static_cast<EventType&>(e));
						};
					};
				m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);

			};

			void dispatch(BaseEvent& event) {
				auto& callback = m_eventCallbacks[static_cast<size_t> (event.get_type())];
				if (callback) {
					callback(event);
				}
			}
		private: 
			std::array <std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;

	};

	struct EventMouseMoved : public BaseEvent {

		double x;
		double y;

		static const EventType type = EventType::MouseMoved;

		EventMouseMoved(const double new_x, const double new_y)
			: x(new_x)
			, y(new_y)
		{}

		virtual EventType get_type() const override {
			return type;
		}
	};


	struct EventWindowResize : public BaseEvent {

		unsigned int width;
		unsigned int height;

		static const EventType type = EventType::WindowResize;

		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			: width(new_width)
			, height(new_height)
		{}

		virtual EventType get_type() const override {
			return type;
		}
	};

	struct EventWindowClose : public BaseEvent {
		static const EventType type = EventType::WindowClose;

		EventWindowClose()
		{}

		virtual EventType get_type() const override {
			return type;
		}
	};

	struct EventMouseButtonPressed : public BaseEvent {

		unsigned int button;
		unsigned int action;
		unsigned int mods;

		static const EventType type = EventType::MouseButtonPressed;

		EventMouseButtonPressed(int button, int action, int mods)
			: button(button)
			, action(action)
			, mods(mods)
		{}

		virtual EventType get_type() const override {
			return type;
		}
	};

}