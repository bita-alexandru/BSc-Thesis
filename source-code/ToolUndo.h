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
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions,
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> prevCells,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> prevStatePositions
	);

	void Reset();
private:
	Grid* m_Grid = nullptr;

	std::stack<
		std::pair<
			std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>,
			std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>
		>
	>m_UndoCells;
	std::stack<
		std::pair<
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>,
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>
		>
	>m_RedoCells;
	std::stack<
		std::pair<
			std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>,
			std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>
		>
	> m_UndoStatePositions;
	std::stack <
		std::pair<
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>
		>
	>m_RedoStatePositions;

	wxBitmapButton* m_Undo = nullptr;
	wxBitmapButton* m_Redo = nullptr;

	void Undo(wxCommandEvent& evt);
	void Redo(wxCommandEvent& evt);

	void BuildInterface();
};

