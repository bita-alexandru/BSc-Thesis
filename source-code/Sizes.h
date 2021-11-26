#pragma once
class Sizes
{
private:
	Sizes() {};
	~Sizes() {};
public:
	static const int TOTAL_CELLS = 1000;

	static const int CELL_SIZE_DEFAULT = 16;
	static const int CELL_SIZE_MIN = 4;
	static const int CELL_SIZE_MAX = 64;

	static const int MAIN_WIDTH = 1600;
	static const int MAIN_HEIGHT = 900;

	static const int EDITOR_WIDTH = 960;
	static const int EDITOR_HEIGHT = 720;
};

