#include "PanelAlgorithm.h"

PanelAlgorithm::PanelAlgorithm(wxWindow* parent) : wxScrolledWindow(parent)
{
    SetBackgroundColour(wxColor(220, 220, 220));

	m_AlgorithmParameters = new AlgorithmParameters(this);
	m_AlgorithmOutput = new AlgorithmOutput(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_AlgorithmParameters, 1, wxEXPAND | wxALL, 6);
	sizer->Add(m_AlgorithmOutput, 1, wxEXPAND | wxALL, 6);

	SetScrollRate(4, 4);
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
