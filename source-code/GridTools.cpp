#include "GridTools.h"

GridTools::GridTools(wxWindow* parent): wxPanel(parent)
{
    SetBackgroundColour(wxColor(253, 255, 182));

    m_ToolZoom = new ToolZoom(this);
    m_ToolModes = new ToolModes(this);
    m_ToolCoords = new ToolCoords(this);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_ToolZoom, 1, wxEXPAND);
    sizer->AddSpacer(64);
    sizer->Add(m_ToolModes, 1, wxEXPAND);
    sizer->AddSpacer(64);
    sizer->Add(m_ToolCoords, 1, wxEXPAND);

    this->SetSizer(sizer);
}

GridTools::~GridTools()
{
    wxDELETE(m_ToolZoom);
    wxDELETE(m_ToolModes);
    wxDELETE(m_ToolCoords);
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
