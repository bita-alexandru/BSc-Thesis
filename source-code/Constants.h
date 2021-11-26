#pragma once
class Constants
{
private:
	Constants() {}
	~Constants() {};
public:
	static const int TOTAL_CELLS = 1000;

	static const int CELL_SIZE_DEFAULT = 16;
	static const int CELL_SIZE_MIN = 4;
	static const int CELL_SIZE_MAX = 64;

	static const int MAIN_WIDTH = 1600;
	static const int MAIN_HEIGHT = 900;

	static const int EDITOR_WIDTH = 960;
	static const int EDITOR_HEIGHT = 720;

	enum
	{
		INDEX_START = 10000,

		// InputStates
		ID_EDIT_STATES,
		ID_DELETE_STATE,
		ID_GOTO_STATE,
		ID_LIST_STATES,

		// InputRules
		ID_EDIT_RULES,
		ID_DELETE_RULE,
		ID_GOTO_RULE,
		ID_LIST_RULES,

		// InputNeighbors
		ID_EDIT_NEIGHBORS,
		ID_DELETE_NEIGHBOR,
		ID_GOTO_NEIGHBOR,
		ID_LIST_NEIGHBORS,

		// InputMenubar
		ID_OPEN_C, ID_OPEN_G,
		ID_SAVE_C, ID_SAVE_G,
		ID_EXIT,
		ID_RESET_C, ID_RESET_G,
		ID_DOCUMENTATION,

		// StatusControls
		ID_BUTTON_START, ID_BUTTON_RESET,
		ID_BUTTON_STEP, ID_BUTTON_GENERATION,

		// StatusSpeed
		ID_BUTTON_FASTER, ID_BUTTON_SLOWER,

		// ToolModes
		ID_MODE_DRAW, ID_MODE_PICK, ID_MODE_DRAG,
		ID_BUTTON_PREV, ID_BUTTON_NEXT,

		// ToolZoom
		ID_ZOOM_IN, ID_ZOOM_OUT,

		// text options
		ID_FIND, ID_REPLACE, ID_CUT, ID_COPY, ID_PASTE, ID_SELECT_ALL,

		// save buttons
		ID_SAVE_STATES, ID_SAVE_RULES, ID_SAVE_NEIGHBORS,
	};
};

