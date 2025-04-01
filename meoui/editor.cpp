//
// Created by ChenhuiWang on 2025/4/1.

// Copyright (c) 2025 Mihoyo. All rights reserved.
//
#define IMGUI_DEFINE_MATH_OPERATORS
#include <application.h>
#include <imgui_node_editor.h>
#include <imgui_internal.h>


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
    }

    void OnStop() override
    {
        ed::DestroyEditor(m_Context);
    }

    void OnFrame(float deltaTime) override
    {
        auto& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();

        ed::SetCurrentEditor(m_Context);

        static float leftPaneWidth  = 400.0f;
        static float rightPaneWidth = 800.0f;
        Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);

        ShowLeftPane(leftPaneWidth - 4.0f);
        ImGui::SameLine(0.0f, 12.0f);

        ed::Begin("My Editor", ImVec2(0.0, 0.0f));
        int uniqueId = 1;
        // Start drawing nodes.
        ed::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ed::BeginPin(uniqueId++, ed::PinKind::Input);
        ImGui::Text("-> In");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(uniqueId++, ed::PinKind::Output);
        ImGui::Text("Out ->");
        ed::EndPin();
        ed::EndNode();
        ed::End();
        ed::SetCurrentEditor(nullptr);

        // ImGui::ShowMetricsWindow();
        // ShowMenuBar();
    }



    static void ShowLeftPane(float paneWidth)
    {
        auto& io = ImGui::GetIO();
        ImGui::BeginChild("Hierarchy", ImVec2(paneWidth, 0));
        ImGui::EndChild();
    }

    static void ShowMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

    }

    ed::EditorContext* m_Context = nullptr;
};

int main(int argc, char** argv)
{
    Example exampe("Simple", argc, argv);

    if (exampe.Create())
        return exampe.Run();

    return 0;
}