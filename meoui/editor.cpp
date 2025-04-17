//
// Created by ChenhuiWang on 2025/4/1.

// Copyright (c) 2025 Mihoyo. All rights reserved.
#define IMGUI_DEFINE_MATH_OPERATORS
#include "window/NodeEditorWindow.h"
#include "window/DetailWindow.h"
#include "window/SceneWindow.h"

#include <application.h>
#include <imgui_node_editor.h>
#include <vector>
#include <memory>
#include <imgui_internal.h>

static float topPaneHeight = 500.0f;
static float bottomPaneHeight = 300.0f;
static float leftPaneWidth  = 400.0f;
static float rightPaneWidth = 800.0f;

namespace ed = ax::NodeEditor;

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}


struct Example : public Application
{
    using Application::Application;

    void OnStart() override
    {
        ed::Config config;
        config.SettingsFile = "Editor.json";
        m_Context = ed::CreateEditor(&config);

        m_detailWindow = std::make_shared<DetailWindow>("Detail", leftPaneWidth - 4.0f, 0);
        m_nodeEditorWindow = std::make_shared<NodeEditorWindow>("NodeEditor", 0, bottomPaneHeight - 4.0f);
        m_sceneWindow = std::make_shared<SceneWindow>("Scene", rightPaneWidth - 4.0f, 0);
    }

    void OnStop() override
    {
        ed::DestroyEditor(m_Context);
    }

    void OnFrame(float deltaTime) override
    {
        ed::SetCurrentEditor(m_Context);

        auto& io = ImGui::GetIO();
        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
        ImGui::Separator();

        Splitter(false, 4.0f, &topPaneHeight, &bottomPaneHeight, 50.0f, 50.0f);
        {
            ImGui::BeginChild("Top", ImVec2(0, topPaneHeight - 4.0f));
            Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
            {
                ImGui::BeginChild("a", ImVec2(leftPaneWidth - 4, 0));
                m_detailWindow->imgui();
                ImGui::EndChild();
            }
            ImGui::SameLine(0.0f, 12.0f);
            {
                ImGui::BeginChild("b", ImVec2(rightPaneWidth - 4, 0));
                m_sceneWindow->imgui();
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::Separator();
        {
            ImGui::BeginChild("Bottom", ImVec2(0, bottomPaneHeight - 4.0f));
            m_nodeEditorWindow->imgui();
            ImGui::EndChild();
        }

        ed::SetCurrentEditor(nullptr);
    }



    ed::EditorContext* m_Context = nullptr;

    std::shared_ptr<ImguiWindow> m_detailWindow;
    std::shared_ptr<ImguiWindow> m_nodeEditorWindow;
    std::shared_ptr<ImguiWindow> m_sceneWindow;


};

int main(int argc, char** argv)
{
    Example exampe("Simple", argc, argv);

    if (exampe.Create())
        return exampe.Run();

    return 0;
}