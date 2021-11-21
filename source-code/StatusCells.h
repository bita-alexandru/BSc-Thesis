#pragma once
#include "wx/wx.h"

class StatusCells
{
public:
	StatusCells(wxWindow* parent, wxBoxSizer* sizer);
	~StatusCells();

	int GetCountGeneration();
	int GetCountPopulation();
	void SetCountGeneration(int n);
	void SetCountPopulation(int n);
private:
	int m_CountGeneration = 0;
	int m_CountPopulation = 0;
	wxStaticText* m_TextCountGeneration = nullptr;
	wxStaticText* m_TextCountPopulation = nullptr;

	void UpdateTextCountGeneration();
	void UpdateTextCountPopulation();
};

