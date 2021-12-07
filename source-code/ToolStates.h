#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

class ToolStates : public wxPanel
{
public:
	ToolStates(wxWindow* parent);
	~ToolStates();

	int GetIndex();
	std::pair<std::string, wxColour> GetState();

	void SetStates(std::vector<std::pair<std::string, wxColour>> states);
	void SetListStates(wxListView* list);
	void SetState(std::string state);

	void SelectPrevState();
	void SelectNextState();
private:
	int m_Index = 0;
	int m_MaximumIndex = 0;
	wxListView* m_ListStates = nullptr;

	std::vector<std::pair<std::string, wxColour>> m_States = std::vector<std::pair<std::string, wxColor>>({ {"FREE", wxColour("white")} });

	wxStaticText* m_TextIndex = nullptr;
	wxPanel* m_State = nullptr;

	void BuildInterface();
	void UpdateTextIndex();
	void UpdateState();

	wxDECLARE_EVENT_TABLE();
	void OnPrev(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
};

