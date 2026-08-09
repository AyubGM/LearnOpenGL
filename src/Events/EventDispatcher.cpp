#include "EventDispatcher.h"

template<> 
void EventDispatcher::AddEventListener<KeyEvent>(std::function<void(const KeyEvent&)> CallbackFn) {
    m_KeyEventCallbacks.push_back(CallbackFn);
}

template<>
void EventDispatcher::AddEventListener<MouseMoveEvent>(std::function<void(const MouseMoveEvent&)> CallbackFn) {
	m_MouseMoveEventCallbacks.push_back(CallbackFn);
}

template<>
void EventDispatcher::AddEventListener<MouseButtonEvent>(std::function<void(const MouseButtonEvent&)> CallbackFn) {
	m_MouseButtonEventCallbacks.push_back(CallbackFn);
}

template<> 
void EventDispatcher::AddEventListener<MouseScrollEvent>(std::function<void(const MouseScrollEvent&)> CallbackFn) {
    m_MouseScrollEventCallbacks.push_back(CallbackFn);
}

template<>
void EventDispatcher::DispatchEvent<KeyEvent>(const KeyEvent& event) {
	for (auto& callback : m_KeyEventCallbacks) {
		callback(event);
	}
}

template<>
void EventDispatcher::DispatchEvent<MouseMoveEvent>(const MouseMoveEvent& event) {
	for (auto& callback : m_MouseMoveEventCallbacks) {
		callback(event);
	}
}

template<>
void EventDispatcher::DispatchEvent<MouseButtonEvent>(const MouseButtonEvent& event) {
	for (auto& callback : m_MouseButtonEventCallbacks) {
		callback(event);
	}
}

template<>
void EventDispatcher::DispatchEvent<MouseScrollEvent>(const MouseScrollEvent& event) {
	for (auto& callback : m_MouseScrollEventCallbacks) {
		callback(event);
	}
}



void EventDispatcher::ClearCallbacks()
{
	m_KeyEventCallbacks.clear();
	m_MouseMoveEventCallbacks.clear();
	m_MouseScrollEventCallbacks.clear();
	m_MouseButtonEventCallbacks.clear();
}