#include "Main.h"
#include "DialogDimensions.h"

#include <fstream>
#include <sstream>

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_CLOSE(Main::OnClose)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "CellyGen", wxDefaultPosition, wxSize(Sizes::MAIN_WIDTH, Sizes::MAIN_HEIGHT))
{
	SetIcon(wxICON(aaaIcon));

	SetClientSize(Sizes::MAIN_WIDTH, Sizes::MAIN_HEIGHT);

	Center();

    //SetBackgroundColour(wxColour(255, 255, 255));

	BuildMenuBar();

	BuildInterface();

	SetShortcuts();

	PrepareInput();
}

Main::~Main()
{
}

void Main::BuildInterface()
{
	Sizes::N_COLS = 51; Sizes::N_ROWS = 51;

	m_SplitterInputGrid = new wxSplitterWindow(this, wxID_ANY);
	m_PanelInput = new PanelInput(m_SplitterInputGrid);

	//m_SplitterGridAlgorithm = new wxSplitterWindow(m_SplitterInputGrid, wxID_ANY);
	//m_PanelGrid = new PanelGrid(m_SplitterGridAlgorithm);
	m_PanelGrid = new PanelGrid(m_SplitterInputGrid);
	//m_PanelAlgorithm = new PanelAlgorithm(m_SplitterGridAlgorithm);

	//wxBoxSizer* sizerGridAlgorithm = new wxBoxSizer(wxVERTICAL);
	//sizerGridAlgorithm->Add(m_PanelGrid, 0);
	//sizerGridAlgorithm->Add(m_PanelAlgorithm, 0);

	//m_SplitterGridAlgorithm->SetSizer(sizerGridAlgorithm);
	//m_SplitterGridAlgorithm->SplitHorizontally(m_PanelGrid, m_PanelAlgorithm);
	//m_SplitterGridAlgorithm->SetMinimumPaneSize(1);
	//m_SplitterGridAlgorithm->SetSashGravity(0.75);

	//m_SplitterInputGrid->SplitVertically(m_PanelInput, m_SplitterGridAlgorithm);
	m_SplitterInputGrid->SplitVertically(m_PanelInput, m_PanelGrid);
	m_SplitterInputGrid->SetMinimumPaneSize(1);
	m_SplitterInputGrid->SetSashGravity(0.2);

	m_SplitterInputGrid->SetSashPosition(GetClientSize().GetX() * 0.2);
	//m_SplitterGridAlgorithm->SetSashPosition(GetClientSize().GetY() * 0.75);

	m_EditorStates = new EditorStates(this);
	m_EditorRules = new EditorRules(this);
	m_HelpWindow = new HelpWindow(this);
}

void Main::BuildMenuBar()
{
	wxMenu* menuFile = new wxMenu();
	wxMenu* menuView = new wxMenu();
	wxMenu* menuHelp = new wxMenu();

	menuFile->Append(Ids::ID_IMPORT, "&Import\tCtrl+O");
	menuFile->Append(Ids::ID_EXPORT, "&Export\tCtrl+S");
	menuFile->AppendSeparator();
	menuFile->Append(Ids::ID_EXIT, "E&xit\tAlt+F4");

	menuView->Append(Ids::ID_VIEW_DIMENSIONS, "&Change Dimensions\tCtrl+Enter");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_VIEW_STATES, "&States Editor\tCtrl+1");
	menuView->Append(Ids::ID_VIEW_RULES, "&Rules Editor\tCtrl+2");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_VIEW_DEFAULT, "&Default Perspective\tShift+1");
	menuView->Append(Ids::ID_VIEW_GRID, "&Grid Perspective\tShift+2");
	menuView->AppendSeparator();
	menuView->Append(Ids::ID_VIEW_FULLSCREEN, "&Full Screen\tF11");

	menuHelp->Append(Ids::ID_USERMANUAL, "&User Manual\tF1");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuView, "&View");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuImport, this, Ids::ID_IMPORT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuExport, this, Ids::ID_EXPORT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuExit, this, Ids::ID_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuChangeDimensions, this, Ids::ID_VIEW_DIMENSIONS);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuEditorStates, this, Ids::ID_VIEW_STATES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuEditorRules, this, Ids::ID_VIEW_RULES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuPerspectiveDefault, this, Ids::ID_VIEW_DEFAULT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuPerspectiveGrid, this, Ids::ID_VIEW_GRID);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuFullScreen, this, Ids::ID_VIEW_FULLSCREEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &Main::MenuHelp, this, Ids::ID_USERMANUAL);
}

