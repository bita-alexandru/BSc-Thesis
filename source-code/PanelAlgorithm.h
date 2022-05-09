#pragma once
#include "wx/wx.h"
#include "wx/statline.h"

#include "AlgorithmParameters.h"
#include "AlgorithmPattern.h"
#include "AlgorithmOutput.h"

class PanelAlgorithm: public wxScrolledWindow
{
public:
	PanelAlgorithm(wxWindow* parent);
	~PanelAlgorithm();

	AlgorithmParameters* GetAlgorithmParameters();
	AlgorithmOutput* GetAlgorithmOutput();
private:
	AlgorithmParameters* m_AlgorithmParameters;
	AlgorithmOutput* m_AlgorithmOutput;
};

