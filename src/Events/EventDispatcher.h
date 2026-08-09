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
	double XPos;
	double YPos;
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
};

inline EventDispatcher g_EventDispatcher;