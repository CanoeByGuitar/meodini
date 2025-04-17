#include "NodeEditorWindow.h"
#include <imgui_node_editor.h>


namespace ed = ax::NodeEditor;

NodeEditorWindow::~NodeEditorWindow()
{

}

void NodeEditorWindow::imgui()
{
//    auto& io = ImGui::GetIO();
//
//    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
//
////    ImGui::Separator();
//    static float topPaneHeight = 500.0f;
//    static float bottomPaneHeight = 300.0f;
//    Splitter(false, 4.0f, &topPaneHeight, &bottomPaneHeight, 50.0f, 50.0f);
//
//    ImGui::BeginChild("Hierarchy", ImVec2(0, topPaneHeight - 4.0f));
//    {
//        static float leftPaneWidth  = 400.0f;
//        static float rightPaneWidth = 800.0f;
//        Splitter(true, 4.0f, &leftPaneWidth, &rightPaneWidth, 50.0f, 50.0f);
//        ShowLeftPane(leftPaneWidth - 4.0f);
//        ImGui::SameLine(0.0f, 12.0f);
//
//        ed::Begin("Node Editor", ImVec2(0.0, 0.0f));
//        int uniqueId = 1;
//        // Start drawing nodes.
//        ed::BeginNode(uniqueId++);
//        ImGui::Text("Node A");
//        ed::BeginPin(uniqueId++, ed::PinKind::Input);
//        ImGui::Text("-> In");
//        ed::EndPin();
//        ImGui::SameLine();
//        ed::BeginPin(uniqueId++, ed::PinKind::Output);
//        ImGui::Text("Out ->");
//        ed::EndPin();
//        ed::EndNode();
//        ed::End();
//    }
//    ImGui::EndChild();
//
//    ImGui::BeginChild("xxx", ImVec2(0, bottomPaneHeight - 4.0f));
//    ImGui::Text("xxx");
//    ImGui::EndChild();

        ed::Begin("Node Editor", ImVec2(0, 0));
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

}
