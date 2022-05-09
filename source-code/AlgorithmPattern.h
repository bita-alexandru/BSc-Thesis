#pragma once
#include "wx/wx.h"

class AlgorithmPattern : public wxPanel
{
public:
	AlgorithmPattern(wxWindow* parent);
	~AlgorithmPattern();
private:
	void BuildInterface();
};