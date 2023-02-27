#include "Input.h"


Input::Input(HWND hwnd)
    : m_hwnd(hwnd)
{
    // Initialize the keyboard state arrays
    ZeroMemory(m_currentKeys, sizeof(m_currentKeys));
    ZeroMemory(m_previousKeys, sizeof(m_previousKeys));
}

Input::~Input()
{
}

void Input::Update()
{
    // Copy the current keys to the previous keys
    memcpy(m_previousKeys, m_currentKeys, sizeof(m_currentKeys));

    // Get the current keyboard state
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);

    // Convert the keyboard state to boolean arrays
    for (int i = 0; i < 256; i++)
    {
        m_currentKeys[i] = (keyboardState[i] & 0x80) != 0;
    }
}

bool Input::IsKeyDown(UINT key) const
{
    return m_currentKeys[key];
}

bool Input::IsKeyPressed(UINT key) const
{
    return m_currentKeys[key] && !m_previousKeys[key];
}

bool Input::IsKeyReleased(UINT key) const
{
    return !m_currentKeys[key] && m_previousKeys[key];
}