void Main::SetShortcuts()
{
	wxAcceleratorEntry entries[20];
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
	entries[13].Set(wxACCEL_CTRL, (int)' ', Ids::ID_BUTTON_PLAY);
	entries[14].Set(wxACCEL_CTRL, (int)'G', Ids::ID_BUTTON_GENERATION);
	entries[15].Set(wxACCEL_CTRL, (int)',', Ids::ID_BUTTON_SLOWER);
	entries[16].Set(wxACCEL_CTRL, (int)'.', Ids::ID_BUTTON_FASTER);

	wxAcceleratorTable accel(20, entries);
	SetAcceleratorTable(accel);
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

	StatusDelay* statusDelay = gridStatus->GetStatusDelay();
	StatusControls* statusControls = gridStatus->GetStatusControls();
	StatusCells* statusCells = gridStatus->GetStatusCells();

	grid->SetFocus();

	m_EditorStates->SetInputStates(inputStates);
	m_EditorStates->SetHelpWindow(m_HelpWindow);
	m_EditorRules->SetInputRules(inputRules);
	m_EditorRules->SetHelpWindow(m_HelpWindow);

	inputStates->SetToolStates(toolStates);
	inputStates->SetGrid(grid);
	inputStates->SetEditorStates(m_EditorStates);
	inputStates->SetInputRules(inputRules);

	inputRules->SetEditorRules(m_EditorRules);
	inputRules->SetInputStates(inputStates);
	inputRules->SetInputNeighbors(inputNeighbors);

	inputNeighbors->SetGrid(grid);

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
	grid->SetStatusControls(statusControls);
	grid->SetStatusDelay(statusDelay);

	statusControls->SetGrid(grid);
	statusDelay->SetGrid(grid);

	//inputNeighbors->SetNeighbors({ "NW","N","NE","W","C","E","SW","S","SE"});
	//inputStates->SetStates({ "FREE","LIVE" });
}

void Main::MenuExit(wxCommandEvent& evt)
{
	Close();
}

void Main::MenuPerspectiveDefault(wxCommandEvent& evt)
{
	//m_SplitterGridAlgorithm->SetSashPosition(GetClientSize().GetY() * 0.75);
	m_SplitterInputGrid->SetSashPosition(GetClientSize().GetX() * 0.2);
}

void Main::MenuPerspectiveInput(wxCommandEvent& evt)
{
	m_SplitterInputGrid->SetSashPosition(9999);
}

void Main::MenuPerspectiveGrid(wxCommandEvent& evt)
{
	//m_SplitterGridAlgorithm->SetSashPosition(9999);
	m_SplitterInputGrid->SetSashPosition(-9999);
}

