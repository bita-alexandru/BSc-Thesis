#include "GridTools.h"

GridTools::GridTools(wxWindow* parent): wxPanel(parent)
{
}

GridTools::~GridTools()
{
}

ToolZoom* GridTools::GetToolZoom()
{
    return m_ToolZoom;
}

ToolModes* GridTools::GetToolModes()
{
    return m_ToolModes;
}

ToolCoords* GridTools::GetToolCoords()
{
    return m_ToolCoords;
}
