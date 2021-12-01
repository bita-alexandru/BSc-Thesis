#include "MenuBar.h"

MenuBar::MenuBar(): wxMenuBar()
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
	menuFile->Append(Ids::ID_EXIT, "E&xit");

	menuClear->Append(Ids::ID_CLEAR_C, "&Cellular Automaton Input\tCtrl-T");
	menuClear->Append(Ids::ID_CLEAR_G, "&Genetic Algorithm Input\tCtrl-Shift-T");

	menuView->Append(Ids::ID_VIEW_DEFAULT, "&Default Perspective\tCtrl-1");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_VIEW_INPUT, "&Input Perspective\tCtrl-2");
	menuView->Append(Ids::ID_VIEW_GRID, "&Grid Perspective\tCtrl-3");
	menuView->Append(Ids::ID_VIEW_PARAMETERS, "&Parameters Perspective\tCtrl-4");

	menuHelp->Append(Ids::ID_USERMANUAL, "&User Manual\tCtrl-U");
	menuHelp->Append(Ids::ID_SHORTCUTS, "&Shortcuts\tCtrl-J");

	this->Append(menuFile, "&File");
	this->Append(menuView, "&View");
	this->Append(menuHelp, "&Help");
}

MenuBar::~MenuBar()
{
}

