#include "ImguiWindow.h"
ImguiWindow::ImguiWindow(std::string_view title) :
      m_title(title),
      m_width(0.f),
      m_height(0.f)
{
}

ImguiWindow::ImguiWindow(std::string_view title, float width, float height) :
    m_title(title),
    m_width(width),
    m_height(height)
{

}

ImguiWindow::~ImguiWindow()
{
}

