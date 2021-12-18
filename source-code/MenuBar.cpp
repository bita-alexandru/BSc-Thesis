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
	wxMenu* menuEdit = new wxMenu();
	wxMenu* menuView = new wxMenu();
	wxMenu* menuControl = new wxMenu();
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

	menuEdit->Append(Ids::ID_BUTTON_UNDO, "&Undo\tCtrl-Z");
	menuEdit->Append(Ids::ID_BUTTON_REDO, "&Redo\tCtrl-Shift-Z");
	menuEdit->AppendSeparator();
	menuEdit->Append(Ids::ID_BUTTON_NEXT, "Ne&xt State\tCtrl-Right");
	menuEdit->Append(Ids::ID_BUTTON_PREV, "Pre&vious State\tCtrl-Left");
	menuEdit->AppendSeparator();
	menuEdit->AppendCheckItem(Ids::ID_MODE_DRAW, "&Draw\tCtrl-Q");
	menuEdit->AppendCheckItem(Ids::ID_MODE_PICK, "&Pick\tCtrl-W");
	menuEdit->AppendCheckItem(Ids::ID_MODE_MOVE, "&Move\tCtrl-E");

	menuView->Append(Ids::ID_ZOOM_IN, "Zoom I&n\t+");
	menuView->Append(Ids::ID_ZOOM_OUT, "Zoom O&ut\t-");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_BUTTON_CENTER, "Bring To &Center\tCtrl-M");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_EDIT_STATES, "Edit &States\tCtrl-1");
	menuView->Append(Ids::ID_EDIT_RULES, "Edit &Rules\tCtrl-2");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_SEARCH_STATES, "Search Sta&tes\tCtrl-Shift-1");
	menuView->Append(Ids::ID_SEARCH_RULES, "Search Ru&les\tCtrl-Shift-2");
	menuView->AppendSeparator();
	menuView->AppendCheckItem(Ids::ID_VIEW_DEFAULT, "&Default Perspective\tF1");
	menuView->AppendCheckItem(Ids::ID_VIEW_INPUT, "&Input Perspective\tF2");
	menuView->AppendCheckItem(Ids::ID_VIEW_GRID, "&Grid Perspective\tF3");
	menuView->AppendCheckItem(Ids::ID_VIEW_ALGORITHM, "&Algorithm Perspective\tF4");

	menuControl->Append(Ids::ID_BUTTON_START, "&Start\tCtrl-Space");
	menuControl->Append(Ids::ID_BUTTON_RESET, "&Reset\tCtrl-R");
	menuControl->Append(Ids::ID_BUTTON_STEP, "Next Ste&p\t,");
	menuControl->Append(Ids::ID_BUTTON_GENERATION, "Next &Generation\t.");
	menuControl->AppendSeparator();
	menuControl->Append(Ids::ID_BUTTON_FASTER, "Speed &Up\t>");
	menuControl->Append(Ids::ID_BUTTON_SLOWER, "Speed &Down\t<");

	menuHelp->Append(Ids::ID_USERMANUAL, "&User Manual\tCtrl-U");
	menuHelp->Append(Ids::ID_SHORTCUTS, "&Shortcuts\tCtrl-J");

	Append(menuFile, "&File");
	Append(menuEdit, "&Edit");
	Append(menuView, "&View");
	Append(menuControl, "&Control");
	Append(menuHelp, "&Help");

	Check(Ids::ID_VIEW_DEFAULT, true);
	Check(Ids::ID_MODE_DRAW, true);

	Enable(Ids::ID_BUTTON_UNDO, false);
	Enable(Ids::ID_BUTTON_REDO, false);
	Enable(Ids::ID_BUTTON_START, false);
	Enable(Ids::ID_BUTTON_RESET, false);
	Enable(Ids::ID_BUTTON_STEP, false);
	Enable(Ids::ID_BUTTON_GENERATION, false);
}

void MenuBar::SetFunctions()
{
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MenuBar::OnExit, this, Ids::ID_EXIT);
}

void MenuBar::OnExit(wxCommandEvent& evt)
{
	GetParent()->Close();
}
