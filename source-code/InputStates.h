#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Ids.h"

#include <deque>

class InputStates : public wxPanel
{
public:
	InputStates(wxWindow* parent);
	~InputStates();

	wxListView* GetList();
	std::unordered_map<std::string, std::string>& GetStates();

	void SetStates(std::vector<std::string> states);
private:
	wxListView* m_List = nullptr;

	std::deque<std::string> m_Colors = std::deque<std::string>();
	std::unordered_map<std::string, std::string> m_States = std::unordered_map<std::string, std::string>();

	void BuildInterface();
	void InitializeColors();
	void MakeColorAvailable(std::string& color);
	void MakeColorUnavailable(std::string& color);
};

