#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Grid.h"

class Grid;

class StatusDelay : public wxPanel
{
public:
	StatusDelay(wxWindow* parent);
	~StatusDelay();

	int GetDelay();
	
	void SetGrid(Grid* grid);
private:
	Grid* m_Grid = nullptr;

	int m_Delay = 0;
	int m_MaximumDelay = 2000;
	int m_IncrementDelay = 250;

	wxStaticText* m_TextDelay = nullptr;

	void BuildInterface();
	void UpdateTextDelay();

	void IncreaseDelay(wxCommandEvent& evt);
	void DecreaseDelay(wxCommandEvent& evt);
};

