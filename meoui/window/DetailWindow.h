//
// Created by ChenhuiWang on 2025/4/17.

// Copyright (c) 2025 Mihoyo. All rights reserved.
//

#ifndef MEODINI_DETAILWINDOW_H
#define MEODINI_DETAILWINDOW_H


#include "ImguiWindow.h"


class DetailWindow : public ImguiWindow {
public:
    DetailWindow(std::string_view title):ImguiWindow(title){}
    DetailWindow(std::string_view title, float width, float height): ImguiWindow(title, width, height){}
    ~DetailWindow() override;
    void imgui() override;
};


#endif // MEODINI_DETAILWINDOW_H
