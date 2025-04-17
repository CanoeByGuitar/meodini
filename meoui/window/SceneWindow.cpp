//
// Created by ChenhuiWang on 2025/4/17.

// Copyright (c) 2025 Mihoyo. All rights reserved.
//

#include "SceneWindow.h"
#include <imgui.h>

SceneWindow::~SceneWindow()
{
}

void SceneWindow::imgui()
{
    ImGui::Text("%s", m_title.c_str());
}