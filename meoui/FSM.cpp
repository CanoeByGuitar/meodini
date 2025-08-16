#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>
#include "imgui_internal.h"

#include <nlohmann/json.hpp>
#include <application.h>
#include <fstream>

#include <imgui_node_editor.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <vector>
#include <windows.h>


using json = nlohmann::json;
namespace ed = ax::NodeEditor;

class MemoryMappedReader
{
public:
    MemoryMappedReader() = default;
    MemoryMappedReader(const std::string& mapName, size_t size = 256)
            : hMap(nullptr), pBuf(nullptr), mapName_(mapName), size_(size) {}

    ~MemoryMappedReader() {
        if (pBuf) UnmapViewOfFile(pBuf);
        if (hMap) CloseHandle(hMap);
    }

    bool open()
    {
        hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, mapName_.c_str());
        if (!hMap)
        {
            std::cout << GetLastError() << std::endl;
            return false;
        }
        pBuf = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, size_);
        return pBuf != nullptr;
    }

    std::string readStringAndClear() {
        if (!pBuf) return "";
        unsigned char len = *((unsigned char*)pBuf);
        if (len == 0)
            return ""; // 没有新消息
        const char* raw = (const char*)pBuf + 1;
        std::string result(raw, raw + len);
        *((unsigned char*)pBuf) = 0; // 读完后清零，防止重复读取
        return result;
    }


private:
    HANDLE hMap;
    LPVOID pBuf;
    std::string mapName_;
    size_t size_;
};

struct StateNode
{
    int id;
    char name[32];
    int inputPinId;
    int outputPinId;

    int leftInputPin, leftOutputPin;
    int topInputPin, topOutputPin;
    int rightInputPin, rightOutputPin;
    int bottomInputPin, bottomOutputPin;
};

struct TransitionLink
{
    int id;
    int fromPinId;
    int toPinId;
    char condition[256];
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

