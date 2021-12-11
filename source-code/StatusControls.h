#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Grid.h"

class StatusControls : public wxPanel
{
public:
	StatusControls(wxWindow* parent);
	~StatusControls();

	char GetStartState();
	void SetStartState(char state);

	void SetGrid(Grid* grid);
private:
	Grid* m_Grid = nullptr;

	char m_StartState = 'S';
	wxButton* m_StartButton = nullptr;

	void BuildInterface();

	void GoToCenter(wxCommandEvent& evt);
};

