#pragma once
#include "wx/wx.h"
#include "wx/tglbtn.h"

#include "Ids.h"

#include <unordered_set>

class InputNeighbors : public wxPanel
{
public:
	InputNeighbors(wxWindow* parent);
	~InputNeighbors();

	std::unordered_set<std::string>& GetNeighbors();
	
	void SetNeighbors(std::vector<std::string> neighbors);
private:
	std::unordered_set<std::string> m_Neighbors = std::unordered_set<std::string>();

	std::unordered_map<std::string, wxToggleButton*> m_Buttons = std::unordered_map<std::string, wxToggleButton*>();
	std::unordered_map<int, std::string> m_Ids = std::unordered_map<int, std::string>();

	void BuildInterface();

	void OnToggleButton(wxCommandEvent& evt);
};

