#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Sizes.h"
#include "Hashes.h"
#include "Grid.h"

#include <stack>
#include <unordered_set>

class Grid;

class ToolUndo : public wxPanel
{
public:
	ToolUndo(wxWindow* parent);
	~ToolUndo();

	void SetGrid(Grid* grid);

	void PushBack(
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>& cells,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>& statePositions,
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>& prevCells,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>& prevStatePositions,
		std::unordered_map<std::pair<int, int>, std::unordered_map<std::string, std::string>, Hashes::PairInt>& neighbors,
		std::unordered_map<std::pair<int, int>, std::unordered_map<std::string, std::string>, Hashes::PairInt>& prevNeighbors
	);

	void Reset();
private:
	Grid* m_Grid = nullptr;

	std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>>> m_UndoCells;
	std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, wxColour>>>> m_RedoCells;
	std::stack<std::vector<std::pair<std::string, std::pair<int, int>>>> m_UndoStatePositions;
	std::stack<std::vector<std::pair<std::string, std::pair<int, int>>>> m_RedoStatePositions;
	std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, std::string>>>> m_UndoNeighbors;
	std::stack<std::vector<std::pair<std::pair<int, int>, std::pair<std::string, std::string>>>> m_RedoNeighbors;

	wxBitmapButton* m_Undo = nullptr;
	wxBitmapButton* m_Redo = nullptr;

	void Undo(wxCommandEvent& evt);
	void Redo(wxCommandEvent& evt);

	void BuildInterface();
};

