#include "MenuBar.h"

MenuBar::MenuBar(): wxMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuOpen = new wxMenu();
	wxMenu* menuSave = new wxMenu();
	wxMenu* menuReset = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuOpen->Append(ID_OPEN_C, "&Cellular automaton configuration\tCtrl-O");
	menuOpen->Append(ID_OPEN_G, "&Genetic algorithm configuration\tCtrl-Shift-O");

	menuSave->Append(ID_SAVE_C, "&Cellular automaton configuration\tCtrl-S");
	menuSave->Append(ID_SAVE_G, "&Genetic algorithm configuration\tCtrl-Shift-S");

	menuFile->AppendSubMenu(menuOpen, "&Open");
	menuFile->AppendSubMenu(menuSave, "&Save");
	menuFile->AppendSeparator();
	menuFile->Append(ID_EXIT, "E&xit");

	menuReset->Append(ID_RESET_C, "Reset &cellular automaton\tCtrl-R");
	menuReset->Append(ID_RESET_G, "Reset &genetic algorithm\tCtrl-Shift-R");

	menuHelp->Append(ID_DOCUMENTATION, "&Documentation\tCtrl-D");

	this->Append(menuFile, "&File");
	this->Append(menuReset, "&Reset");
	this->Append(menuHelp, "&Help");
}

MenuBar::~MenuBar()
{
}

