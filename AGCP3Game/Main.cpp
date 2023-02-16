//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
//

// https://github.com/microsoft/DirectXTK12 - Sound stuff / Helpers / DDSTex/WIC etc

#include "stdafx.h"
#include "AGCP3.h"

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    AGCP3 sample(1280, 720, L"AGCP3 Game");
    return Win32Application::Run(&sample, hInstance, nCmdShow);
}
