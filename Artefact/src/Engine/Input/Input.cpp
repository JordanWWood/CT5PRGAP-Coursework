#include "ArtifactPCH.h"
#include "Input.h"

namespace Artefact {
    Input::Input() {}


    Input::~Input() {}

    int Input::Initialize() { return 1; }

    MouseButtonEventArgs::MouseButton Input::DecodeMouseButton(UINT messageID) {
        MouseButtonEventArgs::MouseButton mouseButton = MouseButtonEventArgs::None;
        switch (messageID) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK: { mouseButton = MouseButtonEventArgs::Left; }
                               break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK: { mouseButton = MouseButtonEventArgs::Right; }
                               break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK: { mouseButton = MouseButtonEventArgs::Middel; }
                               break;
        }

        return mouseButton;
    }
}

