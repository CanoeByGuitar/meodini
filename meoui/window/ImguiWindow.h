#ifndef IMGUIWINDOW_H
#define IMGUIWINDOW_H

#include <string>

// wrapper for imgui window
class ImguiWindow
{
public:
    ImguiWindow(std::string_view title);
    ImguiWindow(std::string_view title, float width, float height);
    virtual ~ImguiWindow();

    virtual void imgui() = 0;
    virtual void begin(){};
    virtual void end(){};

protected:
    std::string m_title;
    float m_width = 0.f;
    float m_height = 0.f;
};

#endif // IMGUIWINDOW_H
