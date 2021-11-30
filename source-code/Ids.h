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
		ID_SAVE_STATES, ID_SAVE_RULES,
	};
};

