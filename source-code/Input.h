#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include <any>
#include <deque>

class Input
{
public:
	Input(wxListView* listStates, wxListView* listRules);
	~Input();
	
	std::unordered_map<std::string, std::string>* GetStates();
	std::unordered_map<std::string, bool>* GetNeighbors();
	std::unordered_map<std::string, std::any>* GetRules();
	std::deque<std::string>* GetColors();

	void SetStates(std::vector<std::string> states);
	void SetNeighbors(std::vector<std::string> neighbors);
	void SetRules(std::vector<std::string> rules);
private:
	wxListView* m_ListStates = nullptr;
	wxListView* m_ListNeighbors = nullptr;
	wxListView* m_ListRules = nullptr;

	std::unordered_map<std::string, std::string> m_States = std::unordered_map<std::string, std::string>();
	std::unordered_map<std::string, bool> m_Neighbors = std::unordered_map<std::string, bool>();
	std::unordered_map<std::string, std::any> m_Rules = std::unordered_map<std::string, std::any>();

	std::deque<std::string> m_Colors = std::deque<std::string>();

	void MakeColorAvailable(std::string& color);
	void MakeColorUnavailable(std::string& color);
};

