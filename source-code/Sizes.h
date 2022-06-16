#pragma once
class Sizes
{
private:
	Sizes() {};
	~Sizes() {};
public:
	// grid dimensions
	inline static int N_ROWS;
	inline static int N_COLS;

	static const int MAX_ROWS = 101;
	static const int MAX_COLS = 101;

	// grid cell pixel sizes
	static const int CELL_SIZE_DEFAULT = 32;
	static const int CELL_SIZE_MIN = 1;
	static const int CELL_SIZE_BORDER = 8;
	static const int CELL_SIZE_MAX = 512;

	// main window pixel sizes
	static const int MAIN_WIDTH = 1600;
	static const int MAIN_HEIGHT = 900;

	// sub-windows pixel sizes
	static const int EDITOR_WIDTH = 960;
	static const int EDITOR_HEIGHT = 720;

	// CA configuration limits
	static const int STATES_MAX = 256;
	static const int NEIGHBORS_MAX = 9;
	static const int RULES_MAX = 256*255;

	// editors length limits
	static const int CHARS_STATE_MIN = 1;
	static const int CHARS_STATE_MAX = 64;
	static const int CHARS_RULE_MIN = 4;
	static const int CHARS_RULE_MAX = 65536;
};

