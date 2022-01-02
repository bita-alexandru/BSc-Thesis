#include "ToolUndo.h"

#include <fstream>

ToolUndo::ToolUndo(wxWindow* parent) : wxPanel(parent)
{
	SetBackgroundColour(wxColour(242, 204, 143));

	BuildInterface();
}

ToolUndo::~ToolUndo()
{
}

void ToolUndo::SetGrid(Grid* grid)
{
	m_Grid = grid;
}

void ToolUndo::PushBack(
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> &cells,
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> &statePositions,
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> &prevCells,
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> &prevStatePositions
)
{
	// store the actual changes
	std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>> cellChanges;
	std::vector<std::pair<std::string, std::pair<int, int>>> statePositionsChanges;

	for (auto i : cells)
	{
		// new cell
		if (prevCells.find(i.first) == prevCells.end())
		{
			//wxLogDebug("1");
			cellChanges.push_back({ i.first,i.second });
		}
		// new state/color
		else if (prevCells[i.first] != cells[i.first])
		{
			//wxLogDebug("2");
			cellChanges.push_back({ i.first,i.second });
		}
	}
	for (auto i : statePositions)
	{
		// new state
		if (prevStatePositions.find(i.first) == prevStatePositions.end())
		{
			for (auto j : statePositions[i.first])
			{
				statePositionsChanges.push_back({ i.first,j });
			}
		}
		// check the positions
		else
		{
			for (auto j : statePositions[i.first])
			{
				// new position
				if (prevStatePositions[i.first].find(j) == prevStatePositions[i.first].end())
				{
					statePositionsChanges.push_back({ i.first,j });
				}
			}
		}
	}

	m_UndoCells.push(cellChanges);
	m_UndoStatePositions.push(statePositionsChanges);

	m_RedoCells = std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>>>();
	m_RedoStatePositions = std::stack<std::vector<std::pair<std::string, std::pair<int, int>>>>();
	m_Redo->Disable();

	//SetFocus();

	if (m_UndoCells.size()) m_Undo->Enable();
}

void ToolUndo::Reset()
{
	m_Undo->Disable();
	m_Redo->Disable();

	m_RedoCells = std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>>>();
	m_RedoStatePositions = std::stack<std::vector<std::pair<std::string, std::pair<int, int>>>>();

	m_UndoCells = std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>>>();
	m_UndoStatePositions = std::stack<std::vector<std::pair<std::string, std::pair<int, int>>>>();
}

void ToolUndo::Undo(wxCommandEvent& evt)
{
	if (m_UndoCells.empty()) return;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells = m_Grid->GetCells();
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions = m_Grid->GetStatePositions();

	// iterate through the most recent changes
	for (auto it : m_UndoCells.top())
	{
		//wxLogDebug("%i,%i = %s,%s", it.first.first-200, it.first.second-200, it.second.first, it.second.second.GetAsString());
		// if i find a change in the current configuration -> delete it
		if (cells.find(it.first) != cells.end()) cells.erase(it.first);
		// otherwise -> add it
		else cells.insert(it);
	}
	for (auto it : m_UndoStatePositions.top())
	{
		//wxLogDebug("%s = %i,%i", it.first, it.second.first-200, it.second.second-200);
		// if i find a change in the current configuration -> update/insert it
		if (statePositions.find(it.first) != statePositions.end())
		{
			// delete
			if (statePositions[it.first].find(it.second) != statePositions[it.first].end()) statePositions[it.first].erase(it.second);
			// insert
			else statePositions[it.first].insert(it.second);
		}
		// otherwise -> add it
		else
		{
			statePositions.insert({ it.first, std::unordered_set<std::pair<int,int>,Hashes::PairHash>()});
			statePositions[it.first].insert(it.second);
		}
	}

	m_RedoCells.push(m_UndoCells.top());
	m_RedoStatePositions.push(m_UndoStatePositions.top());

	m_UndoCells.pop();
	m_UndoStatePositions.pop();

	m_Redo->Enable();

	if (m_UndoCells.empty()) m_Undo->Disable();

	m_Grid->SetCells(cells, statePositions);
	m_Grid->SetFocus();
}

void ToolUndo::Redo(wxCommandEvent& evt)
{
	if (m_RedoCells.empty()) return;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells = m_Grid->GetCells();
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions = m_Grid->GetStatePositions();

	// iterate through the most recent changes
	for (auto it : m_RedoCells.top())
	{
		// if i find a change in the current configuration -> delete it
		if (cells.find(it.first) != cells.end()) cells.erase(it.first);
		// otherwise -> add it
		else
		{
			it.second.second = m_Grid->GetColors()[it.second.first];
			cells.insert(it);
		}
	}
	for (auto it : m_RedoStatePositions.top())
	{
		// if i find a change in the current configuration -> update/insert it
		if (statePositions.find(it.first) != statePositions.end())
		{
			// delete
			if (statePositions[it.first].find(it.second) != statePositions[it.first].end()) statePositions[it.first].erase(it.second);
			// insert
			else statePositions[it.first].insert(it.second);
		}
		// otherwise -> add it
		else
		{
			statePositions.insert({ it.first, std::unordered_set<std::pair<int,int>,Hashes::PairHash>() });
			statePositions[it.first].insert(it.second);
		}
	}

	
	m_UndoCells.push(m_RedoCells.top());
	m_UndoStatePositions.push(m_RedoStatePositions.top());

	m_RedoCells.pop();
	m_RedoStatePositions.pop();

	m_Undo->Enable();

	if (m_RedoCells.empty()) m_Redo->Disable();

	m_Grid->SetCells(cells, statePositions);
	m_Grid->SetFocus();
}

void ToolUndo::BuildInterface()
{
	m_Undo = new wxBitmapButton(this, Ids::ID_BUTTON_UNDO, wxBitmap("D:/Diverse/BSc-Thesis/assets/buttons/undo.png", wxBITMAP_TYPE_PNG), wxDefaultPosition, wxSize(32, 32));
	m_Redo = new wxBitmapButton(this, Ids::ID_BUTTON_REDO, wxBitmap("D:/Diverse/BSc-Thesis/assets/buttons/redo.png", wxBITMAP_TYPE_PNG), wxDefaultPosition, wxSize(32, 32));

	m_Undo->SetToolTip("Undo");
	m_Undo->Disable();
	m_Undo->Bind(wxEVT_BUTTON, &ToolUndo::Undo, this);
	m_Redo->SetToolTip("Redo");
	m_Redo->Disable();
	m_Redo->Bind(wxEVT_BUTTON, &ToolUndo::Redo, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_Undo, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_Redo, 0, wxALIGN_CENTER_VERTICAL);

	this->SetSizerAndFit(sizer);
}
