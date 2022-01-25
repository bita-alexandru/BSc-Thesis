#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_CLOSE(Main::OnClose)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "CellyGen", wxDefaultPosition, wxSize(Sizes::MAIN_WIDTH, Sizes::MAIN_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	SetClientSize(Sizes::MAIN_WIDTH, Sizes::MAIN_HEIGHT);

	Center();

    SetBackgroundColour(wxColour(255, 255, 255));

	BuildMenubar();

	BuildInterface();

	SetShortcuts();

	PrepareInput();
}

Main::~Main()
{
}

void Main::BuildInterface()
{
	m_SplitterInputGrid = new wxSplitterWindow(this, wxID_ANY);
	m_PanelInput = new PanelInput(m_SplitterInputGrid);

	m_SplitterGridAlgorithm = new wxSplitterWindow(m_SplitterInputGrid, wxID_ANY);
	m_PanelGrid = new PanelGrid(m_SplitterGridAlgorithm);
	m_PanelAlgorithm = new PanelAlgorithm(m_SplitterGridAlgorithm);

	wxBoxSizer* sizerGridAlgorithm = new wxBoxSizer(wxVERTICAL);
	sizerGridAlgorithm->Add(m_PanelGrid, 0);
	sizerGridAlgorithm->Add(m_PanelAlgorithm, 0);

	m_SplitterGridAlgorithm->SetSizer(sizerGridAlgorithm);
	m_SplitterGridAlgorithm->SplitHorizontally(m_PanelGrid, m_PanelAlgorithm);
	m_SplitterGridAlgorithm->SetMinimumPaneSize(1);
	m_SplitterGridAlgorithm->SetSashGravity(0.75);

	m_SplitterInputGrid->SplitVertically(m_PanelInput, m_SplitterGridAlgorithm);
	m_SplitterInputGrid->SetMinimumPaneSize(1);
	m_SplitterInputGrid->SetSashGravity(0.2);

	m_SplitterInputGrid->SetSashPosition(GetClientSize().GetX() * 0.2);
	m_SplitterGridAlgorithm->SetSashPosition(GetClientSize().GetY() * 0.75);

	m_EditorStates = new EditorStates(this);
	m_EditorRules = new EditorRules(this);
}

void Main::BuildMenubar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuView = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuFile->Append(Ids::ID_IMPORT, "&Import\tCtrl+O");
	menuFile->Append(Ids::ID_EXPORT, "&Export\tCtrl+S");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_EXIT, "E&xit\tAlt+F4");

	menuView->Append(Ids::ID_VIEW_DEFAULT, "&Default Perspective\tF1");
	menuView->Append(Ids::ID_VIEW_GRID, "&Grid Perspective\tF2");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_VIEW_STATES, "&States Editor\tCtrl+1");
	menuView->Append(Ids::ID_VIEW_RULES, "&Rules Editor\tCtrl+2");

	menuHelp->Append(Ids::ID_USERMANUAL, "&User Manual\tCtrl+U");
	menuHelp->Append(Ids::ID_SHORTCUTS, "&Shortcuts\tCtrl+J");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuView, "&View");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuExit, this, Ids::ID_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuPerspectiveDefault, this, Ids::ID_VIEW_DEFAULT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuPerspectiveGrid, this, Ids::ID_VIEW_GRID);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuEditorStates, this, Ids::ID_VIEW_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuEditorRules, this, Ids::ID_VIEW_RULES);
}

void Main::SetShortcuts()
{
	wxAcceleratorEntry entries[16];
	// ToolZoom
	entries[0].Set(wxACCEL_CTRL, (int)'+', Ids::ID_ZOOM_IN);
	entries[1].Set(wxACCEL_CTRL, (int)'-', Ids::ID_ZOOM_OUT);
	// ToolUndo
	entries[2].Set(wxACCEL_CTRL, (int)'Z', Ids::ID_BUTTON_UNDO);
	entries[3].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'Z', Ids::ID_BUTTON_REDO);
	// ToolModes
	entries[4].Set(wxACCEL_CTRL, (int)'Q', Ids::ID_MODE_DRAW);
	entries[5].Set(wxACCEL_CTRL, (int)'W', Ids::ID_MODE_PICK);
	entries[6].Set(wxACCEL_CTRL, (int)'E', Ids::ID_MODE_MOVE);
	// ToolStates
	entries[7].Set(wxACCEL_CTRL, WXK_LEFT, Ids::ID_BUTTON_PREV);
	entries[8].Set(wxACCEL_CTRL, WXK_RIGHT, Ids::ID_BUTTON_NEXT);
	// Editors and their Searchbars
	//entries[9].Set(wxACCEL_CTRL, (int)'1', Ids::ID_EDIT_STATES);
	//entries[10].Set(wxACCEL_CTRL, (int)'2', Ids::ID_EDIT_RULES);
	entries[9].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'1', Ids::ID_SEARCH_STATES);
	entries[10].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int)'2', Ids::ID_SEARCH_RULES);
	// GridStatus
	entries[11].Set(wxACCEL_CTRL, (int)'M', Ids::ID_BUTTON_CENTER);
	entries[12].Set(wxACCEL_CTRL, (int)'R', Ids::ID_BUTTON_RESET);
	entries[13].Set(wxACCEL_CTRL, (int)'G', Ids::ID_BUTTON_GENERATION);

	wxAcceleratorTable accel(16, entries);
	this->SetAcceleratorTable(accel);
}

