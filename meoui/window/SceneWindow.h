//
// Created by ChenhuiWang on 2025/4/17.

// Copyright (c) 2025 Mihoyo. All rights reserved.
//

#ifndef MEODINI_SCENEWINDOW_H
#define MEODINI_SCENEWINDOW_H


#include "ImguiWindow.h"


class SceneWindow : public ImguiWindow {
public:
    SceneWindow(std::string_view title):ImguiWindow(title){}
    SceneWindow(std::string_view title, float width, float height): ImguiWindow(title, width, height){}
    ~SceneWindow() override;
    void imgui() override;
};


#endif // MEODINI_SCENEWINDOW_H
