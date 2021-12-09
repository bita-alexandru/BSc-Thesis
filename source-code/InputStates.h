#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "ListStates.h"
#include "ToolStates.h"
#include "Grid.h"
#include "InputRules.h"

#include <deque>

class InputStates : public wxPanel
{
public:
	InputStates(wxWindow* parent);
	~InputStates();

	ListStates* GetList();
	std::unordered_map<std::string, std::string>& GetStates();

	void SetStates(std::vector<std::string> states);
	void SetToolStates(ToolStates* toolModes);
	void SetGrid(Grid* grid);
	void SetInputRules(InputRules* inputRules);
private:
	ListStates* m_List = nullptr;
	ToolStates* m_ToolStates = nullptr;
	Grid* m_Grid = nullptr;
	InputRules* m_InputRules = nullptr;

	std::deque<std::string> m_Colors = std::deque<std::string>();
	std::unordered_map<std::string, std::string> m_States;
	
	void BuildInterface();
	void InitializeColors();
	void MakeColorAvailable(std::string color);
	void MakeColorUnavailable(std::string color);
};

