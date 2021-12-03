#include "ToolZoom.h"

ToolZoom::ToolZoom(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

ToolZoom::~ToolZoom()
{
}

int ToolZoom::GetSize()
{
	return m_Size;
}

void ToolZoom::SetSize(char mode, Grid* grid)
{
	switch (mode)
	{
	case '+':
	{
		if (m_Size == m_MaximumSize)
		{
			// error

			return;
		}

		m_Size *= 2;
		break;
	}
	case '-':
	{
		if (m_Size == m_MinimumSize)
		{
			// error

			return;
		}

		m_Size /= 2;
		break;
	}
	default:
		return;
	}

	grid->SetSize(m_Size);

	std::string label = "Scale=1:" + std::to_string(m_Size);
	m_TextScale->SetLabel(label);
}

void ToolZoom::BuildInterface()
{
	wxButton* zoomIn = new wxButton(this, Ids::ID_ZOOM_IN, "+");
	wxButton* zoomOut = new wxButton(this, Ids::ID_ZOOM_OUT, "-");

	zoomIn->SetToolTip("Zoom in");
	zoomOut->SetToolTip("Zoom out");

	std::string label = "Scale=1:" + std::to_string(m_Size);
	m_TextScale = new wxStaticText(this, wxID_ANY, label);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(zoomIn, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	sizer->Add(zoomOut, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	sizer->Add(m_TextScale, 0, wxALIGN_CENTER_VERTICAL);

	this->SetSizer(sizer);
}
