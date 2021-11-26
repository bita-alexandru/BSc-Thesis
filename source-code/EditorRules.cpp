#include "EditorRules.h"

EditorRules::EditorRules(wxFrame* parent) : wxFrame(parent, wxID_ANY, "CellyGen::Rules", wxDefaultPosition, wxSize(Sizes::EDITOR_WIDTH, Sizes::EDITOR_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	Center();

	SetBackgroundColour(wxColour(255, 232, 214));

	BuildMenuBar();

	BuildInputPanel();

	LoadData();
}

EditorRules::~EditorRules()
{
}

std::vector<std::string> EditorRules::GetData()
{
	return std::vector<std::string>();
}

void EditorRules::BuildMenuBar()
{
	wxMenu* menu = new wxMenu();

	menu->Append(Ids::ID_FIND, "&Find\tCtrl-F");
	menu->Append(Ids::ID_REPLACE, "&Replace\tCtrl-R");
	menu->AppendSeparator();
	menu->Append(Ids::ID_EXIT, "E&xit");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menu, "&File");

	this->SetMenuBar(menuBar);
}

void EditorRules::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 Rules lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	m_TextCtrl = new wxStyledTextCtrl(this);
	m_TextCtrl->SetMarginWidth(wxSTC_MARGIN_NUMBER, 32);
	m_TextCtrl->SetMarginType(wxSTC_MARGINOPTION_SUBLINESELECT, wxSTC_MARGIN_NUMBER);
	m_TextCtrl->SetScrollWidth(1);

	wxFont font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
	m_TextCtrl->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	wxButton* close = new wxButton(this, Ids::ID_SAVE_RULES, wxString("Close"));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0, wxALL, 6);
	sizer->Add(m_TextCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(close, 0, wxALIGN_RIGHT | wxALL, 6);

	this->SetSizer(sizer);
}

void EditorRules::LoadData()
{
}
