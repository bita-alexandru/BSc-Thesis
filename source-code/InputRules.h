#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Ids.h"

#include <any>

class InputRules : public wxPanel
{
public:
	InputRules(wxWindow* parent);
	~InputRules();

	wxListView* GetList();
	std::unordered_map<std::string, std::unordered_map<std::string, std::any>>& GetRules();
	void SetRules(std::vector<std::string> rules);
private:
	wxListView* m_List = nullptr;
	std::unordered_map<std::string, std::unordered_map<std::string, std::any>> m_Rules = 
		std::unordered_map<std::string, std::unordered_map<std::string, std::any>>();

	void BuildInterface();
};

