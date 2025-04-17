#ifndef IMGUIWINDOW_H
#define IMGUIWINDOW_H

#include <string>

// wrapper for imgui window
class ImguiWindow
{
public:
    ImguiWindow(std::string_view title);
    virtual ~ImguiWindow();

    virtual void imgui() = 0;
    virtual void begin(){};
    virtual void end(){};

protected:
    std::string m_title;
};

#endif // IMGUIWINDOW_H
