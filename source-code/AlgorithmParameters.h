#pragma once
#include "wx/wx.h"

class AlgorithmParameters : public wxPanel
{
public:
	AlgorithmParameters(wxWindow* parent);
	~AlgorithmParameters();
private:
	void BuildInterface();
};

