#include "DialogDimensions.h"
#include "Sizes.h"

#include "wx/wx.h"

DialogDimensions::DialogDimensions(wxWindow* parent) : wxDialog(parent, wxID_ANY, "Change Grid Dimensions")
{
	Centre();

	wxBoxSizer* sizerRows = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* textRows = new wxStaticText(this, wxID_ANY, "Rows:");
	m_SpinRows = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 1, Sizes::MAX_ROWS, Sizes::N_ROWS);
	sizerRows->Add(textRows, 0, wxALIGN_CENTER_VERTICAL);
	sizerRows->Add(m_SpinRows, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 4);

	wxBoxSizer* sizerCols = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* textCols = new wxStaticText(this, wxID_ANY, "Columns:");
	m_SpinCols = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP, 1, Sizes::MAX_COLS, Sizes::N_COLS);
	sizerCols->Add(textCols, 0, wxALIGN_CENTER_VERTICAL);
	sizerCols->Add(m_SpinCols, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 4);

	wxSizer* buttons = CreateButtonSizer(wxOK | wxCANCEL);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(sizerRows, 1, wxEXPAND | wxALL, 8);
	sizer->Add(sizerCols, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
	sizer->Add(buttons, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);

	m_SpinRows->SetFocus();

	SetSizerAndFit(sizer);
}

DialogDimensions::~DialogDimensions()
{
}

int DialogDimensions::GetRows()
{
	return m_SpinRows->GetValue();
}

int DialogDimensions::GetCols()
{
	return m_SpinCols->GetValue();
}