        m_Reader = MemoryMappedReader("SharedMemory");
        if (!m_Reader.open())
        {
            std::cout << "Faild to open mapping" << std::endl;
        }
    }
    void OnStop() override {
        ed::DestroyEditor(m_Context);
    }
    void OnFrame(float deltaTime) override {
        ImGui::Text("FPS: %.2f (%.2gms)", ImGui::GetIO().Framerate, ImGui::GetIO().Framerate ? 1000.0f / ImGui::GetIO().Framerate : 0.0f);
        ImGui::Separator();
        // 左侧 Node Editor
        ImGui::BeginChild("NodeArea", ImVec2(ImGui::GetWindowWidth()*0.8f, 0), true);
        ed::SetCurrentEditor(m_Context);
        ed::Begin("State Machine Editor", ImVec2(0.0, 0.0f));

        ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
        for (auto& state : m_States)
        {
            ed::BeginNode(state.id);

            ImGui::BeginGroup();
            ed::BeginPin(state.leftInputPin, ed::PinKind::Input);
            ImGui::Text(">> In");
            ed::EndPin();
            ed::BeginPin(state.leftOutputPin, ed::PinKind::Output);
            ImGui::Text("<< Out");
            ed::EndPin();
            ImGui::EndGroup();


            ImGui::SameLine();

            ImGui::PushID(state.id);
            ImGui::SetNextItemWidth(80);
            ImGui::InputText("##StateName", state.name, sizeof(state.name));
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::BeginGroup();
            ed::BeginPin(state.rightInputPin, ed::PinKind::Input);
            ImGui::Text("In <<");
            ed::EndPin();
            ed::BeginPin(state.rightOutputPin, ed::PinKind::Output);
            ImGui::Text("Out >>");
            ed::EndPin();
            ImGui::EndGroup();


            ed::EndNode();
        }

        for (auto& link : m_Links)
        {
            ImVec2 p1 = ed::GetPinPosition(link.fromPinId, ed::PinKind::Output);
            ImVec2 p2 = ed::GetPinPosition(link.toPinId, ed::PinKind::Input);
            ImVec2 screen1 = ed::CanvasToScreen(p1);
            ImVec2 screen2 = ed::CanvasToScreen(p2);

            ImVec2 mid = (screen1 + screen2) * 0.5f;
            ImVec2 dir = screen2 - screen1;
            float len = sqrt(dir.x*dir.x + dir.y*dir.y);
            if(len < 1e-3f) continue;
            dir.x /= len; dir.y /= len;

            float arrow_length = 14.0f;
            float arrow_width  = 8.0f;

            ImVec2 tip = mid + ImVec2(dir.x * arrow_length/2, dir.y*arrow_length/2);
            ImVec2 ortho(-dir.y, dir.x); // 旋转90度为垂直
            ImVec2 left = mid - dir*arrow_length/2 + ortho*arrow_width/2;
            ImVec2 right = mid - dir*arrow_length/2 - ortho*arrow_width/2;

            ImDrawList* draw_list = ImGui::GetForegroundDrawList(); // 推荐用前景drawlist
            draw_list->AddTriangleFilled(tip, left, right, IM_COL32(220,120,50,200));

            ed::Link(link.id, link.fromPinId, link.toPinId);
        }

        ed::PopStyleVar();

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
        if (ImGui::Button("Save Graph"))
        {
            SaveGraphJson("graph.json");
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Graph"))
        {
            LoadGraphJson("graph.json");
        }
        if (ImGui::Button("Generate Runtime Json"))
        {

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

        std::string msg = m_Reader.readStringAndClear();
        if (!msg.empty()) {
            std::cout << "Received from Unity: " << msg << std::endl;
            m_Name2Id.clear();
            for (const auto& state : m_States)
            {
                std::string name = state.name;
                int id = state.id;
                m_Name2Id[name] = id;
            }
            ed::SelectNode(m_Name2Id[msg]);
        }

        ed::SetCurrentEditor(nullptr);
    }

    void AddStateNode(const char* name)
    {
        int nodeId = m_NextId++;
        StateNode node;
        node.id = nodeId;
        strncpy(node.name, name, sizeof(node.name));
        node.name[sizeof(node.name)-1] = 0;
        node.leftInputPin = m_NextId++;
        node.leftOutputPin = m_NextId++;
        node.topInputPin = m_NextId++;
        node.topOutputPin = m_NextId++;
        node.rightInputPin = m_NextId++;
        node.rightOutputPin = m_NextId++;
        node.bottomInputPin = m_NextId++;
        node.bottomOutputPin = m_NextId++;

        // m_Name2Id[node.name] = nodeId;
        m_States.push_back(node);
    }

    void SaveGraphJson(const char* filename)
    {
        json j;
        for (const auto& node : m_States)
        {
            j["states"].push_back({
                {"id", node.id},
                {"name", node.name},
                {"leftInputPin", node.leftInputPin},
                {"leftOutputPin", node.leftOutputPin},
                {"topInputPin", node.topInputPin},
                {"topOutputPin", node.topOutputPin},
                {"rightInputPin", node.rightInputPin},
                {"rightOutputPin", node.rightOutputPin},
                {"bottomInputPin", node.bottomInputPin},
                {"bottomOutputPin", node.bottomOutputPin}
            });
        }
        for (const auto& link : m_Links)
        {
            j["links"].push_back({
                {"id", link.id},
                {"fromPinId", link.fromPinId},
                {"toPinId", link.toPinId},
                {"condition", link.condition}
            });
        }
        j["next_id"] = m_NextId;
        std::ofstream fs(filename);
        fs << j.dump(4);
    }

    void LoadGraphJson(const char* filename)
    {
        std::ifstream fs(filename);
        if (!fs) return;
        json j;
        fs >> j;
        m_States.clear();
        for (auto& js : j["states"])
        {
            StateNode node;
            node.id = js["id"];
            strncpy(node.name, js["name"].get<std::string>().c_str(), sizeof(node.name));
            node.name[sizeof(node.name)-1] = 0;
            node.leftInputPin = js["leftInputPin"];
            node.leftOutputPin = js["leftOutputPin"];
            node.topInputPin = js["topInputPin"];
            node.topOutputPin = js["topOutputPin"];
            node.rightInputPin = js["rightInputPin"];
            node.rightOutputPin = js["rightOutputPin"];
            node.bottomInputPin = js["bottomInputPin"];
            node.bottomOutputPin = js["bottomOutputPin"];
            m_States.push_back(node);
        }
        m_Links.clear();
        for (auto& jl : j["links"])
        {
            TransitionLink link;
            link.id = jl["id"];
            link.fromPinId = jl["fromPinId"];
            link.toPinId = jl["toPinId"];
            strncpy(link.condition, jl["condition"].get<std::string>().c_str(), sizeof(link.condition));
            link.condition[sizeof(link.condition)-1] = 0;
            m_Links.push_back(link);
        }
        if (j.contains("next_id"))
            m_NextId = j["next_id"];
    }

    ed::EditorContext* m_Context = nullptr;
    int m_NextId = 1;
    std::vector<StateNode> m_States;
    std::vector<TransitionLink> m_Links;
    int selectedLinkId = -1;
    std::unordered_map<std::string, int> m_Name2Id;
    MemoryMappedReader m_Reader;
};




int main(int argc, char** argv)
{
    Example exampe("StateMachineNodeTool", argc, argv);
    if (exampe.Create())
        return exampe.Run();
    return 0;
}