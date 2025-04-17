#ifndef NODEEDITORWINDOW_H
#define NODEEDITORWINDOW_H
#include "ImguiWindow.h"


class NodeEditorWindow : public ImguiWindow {
public:
    NodeEditorWindow(std::string_view title):ImguiWindow(title){}
    NodeEditorWindow(std::string_view title, float width, float height): ImguiWindow(title, width, height){}
    ~NodeEditorWindow() override;
    void imgui() override;
};



#endif //NODEEDITORWINDOW_H
