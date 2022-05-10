#pragma once
#include "wx/wx.h"

#include "Grid.h"

class AlgorithmOutput : public wxPanel
{
public:
	AlgorithmOutput(wxWindow* parent);
	~AlgorithmOutput();

	void SetGrid(Grid* grid);
private:
	Grid* m_Grid = nullptr;

	wxButton* m_Start = nullptr;
	wxButton* m_Stop = nullptr;
	wxButton* m_Save = nullptr;
	wxStaticText* m_TextGeneration = nullptr;
	wxStaticText* m_TextFitness = nullptr;
	wxStaticText* m_TextElapsed = nullptr;

	int m_Generation;
	double m_BestFitness;
	bool m_RenderOnScreen;
	int m_TimeElapsed;

	void BuildInterface();

	void OnStart(wxCommandEvent& evt);
	void OnStop(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);
};

