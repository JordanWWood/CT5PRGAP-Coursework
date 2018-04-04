#pragma once
#include "Events.h"

class Input {
public:
	Input();
	virtual ~Input();

	int Initialize();

	static MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID);

	// These functions do nothing by default and are intended to be overriden
	virtual void OnKeyPressed(const KeyEventArgs& keyEventArgs) { }
	virtual void OnKeyReleased(const KeyEventArgs& keyEventArgs) { }
	virtual void OnMouseMoved(const MouseMotionEventArgs& mouseMotionEventArgs) { }
	virtual void OnMouseButtonPressed(const MouseButtonEventArgs& mouseButtonEventArgs) { }
	virtual void OnMouseButtonReleased(const MouseButtonEventArgs& mouseButtonEventArgs) { }
	virtual void OnMouseWheel(const MouseWheelEventArgs& mouseWheelEventArgs) { }
	virtual void OnResize(const ResizeEventArgs& resizeEventArgs) { }

	void SetDeltaTime(float deltatime) { m_deltatime = deltatime; }
protected:
	float m_deltatime = 1.f;
};

