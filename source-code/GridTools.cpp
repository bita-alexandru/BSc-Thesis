#include "GridTools.h"

GridTools::GridTools(wxWindow* parent): wxPanel(parent)
{
    SetBackgroundColour(wxColor(253, 255, 182));

    m_ToolZoom = new ToolZoom(this);
    m_ToolUndo = new ToolUndo(this);
    m_ToolModes = new ToolModes(this);
    m_ToolStates = new ToolStates(this);
    m_ToolCoords = new ToolCoords(this);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_ToolZoom, 0, wxEXPAND);
    sizer->Add(m_ToolUndo, 0, wxEXPAND | wxLEFT, 48);
    sizer->Add(m_ToolModes, 0, wxEXPAND | wxLEFT, 48);
    sizer->Add(m_ToolStates, 0, wxEXPAND | wxLEFT, 48);
    sizer->Add(m_ToolCoords, 0, wxEXPAND | wxLEFT, 48);

    this->SetSizerAndFit(sizer);
}

GridTools::~GridTools()
{
    wxDELETE(m_ToolZoom);
    wxDELETE(m_ToolUndo);
    wxDELETE(m_ToolModes);
    wxDELETE(m_ToolStates);
    wxDELETE(m_ToolCoords);
}

ToolZoom* GridTools::GetToolZoom()
{
    return m_ToolZoom;
}

ToolUndo* GridTools::GetToolUndo()
{
    return m_ToolUndo;
}

ToolModes* GridTools::GetToolModes()
{
    return m_ToolModes;
}

ToolStates* GridTools::GetToolStates()
{
    return m_ToolStates;
}

ToolCoords* GridTools::GetToolCoords()
{
    return m_ToolCoords;
}
