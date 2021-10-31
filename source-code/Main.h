#pragma once
#include "wx/wx.h"

class Main: public wxFrame
{
public:
	Main();
	~Main();

	const int nFieldWidth = 10;
	const int nFieldHeight = 10;
	wxButton** btn;

	int nMines = 20;
	int* field;
	bool bFirstPick = true;

	void OnButtonClicked(wxCommandEvent& evt);
};

