#include "PanelAlgorithm.h"

PanelAlgorithm::PanelAlgorithm(wxWindow* parent) : wxScrolledWindow(parent)
{
    SetBackgroundColour(wxColor(220, 220, 220));

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, wxString("Algorithm parameters"));

	SetScrollRate(1, 1);
	SetSizerAndFit(sizer);
}

PanelAlgorithm::~PanelAlgorithm()
{
}
