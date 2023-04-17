#pragma once

#include "DeviceManager.h"

class Graphics
{
public:
    Graphics(HWND hWnd, int width, int height);
    ~Graphics();

    void BeginFrame();
    void EndFrame();

    void DrawDebugLine(DirectX::SimpleMath::Vector3 startPoint, DirectX::SimpleMath::Vector3 endPoint, DirectX::SimpleMath::Vector4 color);

    struct SimpleVertex
    {
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector4 color;
    };
private:
};

