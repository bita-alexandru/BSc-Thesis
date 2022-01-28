#pragma once
class Sizes
{
private:
	Sizes() {};
	~Sizes() {};
public:
	inline static int N_ROWS;
	inline static int N_COLS;

	static const int CELL_SIZE_DEFAULT = 32;
	static const int CELL_SIZE_MIN = 4;
	static const int CELL_SIZE_MAX = 64;

	static const int MAIN_WIDTH = 1600;
	static const int MAIN_HEIGHT = 900;

	static const int EDITOR_WIDTH = 960;
	static const int EDITOR_HEIGHT = 720;

	static const int STATES_MAX = 256;
	static const int NEIGHBORS_MAX = 9;
	static const int RULES_MAX = 256*255;

	static const int CHARS_STATE_MIN = 1;
	static const int CHARS_STATE_MAX = 16;
	static const int CHARS_RULE_MIN = 4;
	static const int CHARS_RULE_MAX = 65536;
};

