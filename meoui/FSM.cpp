#include <imgui.h>
#include <imgui_node_editor.h>
#include <application.h>
#include <vector>
#include <string>
namespace ed = ax::NodeEditor;

struct StateNode
{
    int id;
    char name[32];
    int inputPinId;
    int outputPinId;
};
struct TransitionLink
{
    int id;
    int fromPinId;
    int toPinId;
    char condition[64];
};
struct Example : public Application
{
    using Application::Application;
    void OnStart() override {
        ed::Config config;
        config.SettingsFile = "StateMachine.json";
        m_Context = ed::CreateEditor(&config);
        m_NextId = 1;
        AddStateNode("Idle");
        AddStateNode("Attack");
        selectedLinkId = -1;
    }
    void OnStop() override {
        ed::DestroyEditor(m_Context);
    }
    void OnFrame(float deltaTime) override {
        ImGui::Text("FPS: %.2f (%.2gms)", ImGui::GetIO().Framerate, ImGui::GetIO().Framerate ? 1000.0f / ImGui::GetIO().Framerate : 0.0f);
        ImGui::Separator();

        // 左侧 Node Editor
        ImGui::BeginChild("NodeArea", ImVec2(ImGui::GetWindowWidth()*0.65f, 0), true);

        ed::SetCurrentEditor(m_Context);
        ed::Begin("State Machine Editor", ImVec2(0.0, 0.0f));
        for (auto& state : m_States)
        {
            ed::BeginNode(state.id);
            ImGui::PushID(state.id);
            ImGui::SetNextItemWidth(80);
            ImGui::InputText("##StateName", state.name, sizeof(state.name));
            ImGui::PopID();
            ed::BeginPin(state.inputPinId, ed::PinKind::Input);
            ImGui::Text(">> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(state.outputPinId, ed::PinKind::Output);
            ImGui::Text("Out >>");
            ed::EndPin();
            ed::EndNode();
        }
        for (auto& link : m_Links)
            ed::Link(link.id, link.fromPinId, link.toPinId);

        if (ed::BeginCreate())
        {
            ed::PinId startPinId, endPinId;
            if (ed::QueryNewLink(&startPinId, &endPinId))
            {
                if (startPinId && endPinId)
                {
                    if (ed::AcceptNewItem())
                    {
                        TransitionLink link;
                        link.id = m_NextId++;
                        link.fromPinId = startPinId.Get();
                        link.toPinId = endPinId.Get();
                        link.condition[0] = 0;
                        m_Links.push_back(link);
                    }
                }
            }
        }
        ed::EndCreate();
        if (ed::BeginDelete())
        {
            ed::LinkId linkId = 0;
            while (ed::QueryDeletedLink(&linkId))
            {
                if (ed::AcceptDeletedItem())
                {
                    auto it = std::remove_if(m_Links.begin(), m_Links.end(),
                        [linkId](const TransitionLink& l) { return l.id == linkId.Get(); });
                    m_Links.erase(it, m_Links.end());
                }
            }
            ed::EndDelete();
        }
        if (ImGui::Button("Add State"))
        {
            AddStateNode(("State" + std::to_string(m_States.size() + 1)).c_str());
        }
        ed::End();

        ImGui::EndChild();

        // 右侧Detail面板
        ImGui::SameLine();
        ImGui::BeginChild("DetailPanel", ImVec2(0, 0), true);

        int selCount = ed::GetSelectedObjectCount();
        ed::LinkId selLinkId;
        TransitionLink* selLink = nullptr;
        if (selCount == 1 && ed::GetSelectedLinks(&selLinkId, 1))
        {
            int linkid = selLinkId.Get();
            for (auto& link : m_Links)
                if (link.id == linkid)
                    selLink = &link;
        }
        if (selLink) {
            ImGui::Text("Selected Transition");
            ImGui::Separator();
            ImGui::Text("ID: %d", selLink->id);
            ImGui::Text("From Pin: %d", selLink->fromPinId);
            ImGui::Text("To Pin: %d", selLink->toPinId);
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("Condition", selLink->condition, sizeof(selLink->condition));
        } else {
            ImGui::Text("No Transition Selected.");
        }
        ImGui::EndChild();
        ed::SetCurrentEditor(nullptr);
    }
    void AddStateNode(const char* name)
    {
        int nodeId = m_NextId++;
        int inputPinId = m_NextId++;
        int outputPinId = m_NextId++;
        StateNode node;
        node.id = nodeId;
        strncpy(node.name, name, sizeof(node.name));
        node.name[sizeof(node.name)-1] = 0;
        node.inputPinId = inputPinId;
        node.outputPinId = outputPinId;
        m_States.push_back(node);
    }
    ed::EditorContext* m_Context = nullptr;
    int m_NextId = 1;
    std::vector<StateNode> m_States;
    std::vector<TransitionLink> m_Links;
    int selectedLinkId = -1;
};
int main(int argc, char** argv)
{
    Example exampe("StateMachineNodeTool", argc, argv);
    if (exampe.Create())
        return exampe.Run();
    return 0;
}