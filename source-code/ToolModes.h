#pragma once
#include "wx/wx.h"
#include "wx/tglbtn.h"
#include "wx/listctrl.h"

class ToolModes : public wxPanel
{
public:
	ToolModes(wxWindow* parent);
	~ToolModes();

	char GetMode();
	int GetIndex();

	void SetMode(char mode);
	void SetStates(std::vector<std::pair<std::string, wxColour>> states);

	void SetListStates(wxListView* list);
private:
	char m_Mode = 'D';
	int m_Index = 0;
	int m_MaximumIndex = 0;

	std::vector<std::pair<std::string, wxColour>> m_States = std::vector<std::pair<std::string, wxColor>>({ {"FREE", wxColour("#FFFFFF")} });
	
	wxToggleButton* m_Draw = nullptr;
	wxToggleButton* m_Pick = nullptr;
	wxToggleButton* m_Move = nullptr;
	
	wxButton* m_State = nullptr;
	wxStaticText* m_TextIndex = nullptr;

	wxListView* m_ListStates = nullptr;

	void BuildInterface();
	void UpdateTextIndex();
	void UpdateStateColor();

	void OnToggleButton(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
	void OnPrev(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
};

