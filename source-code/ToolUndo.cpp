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
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>& cells,
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>& statePositions,
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>& prevCells,
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>& prevStatePositions,
	std::unordered_map<std::pair<int, int>, std::unordered_map<std::string, std::string>, Hashes::PairInt>& neighbors,
	std::unordered_map<std::pair<int, int>, std::unordered_map<std::string, std::string>, Hashes::PairInt>& prevNeighbors
)
{
	// store the actual changes
	std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>> cellChanges;
	std::vector<std::pair<std::string, std::pair<int, int>>> statePositionsChanges;
	std::vector<std::pair<std::pair<int, int>, std::pair<std::string, std::string>>> neighborChanges;

	for (auto& i : cells)
	{
		// new cell
		if (prevCells.find(i.first) == prevCells.end()) cellChanges.push_back({ i.first,i.second });
		// new state/color
		else if (prevCells[i.first] != cells[i.first]) cellChanges.push_back({ i.first,i.second });
	}
	for (auto& i : statePositions)
	{
		// new state
		if (prevStatePositions.find(i.first) == prevStatePositions.end())
			for (auto& j : statePositions[i.first]) statePositionsChanges.push_back({ i.first,j });
		// check the positions
		else
		{
			for (auto& j : statePositions[i.first])
				// new position
				if (prevStatePositions[i.first].find(j) == prevStatePositions[i.first].end()) 
					statePositionsChanges.push_back({ i.first,j });
		}
	}
	for (auto& i : neighbors)
	{
		// new cell neighborhood
		if (prevNeighbors.find(i.first) == prevNeighbors.end())
			for (auto& j : neighbors[i.first]) neighborChanges.push_back({ i.first,j });
		// check the neighbors
		else
		{
			for (auto& j : neighbors[i.first])
				// new state on this direction
				if (prevNeighbors[i.first][j.first] != j.second)
					neighborChanges.push_back({ i.first,j });
		}
	}

	for (auto& i : prevCells)
	{
		// new cell
		if (cells.find(i.first) == cells.end()) cellChanges.push_back({ i.first,i.second });
		// new state/color
		else if (cells[i.first] != prevCells[i.first]) cellChanges.push_back({ i.first,i.second });
	}
	for (auto& i : prevStatePositions)
	{
		// new state
		if (statePositions.find(i.first) == statePositions.end())
			for (auto& j : prevStatePositions[i.first]) statePositionsChanges.push_back({ i.first,j });
		// check the positions
		else
		{
			for (auto& j : prevStatePositions[i.first])
				// new position
				if (statePositions[i.first].find(j) == statePositions[i.first].end())
					statePositionsChanges.push_back({ i.first,j });
		}
	}
	for (auto& i : prevNeighbors)
	{
		// new cell neighborhood
		if (neighbors.find(i.first) == neighbors.end())
			for (auto& j : prevNeighbors[i.first]) neighborChanges.push_back({ i.first,j });
		// check the neighbors
		else
		{
			for (auto& j : prevNeighbors[i.first])
				// new state on this direction
				if (neighbors[i.first][j.first] != j.second)
					neighborChanges.push_back({ i.first,j });
		}
	}

	m_UndoCells.push(cellChanges);
	m_UndoStatePositions.push(statePositionsChanges);
	m_UndoNeighbors.push(neighborChanges);

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
	m_RedoNeighbors = std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, std::string>>>>();

	m_UndoCells = std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>>>();
	m_UndoStatePositions = std::stack<std::vector<std::pair<std::string, std::pair<int, int>>>>();
	m_UndoNeighbors = std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, std::string>>>>();
}

void ToolUndo::Undo(wxCommandEvent& evt)
{
	if (m_UndoCells.empty()) return;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt> cells = m_Grid->GetCells();
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>> statePositions = m_Grid->GetStatePositions();
	std::unordered_map<std::pair<int, int>, std::unordered_map<std::string, std::string>, Hashes::PairInt> neighbors = m_Grid->GetNeighbors();

	// iterate through the most recent changes
	for (auto& it : m_UndoCells.top())
	{
		// if i find a change in the current configuration -> delete it
		if (cells.find(it.first) != cells.end()) cells.erase(it.first);
		// otherwise -> add it
		else cells.insert(it);
	}
	for (auto& it : m_UndoStatePositions.top())
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
			statePositions.insert({ it.first, std::unordered_set<std::pair<int,int>,Hashes::PairInt>()});
			statePositions[it.first].insert(it.second);
		}
	}
	for (auto& it : m_UndoNeighbors.top())
	{
		//// if i find a change in the current configuration -> update it
		//if (neighbors.find(it.first) != neighbors.end())
		//	//if (neighbors[it.first][it.second.first] != it.second.second) neighbors[it.first][it.second.first] = it.second.second;
		//	neighbors[it.first][it.second.first] = it.second.second;
		//// otherwise -> add it
		//else neighbors[it.first][it.second.first] = it.second.second;
		neighbors[it.first][it.second.first] = it.second.second;
	}

	m_RedoCells.push(m_UndoCells.top());
	m_RedoStatePositions.push(m_UndoStatePositions.top());
	m_RedoNeighbors.push(m_UndoNeighbors.top());

	m_UndoCells.pop();
	m_UndoStatePositions.pop();
	m_UndoNeighbors.pop();

	m_Redo->Enable();

	if (m_UndoCells.empty()) m_Undo->Disable();

	m_Grid->SetCells(cells, statePositions, neighbors);
	m_Grid->SetFocus();
}

void ToolUndo::Redo(wxCommandEvent& evt)
{
	if (m_RedoCells.empty()) return;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt> cells = m_Grid->GetCells();
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>> statePositions = m_Grid->GetStatePositions();
	std::unordered_map<std::pair<int, int>, std::unordered_map<std::string, std::string>, Hashes::PairInt> neighbors = m_Grid->GetNeighbors();

	// iterate through the most recent changes
	for (auto& it : m_RedoCells.top())
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
	for (auto& it : m_RedoStatePositions.top())
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
			statePositions.insert({ it.first, std::unordered_set<std::pair<int,int>,Hashes::PairInt>() });
			statePositions[it.first].insert(it.second);
		}
	}
	for (auto& it : m_RedoNeighbors.top())
	{
		//// if i find a change in the current configuration -> update it
		//if (neighbors.find(it.first) != neighbors.end())
		//	//if (neighbors[it.first][it.second.first] != it.second.second) neighbors[it.first][it.second.first] = it.second.second;
		//	neighbors[it.first][it.second.first] = it.second.second;
		//// otherwise -> add it
		//else neighbors[it.first][it.second.first] = it.second.second;
		neighbors[it.first][it.second.first] = it.second.second;
	}

	m_UndoCells.push(m_RedoCells.top());
	m_UndoStatePositions.push(m_RedoStatePositions.top());
	m_UndoNeighbors.push(m_RedoNeighbors.top());

	m_RedoCells.pop();
	m_RedoStatePositions.pop();
	m_RedoNeighbors.pop();

	m_Undo->Enable();

	if (m_RedoCells.empty()) m_Redo->Disable();

	m_Grid->SetCells(cells, statePositions, neighbors);
	m_Grid->SetFocus();
}

void ToolUndo::BuildInterface()
{
	m_Undo = new wxBitmapButton(this, Ids::ID_BUTTON_UNDO, wxBitmap("BTN_UNDO", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
	m_Redo = new wxBitmapButton(this, Ids::ID_BUTTON_REDO, wxBitmap("BTN_REDO", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));

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
