#include "PanelAlgorithm.h"
#include "Colors.h"

#include "wx/statline.h"

PanelAlgorithm::PanelAlgorithm(wxWindow* parent) : wxScrolledWindow(parent)
{
	SetBackgroundColour(wxColor(Colors::COLOR_MAIN_R, Colors::COLOR_MAIN_G, Colors::COLOR_MAIN_B));

	BuildInterface();
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

void PanelAlgorithm::BuildInterface()
{
	m_AlgorithmParameters = new AlgorithmParameters(this);
	m_AlgorithmOutput = new AlgorithmOutput(this);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Genetic Algorithm Playground");
	sizer->Add(m_AlgorithmParameters, 1, wxEXPAND | wxTOP, 6);
	sizer->Add(new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND);
	sizer->AddSpacer(24);
	sizer->Add(m_AlgorithmOutput, 1, wxEXPAND | wxTOP, 6);

	SetScrollRate(0, 4);
	SetSizer(sizer);
}
