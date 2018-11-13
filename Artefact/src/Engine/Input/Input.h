#pragma once
#include "Events.h"

namespace Artefact {
    class Input {
    public:
        Input();
        virtual ~Input();

        int Initialize();

        static MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT);

        // These functions do nothing by default and are intended to be overriden
        virtual void OnKeyPressed(const KeyEventArgs&) { }
        virtual void OnKeyReleased(const KeyEventArgs&) { }
        virtual void OnMouseMoved(const MouseMotionEventArgs&) { }
        virtual void OnMouseButtonPressed(const MouseButtonEventArgs&) { }
        virtual void OnMouseButtonReleased(const MouseButtonEventArgs&) { }
        virtual void OnMouseWheel(const MouseWheelEventArgs&) { }
        virtual void OnResize(const ResizeEventArgs&) { }

        void SetDeltaTime(float deltatime) { m_deltatime = deltatime; }
    protected:
        float m_deltatime = 1.f;
    };
}
