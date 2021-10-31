#include "Main.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "Test - wxWidgets", wxPoint(30, 30), wxSize(800, 600))
{
	btn = new wxButton* [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			int index = y * nFieldWidth + x;

			btn[index] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[index]->SetFont(font);
			grid->Add(btn[index], 1, wxEXPAND | wxALL);

			btn[index]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::OnButtonClicked, this);
		}
	}

	field = new int[nFieldWidth * nFieldHeight]{ 0 };

	this->SetSizer(grid);
	grid->Layout();
}

Main::~Main()
{
	delete[] btn;
	delete[] field;
}

void Main::OnButtonClicked(wxCommandEvent& evt)
{
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;
	int index = y * nFieldWidth + x;

	if (bFirstPick)
	{
		bFirstPick = false;

		srand(time(NULL));

		int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

		while (nMines--)
		{
			while (true)
			{
				int x = rand() % nFieldWidth;
				int y = rand() % nFieldHeight;
				int mine = y * nFieldWidth + x;

				if (mine == index) continue;
				if (field[mine] == -1) continue;

				field[mine] = -1;

				for (int d = 0; d < 8; d++)
				{
					int vx = x + dx[d];
					int vy = y + dy[d];

					if (vx >= 0 && vx < nFieldWidth && vy >= 0 && vy < nFieldHeight)
					{
						int neighbour = vy * nFieldWidth + vx;

						if (field[neighbour] != -1)
						{
							field[neighbour]++;
						}
					}
				}

				break;
			}
		}
	}

	int value = field[index];

	btn[index]->SetLabelText(std::to_string(value));
	btn[index]->Enable(false);

	if (value == -1)
	{
		wxMessageBox("Game Over");

		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				int index = y * nFieldWidth + x;

				btn[index]->Enable(true);
				btn[index]->SetLabelText("");
				field[index] = 0;
			}
		}

		bFirstPick = true;
		nMines = 20;
	}

	evt.Skip(); 
}