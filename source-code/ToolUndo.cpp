#include "ToolUndo.h"

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
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells,
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions,
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> prevCells,
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> prevStatePositions
)
{
	m_UndoCells.push({cells, prevCells});
	m_UndoStatePositions.push({ statePositions, prevStatePositions });

	m_RedoCells = std::stack<
		std::pair<
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>,
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>
		>
	>();
	m_RedoStatePositions = std::stack<
		std::pair<
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>
		>
	>();
	m_Redo->Disable();

	//SetFocus();

	if (m_UndoCells.size()) m_Undo->Enable();
}

void ToolUndo::Reset()
{
	m_Undo->Disable();
	m_Redo->Disable();

	m_RedoCells = std::stack<
		std::pair<
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>,
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>
		>
	>();
	m_RedoStatePositions = std::stack<
		std::pair<
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>
		>
	>();

	m_UndoCells = std::stack<
		std::pair<
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>,
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash>
		>
	>();
	m_UndoStatePositions = std::stack<
		std::pair<
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>>
		>
	>();
}

void ToolUndo::Undo(wxCommandEvent& evt)
{
	if (m_UndoCells.empty()) return;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells = m_UndoCells.top().first;
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> prevCells = m_UndoCells.top().second;
	m_UndoCells.pop();
	m_RedoCells.push({ cells, prevCells });

	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions = m_UndoStatePositions.top().first;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> prevStatePositions = m_UndoStatePositions.top().second;
	m_UndoStatePositions.pop();
	m_RedoStatePositions.push({ statePositions, prevStatePositions });

	m_Redo->Enable();

	if (m_UndoCells.empty())
	{
		m_Undo->Disable();
		m_Redo->SetFocus();
	}

	m_Grid->SetCells(prevCells, prevStatePositions);
}

void ToolUndo::Redo(wxCommandEvent& evt)
{
	if (m_RedoCells.empty()) return;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells = m_RedoCells.top().first;
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> prevCells = m_RedoCells.top().second;
	m_RedoCells.pop();
	m_UndoCells.push({ cells, prevCells});

	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions = m_RedoStatePositions.top().first;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> prevStatePositions = m_RedoStatePositions.top().second;
	m_RedoStatePositions.pop();
	m_UndoStatePositions.push({ statePositions, prevStatePositions });

	m_Undo->Enable();

	if (m_RedoCells.empty())
	{
		m_Redo->Disable();
		m_Undo->SetFocus();
	}

	m_Grid->SetCells(cells, statePositions);
}

void ToolUndo::BuildInterface()
{
	m_Undo = new wxButton(this, Ids::ID_BUTTON_UNDO, "U", wxDefaultPosition, wxSize(32, 32));
	m_Redo = new wxButton(this, Ids::ID_BUTTON_REDO, "R", wxDefaultPosition, wxSize(32, 32));

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
