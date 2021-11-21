#include "GridTools.h"

GridTools::GridTools(wxWindow* parent): wxPanel(parent)
{
    SetBackgroundColour(wxColor(253, 255, 182));

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    m_ToolZoom = new ToolZoom(this, sizer);
    sizer->AddSpacer(64);

    m_ToolModes = new ToolModes(this, sizer);
    sizer->AddSpacer(64);

    m_ToolCoords = new ToolCoords(this, sizer);

    this->SetSizer(sizer);
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
