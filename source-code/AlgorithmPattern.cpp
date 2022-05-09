#include "AlgorithmPattern.h"

#include "wx/spinctrl.h"

AlgorithmPattern::AlgorithmPattern(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

AlgorithmPattern::~AlgorithmPattern()
{
}

void AlgorithmPattern::BuildInterface()
{
	// PATTERN TARGETS
	wxStaticText* textMPT = new wxStaticText(this, wxID_ANY, "MPT");
	textMPT->SetToolTip("Minimum Population Target");
	wxSpinCtrl* spinMPT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinMPT->SetRange(0, 1000000);
	spinMPT->SetValue(0);

	wxStaticText* textMGT = new wxStaticText(this, wxID_ANY, "MGT");
	textMGT->SetToolTip("Minimum Generation Target");
	wxSpinCtrl* spinMGT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinMGT->SetRange(0, 1000000);
	spinMGT->SetValue(0);

	wxStaticText* textMIPT = new wxStaticText(this, wxID_ANY, "MPT");
	textMPT->SetToolTip("Maximum Initial Population Target");
	wxSpinCtrl* spinMIPT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinMIPT->SetRange(0, 1000000);
	spinMIPT->SetValue(0);

	wxGridSizer* sizerPT = new wxGridSizer(2, 3, 0, 0);
	sizerPT->Add(textMPT, 0);
	sizerPT->Add(textMGT, 0);
	sizerPT->Add(textMIPT, 0);
	sizerPT->Add(spinMPT, 0, wxEXPAND);
	sizerPT->Add(spinMGT, 0, wxEXPAND);
	sizerPT->Add(spinMIPT, 0, wxEXPAND);

	// RULES TARGETS
	wxStaticText* textNRT = new wxStaticText(this, wxID_ANY, "NRT");
	textNRT->SetToolTip("Number of Rules Target");
	wxSpinCtrl* spinNRT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinNRT->SetRange(1, 100);
	spinNRT->SetValue(1);
	wxStaticText* textNCT = new wxStaticText(this, wxID_ANY, "MPT");
	textNCT->SetToolTip("Number of Conditions Target");
	wxSpinCtrl* spinNCT = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP | wxSP_ARROW_KEYS);
	spinNCT->SetRange(0, 10);
	spinNCT->SetValue(0);

	wxGridSizer* sizerRT = new wxGridSizer(2, 2, 0, 0);
	sizerRT->Add(textNRT, 0);
	sizerRT->Add(textNCT, 0);
	sizerRT->Add(spinNRT, 0, wxEXPAND);
	sizerRT->Add(spinNCT, 0, wxEXPAND);

	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Exploration Parameters");
	sizer->Add(sizerPT, 0, wxTOP | wxLEFT, 8);
	sizer->Add(sizerRT, 0, wxTOP | wxLEFT, 8);

	SetSizer(sizer);
}
