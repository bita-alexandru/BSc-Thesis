#include "MenuBar.h"

MenuBar::MenuBar(): wxMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuOpen = new wxMenu();
	wxMenu* menuSave = new wxMenu();
	wxMenu* menuReset = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuOpen->Append(Ids::ID_OPEN_C, "&Cellular Automaton Configuration\tCtrl-O");
	menuOpen->Append(Ids::ID_OPEN_G, "&Genetic Algorithm Configuration\tCtrl-Shift-O");

	menuSave->Append(Ids::ID_SAVE_C, "&Cellular Automaton Configuration\tCtrl-S");
	menuSave->Append(Ids::ID_SAVE_G, "&Genetic Algorithm Configuration\tCtrl-Shift-S");

	menuFile->AppendSubMenu(menuOpen, "&Open");
	menuFile->AppendSubMenu(menuSave, "&Save");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_EXIT, "E&xit");

	menuReset->Append(Ids::ID_RESET_C, "Reset &Cellular Automaton\tCtrl-R");
	menuReset->Append(Ids::ID_RESET_G, "Reset &Genetic Algorithm\tCtrl-Shift-R");

	menuHelp->Append(Ids::ID_DOCUMENTATION, "&Documentation\tCtrl-D");

	this->Append(menuFile, "&File");
	this->Append(menuReset, "&Reset");
	this->Append(menuHelp, "&Help");
}

MenuBar::~MenuBar()
{
}

