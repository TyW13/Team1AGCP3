#pragma once

#include <Windows.h>

class Input
{
public:
    Input(HWND hwnd);
    ~Input();

    void Update();

    bool IsKeyDown(UINT key) const;
    bool IsKeyPressed(UINT key) const;
    bool IsKeyReleased(UINT key) const;

private:
    bool m_currentKeys[256] = {};
    bool m_previousKeys[256] = {};
    HWND m_hwnd;
};