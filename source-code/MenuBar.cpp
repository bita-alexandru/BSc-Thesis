#include "MenuBar.h"

MenuBar::MenuBar(): wxMenuBar()
{
	BuildInterface();

	SetFunctions();
}

MenuBar::~MenuBar()
{
}

void MenuBar::BuildInterface()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuOpen = new wxMenu();
	wxMenu* menuSave = new wxMenu();
	wxMenu* menuClear = new wxMenu();
	wxMenu* menuView = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuOpen->Append(Ids::ID_OPEN_C, "&Cellular Automaton Configuration\tCtrl-O");
	menuOpen->Append(Ids::ID_OPEN_G, "&Genetic Algorithm Configuration\tCtrl-Shift-O");

	menuSave->Append(Ids::ID_SAVE_C, "&Cellular Automaton Configuration\tCtrl-S");
	menuSave->Append(Ids::ID_SAVE_G, "&Genetic Algorithm Configuration\tCtrl-Shift-S");

	menuFile->AppendSubMenu(menuOpen, "&Open");
	menuFile->AppendSubMenu(menuSave, "&Save");
	menuFile->AppendSeparator();
	menuFile->AppendSubMenu(menuClear, "&Clear");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_EXIT, "E&xit\tAlt-F4");

	menuClear->Append(Ids::ID_CLEAR_C, "&Cellular Automaton Input\tCtrl-L");
	menuClear->Append(Ids::ID_CLEAR_G, "&Genetic Algorithm Input\tCtrl-Shift-L");

	menuView->Append(Ids::ID_VIEW_DEFAULT, "&Default Perspective\tF1");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_VIEW_INPUT, "&Input Perspective\tF2");
	menuView->Append(Ids::ID_VIEW_GRID, "&Grid Perspective\tF3");
	menuView->Append(Ids::ID_VIEW_ALGORITHM, "&Algorithm Perspective\tF4");

	menuHelp->Append(Ids::ID_USERMANUAL, "&User Manual\tCtrl-U");
	menuHelp->Append(Ids::ID_SHORTCUTS, "&Shortcuts\tCtrl-J");

	Append(menuFile, "&File");
	Append(menuView, "&View");
	Append(menuHelp, "&Help");
}

void MenuBar::SetFunctions()
{
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MenuBar::OnExit, this, Ids::ID_EXIT);
}

void MenuBar::OnExit(wxCommandEvent& evt)
{
	GetParent()->Close();
}
