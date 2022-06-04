#include "AlgorithmParameters.h"

#include "wx/spinctrl.h"

AlgorithmParameters::AlgorithmParameters(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

AlgorithmParameters::~AlgorithmParameters()
{
}

void AlgorithmParameters::BuildInterface()
{
	// POP SIZE, PM, PC
	wxStaticText* textPS = new wxStaticText(this, wxID_ANY, "Population Size");
	textPS->SetToolTip("2 - 10");
	wxSpinCtrl* spinPS = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinPS->SetRange(2, 10);
	spinPS->SetValue(2);

	wxStaticText* textPM = new wxStaticText(this, wxID_ANY, "Mutation Probability");
	textPM->SetToolTip("0.001 - 1.000");
	wxSpinCtrlDouble* spinPM = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinPM->SetDigits(3);
	spinPM->SetRange(0.001, 1.0);
	spinPM->SetIncrement(0.05);
	spinPM->SetValue(0.1);

	wxStaticText* textPC = new wxStaticText(this, wxID_ANY, "Crossover Probability");
	textPC->SetToolTip("0.001 - 1.000");
	wxSpinCtrlDouble* spinPC = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinPC->SetDigits(3);
	spinPC->SetRange(0.001, 1.0);
	spinPC->SetIncrement(0.05);
	spinPC->SetValue(0.1);

	wxGridSizer* sizerP = new wxGridSizer(4, 3, 0, 6);
	sizerP->Add(textPS, 0);
	sizerP->Add(textPM, 0);
	sizerP->Add(textPC, 0);
	sizerP->Add(spinPS, 0, wxEXPAND);
	sizerP->Add(spinPM, 0, wxEXPAND);
	sizerP->Add(spinPC, 0, wxEXPAND);

	// FITNESS MULTIPLIERS
	wxStaticText* textPFM = new wxStaticText(this, wxID_ANY, "Population Fitness Multiplier");
	textPFM->SetToolTip("0.0 - 1.0");
	wxSpinCtrlDouble* spinPFM = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinPFM->SetDigits(1);
	spinPFM->SetRange(0.0, 1.0);
	spinPFM->SetIncrement(0.1);
	spinPFM->SetValue(1.0);

	wxStaticText* textGFM = new wxStaticText(this, wxID_ANY, "Generation Fitness Multiplier");
	textGFM->SetToolTip("0.0 - 1.0");
	wxSpinCtrlDouble* spinGFM = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinGFM->SetDigits(1);
	spinGFM->SetRange(0.0, 1.0);
	spinGFM->SetIncrement(0.1);
	spinGFM->SetValue(1.0);

	wxStaticText* textISFM = new wxStaticText(this, wxID_ANY, "Initial Size Fitness Multiplier");
	textISFM->SetToolTip("0.0 - 1.0");
	wxSpinCtrlDouble* spinISFM = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinISFM->SetDigits(1);
	spinISFM->SetRange(0, 1.0);
	spinISFM->SetIncrement(0.1);
	spinISFM->SetValue(1.0);

	sizerP->Add(textPFM, 0);
	sizerP->Add(textGFM, 0);
	sizerP->Add(textISFM, 0);
	sizerP->Add(spinPFM, 0, wxEXPAND);
	sizerP->Add(spinGFM, 0, wxEXPAND);
	sizerP->Add(spinISFM, 0, wxEXPAND);

	// SELECTION
	wxStaticText* textSelection = new wxStaticText(this, wxID_ANY, "Selection Method");
	wxComboBox* comboSelection = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, {}, wxCB_READONLY | wxCB_DROPDOWN);
	comboSelection->Set({"Roulette Wheel Selection", "Rank Selection", "Steady State Selection", "Tournament Selection", "Elitism Selection", "Boltzmann Selection"});
	comboSelection->SetValue("Roulette Wheel Selection");

	wxBoxSizer* sizerSelection = new wxBoxSizer(wxHORIZONTAL);
	sizerSelection->Add(textSelection, 0, wxALIGN_CENTER_VERTICAL);
	sizerSelection->Add(comboSelection, 0, wxEXPAND | wxLEFT, 8);

	// PATTERN TARGETS
	wxStaticText* textPT = new wxStaticText(this, wxID_ANY, "Population Target");
	textPT->SetToolTip("0 - 1000");
	wxSpinCtrl* spinPT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinPT->SetRange(0, 1000);
	spinPT->SetValue(0);

	wxStaticText* textGT = new wxStaticText(this, wxID_ANY, "Generation Target");
	textGT->SetToolTip("0 - 1000");
	wxSpinCtrl* spinGT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinGT->SetRange(0, 1000);
	spinGT->SetValue(0);

	wxStaticText* textET = new wxStaticText(this, wxID_ANY, "Epochs Target");
	textET->SetToolTip("0 - 1000");
	wxSpinCtrl* spinET = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinET->SetRange(0, 1000);
	spinET->SetValue(0);

	wxGridSizer* sizerTargets = new wxGridSizer(2, 3, 0, 6);
	sizerTargets->Add(textPT, 0);
	sizerTargets->Add(textGT, 0);
	sizerTargets->Add(textET, 0);
	sizerTargets->Add(spinPT, 0, wxEXPAND);
	sizerTargets->Add(spinGT, 0, wxEXPAND);
	sizerTargets->Add(spinET, 0, wxEXPAND);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Algorithm Parameters");
	sizer->Add(sizerP, 0, wxTOP | wxLEFT, 8);
	sizer->AddSpacer(16);
	sizer->Add(sizerSelection, 0, wxLEFT, 8);
	sizer->AddSpacer(16);
	sizer->Add(sizerTargets, 0, wxLEFT, 8);

	SetSizer(sizer);
}
