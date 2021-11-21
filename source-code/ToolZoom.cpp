#include "ToolZoom.h"

ToolZoom::ToolZoom(wxWindow* parent, wxBoxSizer* sizer)
{
	wxButton* zoomIn = new wxButton(parent, ID_ZOOM_IN, "+");
	wxButton* zoomOut = new wxButton(parent, ID_ZOOM_OUT, "-");

	zoomIn->SetToolTip("Zoom in");
	zoomOut->SetToolTip("Zoom out");

	sizer->Add(zoomIn, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	sizer->Add(zoomOut, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	std::string label = "Scale=1:" + std::to_string(m_Size);
	m_TextScale = new wxStaticText(parent, wxID_ANY, label);

	sizer->Add(m_TextScale, 0, wxALIGN_CENTER_VERTICAL);
}

ToolZoom::~ToolZoom()
{
}

int ToolZoom::GetSize()
{
	return m_Size;
}

void ToolZoom::SetSize(char mode)
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

	UpdateTextScale();
}

void ToolZoom::UpdateTextScale()
{
	std::string label = "Scale=1:" + std::to_string(m_Size);
	m_TextScale->SetLabel(label);
}
