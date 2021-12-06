#include "ToolZoom.h"

ToolZoom::ToolZoom(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(wxColour(242, 204, 143));

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

	Layout();
}

void ToolZoom::BuildInterface()
{
	wxButton* zoomIn = new wxButton(this, Ids::ID_ZOOM_IN, "+", wxDefaultPosition, wxSize(32, 32));
	wxButton* zoomOut = new wxButton(this, Ids::ID_ZOOM_OUT, "-", wxDefaultPosition, wxSize(32, 32));

	zoomIn->SetToolTip("Zoom in");
	zoomOut->SetToolTip("Zoom out");

	std::string label = "Scale=1:" + std::to_string(m_Size);
	m_TextScale = new wxStaticText(this, wxID_ANY, label);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(zoomOut, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(zoomIn, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_TextScale, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 48);

	this->SetSizerAndFit(sizer);
}
