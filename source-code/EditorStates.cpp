#include "EditorStates.h"

EditorStates::EditorStates(wxFrame* parent) : wxFrame(parent, wxID_ANY, "CellyGen::States", wxDefaultPosition, wxSize(Constants::EDITOR_WIDTH, Constants::EDITOR_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	Center();

	SetBackgroundColour(wxColour(255, 255, 255));

	BuildMenuBar();

	BuildInputPanel();

	LoadData();
}

EditorStates::~EditorStates()
{
}

void EditorStates::BuildMenuBar()
{
	wxMenu* menu = new wxMenu();

	menu->Append(Constants::ID_FIND, "&Find\tCtrl-F");
	menu->Append(Constants::ID_REPLACE, "&Replace\tCtrl-R");
	menu->AppendSeparator();
	menu->Append(Constants::ID_EXIT, "E&xit");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menu, "&File");

	this->SetMenuBar(menuBar);
}

void EditorStates::BuildInputPanel()
{
	std::string labelHelp = "so basically yeah, plz no more than 256 states lol ok ? thx.";
	wxStaticText* help = new wxStaticText(this, wxID_ANY, labelHelp);

	wxStyledTextCtrl* states = new wxStyledTextCtrl(this);
	states->SetMarginWidth(1, 32);
	states->SetMarginType(1, wxSTC_MARGIN_NUMBER);

	wxButton* close = new wxButton(this, Constants::ID_SAVE_STATES, wxString("Close"));

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(help, 0);
	sizer->Add(states, 1, wxEXPAND);
	sizer->Add(close, 0, wxALIGN_RIGHT);

	this->SetSizer(sizer);
}

void EditorStates::LoadData()
{
}
