#include "App.h"

wxIMPLEMENT_APP(App);

App::App()
{

}

App::~App()
{

}

bool App::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);

	m_Main = new Main();
	m_Main->Show();

	return true;
}