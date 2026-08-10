#pragma once
#include <vector>
#include <functional>

struct KeyEvent{
	int Key;
	int Action;
	int Mods;
};

struct MouseMoveEvent
{
	double XOffset;
	double YOffset;
};

struct MouseScrollEvent
{
	double XOffset;
	double YOffset;
};

struct MouseButtonEvent
{
	int Button;
	int Action;
	int Mods;
};

struct FramebufferResizeEvent
{
	int Width;
	int Height;
};

class EventDispatcher
{	
public:
	template<typename T>
	void AddEventListener(std::function<void(const T&)> callback);

	template<typename T>
	void DispatchEvent(const T& event);

	void ClearCallbacks();

private:
	std::vector<std::function<void(const KeyEvent&)>> m_KeyEventCallbacks;
	std::vector<std::function<void(const MouseMoveEvent&)>> m_MouseMoveEventCallbacks;
	std::vector<std::function<void(const MouseScrollEvent&)>> m_MouseScrollEventCallbacks;
	std::vector<std::function<void(const MouseButtonEvent&)>> m_MouseButtonEventCallbacks;
	std::vector<std::function<void(const FramebufferResizeEvent&)>> m_FramebufferResizeEventCallbacks;
};

inline EventDispatcher g_EventDispatcher;