#include "PanelAlgorithm.h"

#include "wx/statline.h"

PanelAlgorithm::PanelAlgorithm(wxWindow* parent) : wxScrolledWindow(parent)
{
    SetBackgroundColour(wxColor(220, 220, 220));

	m_AlgorithmParameters = new AlgorithmParameters(this);
	m_AlgorithmOutput = new AlgorithmOutput(this);

	//wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Genetic Algorithm Playground");
	sizer->Add(m_AlgorithmParameters, 1, wxEXPAND | wxTOP, 6);
	sizer->Add(new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND);
	sizer->AddSpacer(24);
	sizer->Add(m_AlgorithmOutput, 1, wxEXPAND | wxTOP, 6);

	SetScrollRate(0, 4);
	SetSizer(sizer);
}

PanelAlgorithm::~PanelAlgorithm()
{
	wxDELETE(m_AlgorithmParameters);
	wxDELETE(m_AlgorithmOutput);
}

AlgorithmParameters* PanelAlgorithm::GetAlgorithmParameters()
{
	return m_AlgorithmParameters;
}

AlgorithmOutput* PanelAlgorithm::GetAlgorithmOutput()
{
	return m_AlgorithmOutput;
}
