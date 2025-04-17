//
// Created by ChenhuiWang on 2025/4/17.

// Copyright (c) 2025 Mihoyo. All rights reserved.
//

#include "DetailWindow.h"
#include <imgui.h>

DetailWindow::~DetailWindow()
{
}

void DetailWindow::imgui()
{
    ImGui::Text("%s", m_title.c_str());
}