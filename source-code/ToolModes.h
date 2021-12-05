#pragma once
#include "wx/wx.h"

class ToolModes : public wxPanel
{
public:
	ToolModes(wxWindow* parent);
	~ToolModes();

	char GetMode();

	void SetMode(char mode);
private:
	char m_Mode = 'D';
	
	wxButton* m_Draw = nullptr;
	wxButton* m_Pick = nullptr;
	wxButton* m_Move = nullptr;
	
	wxStaticText* m_TextMode = nullptr;

	void BuildInterface();
	void UpdateTextMode();

	void OnMode(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
};

