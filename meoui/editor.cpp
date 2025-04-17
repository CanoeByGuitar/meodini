//
// Created by ChenhuiWang on 2025/4/1.

// Copyright (c) 2025 Mihoyo. All rights reserved.
#include "window/NodeEditorWindow.h"

#include <application.h>
#include <imgui_node_editor.h>
#include <vector>
#include <memory>


namespace ed = ax::NodeEditor;


struct Example : public Application
{
    using Application::Application;

    void OnStart() override
    {
        ed::Config config;
        config.SettingsFile = "Editor.json";
        m_Context = ed::CreateEditor(&config);

        m_windows.push_back(std::make_shared<NodeEditorWindow>("node_editor"));

    }

    void OnStop() override
    {
        ed::DestroyEditor(m_Context);
    }

    void OnFrame(float deltaTime) override
    {
        ed::SetCurrentEditor(m_Context);
        for (const auto& window : m_windows)
        {
            window->imgui();
        }
        ed::SetCurrentEditor(nullptr);
    }


    static void ShowMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z"))
                {
                }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
                {
                } // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X"))
                {
                }
                if (ImGui::MenuItem("Copy", "CTRL+C"))
                {
                }
                if (ImGui::MenuItem("Paste", "CTRL+V"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    ed::EditorContext* m_Context = nullptr;
    std::vector<std::shared_ptr<ImguiWindow>> m_windows;
};

int main(int argc, char** argv)
{
    Example exampe("Simple", argc, argv);

    if (exampe.Create())
        return exampe.Run();

    return 0;
}