#pragma once
#include "Graphics/Context.h"
#include "Input/Input.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

namespace Artefact {
    // High level interaction with rendering for simple and effective interaction. Creates a DirectX context for the game.
    class GameParent {
    public:
        GameParent();
        GameParent(BOOL, HINSTANCE*, int*, Input*);
        virtual ~GameParent();

        int Init();

        virtual int Run();
        virtual void Update(float) = 0;
    protected:
        class Context m_Context;
        Input* m_Input;

    private:
        HINSTANCE * m_HInstance = nullptr;
    };
}