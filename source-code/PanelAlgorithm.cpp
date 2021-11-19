#include "PanelAlgorithm.h"

PanelAlgorithm::PanelAlgorithm(wxWindow* parent): wxScrolledWindow(parent)
{
	SetBackgroundColour(wxColor(204, 213, 174));
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Algorithm parameters"));

	this->SetScrollRate(4, 4);
	this->SetSizer(sizer);
}

PanelAlgorithm::~PanelAlgorithm()
{
}
