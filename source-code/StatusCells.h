#pragma once
#include "wx/wx.h"

class StatusCells : public wxPanel
{
public:
	StatusCells(wxWindow* parent);
	~StatusCells();

	int GetCountGeneration();
	int GetCountPopulation();

	void SetCountGeneration(int n);
	void SetCountPopulation(int n);

	void UpdateCountGeneration(int n);
	void UpdateCountPopulation(int n);

	void SetGenerationMessage(std::string message);
	void SetPopulationMessage(std::string message);
private:
	int m_CountGeneration = 0;
	int m_CountPopulation = 0;
	wxStaticText* m_TextCountGeneration = nullptr;
	wxStaticText* m_TextCountPopulation = nullptr;

	void BuildInterface();
	void UpdateTextCountGeneration();
	void UpdateTextCountPopulation();
};

