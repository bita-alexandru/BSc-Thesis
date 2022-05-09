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

	wxStaticText* textIPFM = new wxStaticText(this, wxID_ANY, "Initial Population Fitness Multiplier");
	textIPFM->SetToolTip("0.0 - 1.0");
	wxSpinCtrlDouble* spinIPFM = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinIPFM->SetDigits(1);
	spinIPFM->SetRange(0, 1.0);
	spinIPFM->SetIncrement(0.1);
	spinIPFM->SetValue(1.0);

	sizerP->Add(textPFM, 0);
	sizerP->Add(textGFM, 0);
	sizerP->Add(textIPFM, 0);
	sizerP->Add(spinPFM, 0, wxEXPAND);
	sizerP->Add(spinGFM, 0, wxEXPAND);
	sizerP->Add(spinIPFM, 0, wxEXPAND);

	// SELECTION
	wxStaticText* textSelection = new wxStaticText(this, wxID_ANY, "Selection Method");
	wxComboBox* comboSelection = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, {}, wxCB_READONLY | wxCB_DROPDOWN);
	comboSelection->Set({"Roulette Wheel Selection", "Rank Selection", "Steady State Selection", "Tournament Selection", "Elitism Selection", "Boltzmann Selection"});
	comboSelection->SetValue("Roulette Wheel Selection");

	wxBoxSizer* sizerS = new wxBoxSizer(wxHORIZONTAL);
	sizerS->Add(textSelection, 0, wxALIGN_CENTER_VERTICAL);
	sizerS->Add(comboSelection, 0, wxEXPAND | wxLEFT, 8);

	// PATTERN TARGETS
	wxStaticText* textMPT = new wxStaticText(this, wxID_ANY, "Minimum Population Target");
	textMPT->SetToolTip("0 - 1000");
	wxSpinCtrl* spinMPT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinMPT->SetRange(0, 1000);
	spinMPT->SetValue(0);

	wxStaticText* textMGT = new wxStaticText(this, wxID_ANY, "Minimum Generation Target");
	textMGT->SetToolTip("0 - 1000");
	wxSpinCtrl* spinMGT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinMGT->SetRange(0, 1000);
	spinMGT->SetValue(0);

	wxStaticText* textMIPT = new wxStaticText(this, wxID_ANY, "Maximum Initial Population Target");
	textMPT->SetToolTip("0 - 1000");
	wxSpinCtrl* spinMIPT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinMIPT->SetRange(0, 1000);
	spinMIPT->SetValue(0);

	wxGridSizer* sizerPT = new wxGridSizer(2, 3, 0, 6);
	sizerPT->Add(textMPT, 0);
	sizerPT->Add(textMGT, 0);
	sizerPT->Add(textMIPT, 0);
	sizerPT->Add(spinMPT, 0, wxEXPAND);
	sizerPT->Add(spinMGT, 0, wxEXPAND);
	sizerPT->Add(spinMIPT, 0, wxEXPAND);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Algorithm Parameters");
	sizer->Add(sizerP, 0, wxTOP | wxLEFT, 8);
	sizer->AddSpacer(16);
	sizer->Add(sizerS, 0, wxLEFT, 8);
	sizer->AddSpacer(16);
	sizer->Add(sizerPT, 0, wxLEFT, 8);

	SetSizer(sizer);
}
