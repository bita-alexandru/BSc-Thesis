#pragma once
class Ids
{
private:
	Ids() {};
	~Ids() {};
public:
	enum
	{
		INDEX_START = 10000,

		// InputMenubar
		ID_OPEN_C, ID_OPEN_G,
		ID_SAVE_C, ID_SAVE_G,
		ID_EXIT,
		ID_CLEAR_C, ID_CLEAR_G,
		ID_VIEW_DEFAULT, ID_VIEW_INPUT, ID_VIEW_GRID, ID_VIEW_ALGORITHM,
		ID_USERMANUAL, ID_SHORTCUTS,

		// InputStates
		ID_EDIT_STATES,
		ID_SELECT_STATE,
		ID_GOTO_STATE,
		ID_COLOR_STATE,
		ID_DELETE_STATE,
		ID_LIST_STATES,

		// InputNeighbors
		ID_NEIGHBOR_NW, ID_NEIGHBOR_N, ID_NEIGHBOR_NE,
		ID_NEIGHBOR_W, ID_NEIGHBOR_C, ID_NEIGHBOR_E,
		ID_NEIGHBOR_SW, ID_NEIGHBOR_S, ID_NEIGHBOR_SE,

		// InputRules
		ID_EDIT_RULES,
		ID_SELECT_RULE,
		ID_GOTO_RULE,
		ID_DELETE_RULE,
		ID_LIST_RULES,

		// ToolZoom
		ID_ZOOM_OUT, ID_ZOOM_IN,

		// ToolUndo
		ID_BUTTON_UNDO, ID_BUTTON_REDO,

		// ToolModes
		ID_MODE_DRAW, ID_MODE_PICK, ID_MODE_MOVE,

		// ToolStates
		ID_BUTTON_PREV, ID_BUTTON_NEXT,

		// StatusSpeed
		ID_BUTTON_FASTER, ID_BUTTON_SLOWER,

		// StatusControls
		ID_BUTTON_CENTER,
		ID_BUTTON_START, ID_BUTTON_RESET,
		ID_BUTTON_STEP, ID_BUTTON_GENERATION,

		// text options
		ID_FIND, ID_REPLACE, ID_CUT, ID_COPY, ID_PASTE, ID_SELECT_ALL,

		// save buttons
		ID_SAVE_STATES, ID_SAVE_RULES,

		// timers
		ID_TIMER_SELECTION
	};
};