void Main::MenuPerspectiveAlgorithm(wxCommandEvent& evt)
{
	//m_SplitterGridAlgorithm->SetSashPosition(-9999);
	//m_SplitterInputGrid->SetSashPosition(-9999);
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

void Main::MenuChangeDimensions(wxCommandEvent& evt)
{
	DialogDimensions dialog(this);

	if (dialog.ShowModal() == wxID_OK)
	{
		int rows = dialog.GetRows();
		int cols = dialog.GetCols();

		m_PanelGrid->GetGrid()->SetDimensions(rows, cols);
	}
}

void Main::MenuFullScreen(wxCommandEvent& evt)
{
	if (IsFullScreen()) ShowFullScreen(false);
	else ShowFullScreen(true);
}

void Main::MenuImport(wxCommandEvent& evt)
{
	if (m_PanelGrid->GetGrid()->GetGenerating() || !m_PanelGrid->GetGrid()->GetPaused())
	{
		wxMessageBox("Can't import while the simulation is playing. Try pausing it first.", "Error", wxICON_WARNING);
		return;
	}

	wxFileDialog dialogFile(this, "Import States", "", "", "TXT files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (dialogFile.ShowModal() == wxID_CANCEL) return;

	std::ifstream in(dialogFile.GetPath().ToStdString());
	std::stringstream ss; ss << in.rdbuf();

	// read everything and ignore until line mark "[STATES/RULES/SIZE/CELLS]"
	std::unordered_set<wxString> marks = {"[STATES]","[RULES]","[NEIGHBORS]","[SIZE]","[CELLS]"};
	std::unordered_map<wxString, std::string> mark;
	while (true)
	{
		std::string s;

		if (!std::getline(ss, s, '\n')) break;

		wxString symbol = wxString(s).Upper();

		// invalid mark
		if (marks.find(symbol) == marks.end()) continue;
		// duplicate found -> ignore
		if (mark.find(symbol) != mark.end()) continue;

		mark.insert({ symbol, "" });

		while (true)
		{
			s = "";

			std::streampos pos = ss.tellg();
			if (!std::getline(ss, s, '\n')) break;

			// detected line mark -> reset stream cursor position
			if (marks.find(s) != marks.end())
			{
				ss.seekg(pos);
				break;
			}

			mark[symbol] += s + "\n";
		}
	}

	if (mark.empty())
	{
		wxMessageBox("Invalid import file. No line markers found.", "Error", wxICON_ERROR | wxOK);
		return;
	}

	bool hasErrors = false;
	
	auto it = mark.find("[STATES]");
	if (it != mark.end())
	{
		std::pair<std::vector<std::string>, std::vector<std::pair<int, std::string>>> data = m_EditorStates->Process(it->second);
		std::vector<std::string> states = data.first;
		std::vector<std::pair<int, std::string>> errors = data.second;

		if (states.size())
		{
			m_EditorStates->SetText(it->second);
			m_PanelInput->GetInputStates()->SetStates(states);
		}

		if (errors.size()) hasErrors = true;
	}

	it = mark.find("[RULES]");
	if (it != mark.end())
	{
		std::pair<std::vector<std::pair<std::string, Transition>>, std::vector<std::pair<int, std::string>>> data = m_EditorRules->Process(it->second);
		std::vector<std::pair<std::string, Transition>> rules = data.first;
		std::vector<std::pair<int, std::string>> errors = data.second;

		if (rules.size())
		{
			m_EditorRules->SetText(it->second);
			m_PanelInput->GetInputRules()->SetRules(rules);
		}

		if (errors.size()) hasErrors = true;
	}

	it = mark.find("[NEIGHBORS]");
	if (it != mark.end())
	{
		ss.clear();
		ss << it->second;

		std::string direction;
		std::vector<std::string> directions;
		std::unordered_set<std::string> neighborhood = { "NW","N","NE","W","C","E","SW","S","SE" };

		while (ss >> direction)
		{
			direction = wxString(direction).Upper().ToStdString();

			if (neighborhood.find(direction) == neighborhood.end())
			{
				hasErrors = true;
			}
			else
			{
				if (std::find(directions.begin(), directions.end(), direction) == directions.end()) directions.push_back(direction);
			}
		}

		if (directions.size()) m_PanelInput->GetInputNeighbors()->SetNeighbors(directions);
	}

	it = mark.find("[SIZE]");
	if (it != mark.end())
	{
		ss.clear();
		ss << it->second;

		std::string rows;
		std::string cols;
		std::string s;
		ss >> rows >> cols >> s;

		if (rows.empty() || cols.empty() || s.size())
		{
			hasErrors = true;
		}
		else
		{
			// check if number is valid
			if (rows.find_first_not_of("0123456789") != string::npos || cols.find_first_not_of("0123456789") != string::npos)
			{
				hasErrors = true;
			}
			else
			{
				int nrows = std::stoi(rows);
				int ncols = std::stoi(cols);

				if (nrows < 1 || nrows > Sizes::MAX_ROWS || ncols < 1 || ncols > Sizes::MAX_COLS)
				{
					hasErrors = true;
				}
				else
				{
					m_PanelGrid->GetGrid()->SetDimensions(nrows, ncols);
				}
			}
		}
	}

	it = mark.find("[CELLS]");
	std::vector < std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>> cells;
	if (it != mark.end())
	{
		ss.clear();
		ss << it->second;

		std::string s;
		std::unordered_map<std::string, std::string> states = m_PanelInput->GetInputStates()->GetStates();

		while (std::getline(ss, s, ';'))
		{
			std::string x;
			std::string y;
			std::string state;
			std::string tmp;

			std::stringstream line(s);
			line >> x >> y >> state >> tmp;

			if (x.empty()) continue;
			if (y.empty() || state.empty() || tmp.size())
			{
				hasErrors = true;
			}
			else
			{
				std::string signx = "+";
				std::string signy = "+";

				if (x[0] == '-' || x[0] == '+')
				{
					signx = x[0];
					x = x.substr(1);
				}
				if (y[0] == '-' || y[0] == '+')
				{
					signy = y[0];
					y = y.substr(1);
				}

				if (x.find_first_not_of("0123456789") != string::npos || y.find_first_not_of("0123456789") != string::npos)
				{
					hasErrors = true;
				}
				else
				{
					x = signx + x;
					y = signy + y;

					int nx = std::stoi(x);
					int ny = std::stoi(y);

					int xmin = -Sizes::N_COLS / 2;
					int xmax = Sizes::N_COLS / 2;
					int ymin = -Sizes::N_ROWS / 2;
					int ymax = Sizes::N_ROWS / 2;

					if (Sizes::N_COLS % 2 == 0) xmax--;
					if (Sizes::N_ROWS % 2 == 0) ymax--;

					if (nx < xmin || nx > xmax || ny < ymin || ny > ymax || states.find(state) == states.end())
					{
						hasErrors = true;
					}
					else
					{
						cells.push_back({ {nx-xmin, ny-ymin}, {state, wxColour(states[state])} });
					}
				}
			}
		}
	}

	if (cells.size())
	{
		Grid* grid = m_PanelGrid->GetGrid();
		grid->Reset();

		for (auto& cell : cells) grid->InsertCell(cell.first.first, cell.first.second, cell.second.first, cell.second.second, true);
		
		grid->UpdatePrev();
		grid->RefreshUpdate();
		grid->ScrollToCenter();
	}

	if (hasErrors) wxMessageBox("Some of the input appears to be invalid, as a result it has been ignored.", "Warning", wxICON_WARNING | wxOK);
}

void Main::MenuExport(wxCommandEvent& evt)
{
	wxFileDialog dialogFile(this, "Export Pattern", "", "", "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dialogFile.ShowModal() == wxID_CANCEL) return;

	std::ofstream out(dialogFile.GetPath().ToStdString());

	out << "[STATES]\n";
	std::string states = m_EditorStates->GetText().ToStdString();
	states.erase(std::remove(states.begin(), states.end(), '\r'), states.end());
	out << states << '\n';

	out << "[RULES]\n";
	std::string rules = m_EditorRules->GetText().ToStdString();
	rules.erase(std::remove(rules.begin(), rules.end(), '\r'), rules.end());
	out << rules << '\n';

	out << "[NEIGHBORS]\n";
	for (auto& neighbor : m_PanelInput->GetInputNeighbors()->GetNeighbors())
		out << neighbor << ' ';
	out << '\n';

	out << "[SIZE]\n";
	out << Sizes::N_ROWS << ' ' << Sizes::N_COLS << '\n';

	out << "[CELLS]\n";
	for (auto& cell : m_PanelGrid->GetGrid()->GetCells())
	{
		int x = cell.first.first - Sizes::N_COLS / 2;
		int y = cell.first.second - Sizes::N_ROWS / 2;
		std::string state = cell.second.first;

		out << x << ' ' << y << ' ' << state << ';' << '\n';
	}
	out << '\n';
}

void Main::MenuHelp(wxCommandEvent& evt)
{
	m_HelpWindow->Show();
	m_HelpWindow->SetFocus();
}

void Main::OnClose(wxCloseEvent& evt)
{
	m_PanelGrid->GetGrid()->WaitForPause();

	m_EditorStates->ForceClose();
	m_EditorRules->ForceClose();

	evt.Skip();
}