void Main::PrepareInput()
{
	InputStates* inputStates = m_PanelInput->GetInputStates();
	InputNeighbors* inputNeighbors = m_PanelInput->GetInputNeighbors();
	InputRules* inputRules = m_PanelInput->GetInputRules();

	GridTools* gridTools = m_PanelGrid->GetGridTools();
	Grid* grid = m_PanelGrid->GetGrid();
	GridStatus* gridStatus = m_PanelGrid->GetGridStatus();

	ToolZoom* toolZoom = gridTools->GetToolZoom();
	ToolUndo* toolUndo = gridTools->GetToolUndo();
	ToolModes* toolModes = gridTools->GetToolModes();
	ToolStates* toolStates = gridTools->GetToolStates();
	ToolCoords* toolCoords = gridTools->GetToolCoords();

	StatusSpeed* statusSpeed = gridStatus->GetStatusSpeed();
	StatusControls* statusControls = gridStatus->GetStatusControls();
	StatusCells* statusCells = gridStatus->GetStatusCells();

	grid->SetFocus();

	m_EditorStates->SetInputStates(inputStates);
	m_EditorRules->SetInputRules(inputRules);

	inputStates->SetToolStates(toolStates);
	inputStates->SetGrid(grid);
	inputStates->SetEditorStates(m_EditorStates);
	inputStates->SetInputRules(inputRules);

	inputRules->SetEditorRules(m_EditorRules);
	inputRules->SetInputStates(inputStates);
	inputRules->SetInputNeighbors(inputNeighbors);

	toolZoom->SetGrid(grid);
	toolUndo->SetGrid(grid);
	toolStates->SetListStates(inputStates->GetList());
	toolStates->SetGrid(grid);
	toolModes->SetGrid(grid);

	grid->SetToolZoom(toolZoom);
	grid->SetToolUndo(toolUndo);
	grid->SetToolModes(toolModes);
	grid->SetToolStates(toolStates);
	grid->SetToolCoords(toolCoords);
	grid->SetInputRules(inputRules);

	grid->SetStatusCells(statusCells);

	statusControls->SetGrid(grid);

	inputNeighbors->SetNeighbors({ "NW","N","NE","W","C","E","SW","S","SE"});
	inputStates->SetStates({ "FREE","LIVE" });
}

void Main::MenuExit(wxCommandEvent& evt)
{
	Close();
}

void Main::MenuPerspectiveDefault(wxCommandEvent& evt)
{
	m_SplitterGridAlgorithm->SetSashPosition(GetClientSize().GetY() * 0.75);
	m_SplitterInputGrid->SetSashPosition(GetClientSize().GetX() * 0.2);
}

void Main::MenuPerspectiveInput(wxCommandEvent& evt)
{
	m_SplitterInputGrid->SetSashPosition(9999);
}

void Main::MenuPerspectiveGrid(wxCommandEvent& evt)
{
	m_SplitterGridAlgorithm->SetSashPosition(9999);
	m_SplitterInputGrid->SetSashPosition(-9999);
}

void Main::MenuPerspectiveAlgorithm(wxCommandEvent& evt)
{
	m_SplitterGridAlgorithm->SetSashPosition(-9999);
	m_SplitterInputGrid->SetSashPosition(-9999);
}

void Main::MenuEditorStates(wxCommandEvent& evt)
{
	m_EditorStates->Show();
	m_EditorStates->SetFocus();
}

void Main::MenuEditorRules(wxCommandEvent& evt)
{
	m_EditorRules->Show();
	m_EditorRules->SetFocus();
}

void Main::OnClose(wxCloseEvent& evt)
{
	m_EditorStates->ForceClose();
	m_EditorRules->ForceClose();

	evt.Skip();
}