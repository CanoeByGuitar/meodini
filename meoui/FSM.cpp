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
};

struct Example : public Application
{
    using Application::Application;
    void OnStart() override
    {
        ed::Config config;
        config.SettingsFile = "StateMachine.json";
        m_Context = ed::CreateEditor(&config);
        m_NextId = 1;
        AddStateNode("Idle");
        AddStateNode("Attack");
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
        ed::Begin("State Machine Editor", ImVec2(0.0, 0.0f));
        for (auto& state : m_States)
        {
            ed::BeginNode(state.id);
            ImGui::PushID(state.id);
            ImGui::SetNextItemWidth(120);
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
                    bool canCreate = true;
                    int fromPin = ((int)startPinId.AsPointer() % 2 == 0) ? startPinId.Get() : endPinId.Get();
                    int toPin = ((int)startPinId.AsPointer() % 2 == 1) ? startPinId.Get() : endPinId.Get();
                    if (canCreate)
                    {
                        if (ed::AcceptNewItem())
                        {
                            m_Links.push_back({ m_NextId++, (int)startPinId.Get(), (int)endPinId.Get() });
                        }
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
};

int main(int argc, char** argv)
{
    Example exampe("StateMachineNodeTool", argc, argv);
    if (exampe.Create())
        return exampe.Run();
    return 0;
}