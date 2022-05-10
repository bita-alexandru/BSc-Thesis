#include "AlgorithmOutput.h"

AlgorithmOutput::AlgorithmOutput(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

AlgorithmOutput::~AlgorithmOutput()
{

}

void AlgorithmOutput::SetGrid(Grid* grid)
{
	m_Grid = grid;
}

void AlgorithmOutput::BuildInterface()
{
	m_Start = new wxButton(this, wxID_ANY, "Start");
	m_Start->Bind(wxEVT_BUTTON, &AlgorithmOutput::OnStart, this);

	m_Stop = new wxButton(this, wxID_ANY, "Stop");
	m_Stop->Disable();
	m_Stop->Bind(wxEVT_BUTTON, &AlgorithmOutput::OnStop, this);

	m_Save = new wxButton(this, wxID_ANY, "Save");
	m_Save->Disable();
	m_Save->Bind(wxEVT_BUTTON, &AlgorithmOutput::OnSave, this);

	wxBoxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	sizerButtons->Add(m_Start, 0);
	sizerButtons->Add(m_Stop, 0, wxLEFT | wxRIGHT, 4);
	sizerButtons->Add(m_Save, 0);

	m_TextGeneration = new wxStaticText(this, wxID_ANY, "Generation: -");
	m_TextFitness = new wxStaticText(this, wxID_ANY, "Best Fitness: -");
	m_TextElapsed = new wxStaticText(this, wxID_ANY, "Time Elapsed: -");

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(sizerButtons, 0, wxBOTTOM, 4);
	sizer->Add(m_TextGeneration, 0);
	sizer->AddSpacer(4);
	sizer->Add(m_TextFitness, 0);
	sizer->AddSpacer(4);
	sizer->Add(m_TextElapsed, 0);

	SetSizer(sizer);
}

void AlgorithmOutput::OnStart(wxCommandEvent& evt)
{

}

void AlgorithmOutput::OnStop(wxCommandEvent& evt)
{
}

void AlgorithmOutput::OnSave(wxCommandEvent& evt)
{
}
