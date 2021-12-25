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
	
	wxBitmapButton* m_Draw = nullptr;
	wxBitmapButton* m_Pick = nullptr;
	wxBitmapButton* m_Move = nullptr;
	
	wxStaticText* m_TextMode = nullptr;

	void BuildInterface();
	void UpdateTextMode();

	void OnMode(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
};

