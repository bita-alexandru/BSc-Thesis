#include "AlgorithmOutput.h"

#include "wx/richmsgdlg.h"

#include <thread>
#include <fstream>
#include <chrono>

using Clock = chrono::high_resolution_clock;

wxBEGIN_EVENT_TABLE(AlgorithmOutput, wxPanel)
EVT_TIMER(Ids::ID_TIMER_ELAPSED, AlgorithmOutput::UpdateTimer)
wxEND_EVENT_TABLE()

AlgorithmOutput::AlgorithmOutput(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

AlgorithmOutput::~AlgorithmOutput()
{
	wxDELETE(m_Timer);
}

void AlgorithmOutput::SetGrid(Grid* grid)
{
	m_Grid = grid;
}

void AlgorithmOutput::SetInputStates(InputStates* inputStates)
{
	m_InputStates = inputStates;
}

void AlgorithmOutput::SetInputRules(InputRules* inputRules)
{
	m_InputRules = inputRules;
}

void AlgorithmOutput::SetInputNeighbors(InputNeighbors* inputNeighbors)
{
	m_InputNeighbors = inputNeighbors;
}

void AlgorithmOutput::SetAlgorithmParameters(AlgorithmParameters* algorithmParameters)
{
	m_AlgorithmParameters = algorithmParameters;
}

void AlgorithmOutput::BuildInterface()
{
	m_Timer = new wxTimer(this, Ids::ID_TIMER_ELAPSED);

	m_Start = new wxButton(this, wxID_ANY, "Start");
	m_Start->Bind(wxEVT_BUTTON, &AlgorithmOutput::OnStart, this);

	m_Stop = new wxButton(this, wxID_ANY, "Stop");
	m_Stop->Disable();
	m_Stop->Bind(wxEVT_BUTTON, &AlgorithmOutput::OnStop, this);

	m_Save = new wxButton(this, wxID_ANY, "Save");
	m_Save->Disable();
	m_Save->Bind(wxEVT_BUTTON, &AlgorithmOutput::OnSave, this);

	wxBoxSizer* sizerButtons = new wxBoxSizer(wxHORIZONTAL);
	sizerButtons->Add(m_Start, 0);
	sizerButtons->Add(m_Stop, 0, wxLEFT | wxRIGHT, 0);
	sizerButtons->Add(m_Save, 0);

	m_TextEpoch = new wxStaticText(this, wxID_ANY, "Epoch: -");

	m_TextLastNofGeneration = new wxStaticText(this, wxID_ANY, "-");
	m_TextLastAvgPopulation = new wxStaticText(this, wxID_ANY, "-");
	m_TextLastInitialSize = new wxStaticText(this, wxID_ANY, "-");
	m_TextLastFitness = new wxStaticText(this, wxID_ANY, "-");

	m_TextBestNofGeneration = new wxStaticText(this, wxID_ANY, "-");
	m_TextBestAvgPopulation = new wxStaticText(this, wxID_ANY, "-");
	m_TextBestInitialSize = new wxStaticText(this, wxID_ANY, "-");
	m_TextBestFitness = new wxStaticText(this, wxID_ANY, "-");

	wxFlexGridSizer* sizerLast = new wxFlexGridSizer(2, 4, wxSize(24, 0));
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last No. Generations"), 0, wxALIGN_RIGHT);
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last Avg. Population"), 0, wxALIGN_RIGHT);
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last Initial Size"), 0, wxALIGN_RIGHT);
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last Fitness"), 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastNofGeneration, 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastAvgPopulation, 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastInitialSize, 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastFitness, 0, wxALIGN_RIGHT);

	wxFlexGridSizer* sizerBest = new wxFlexGridSizer(2, 4, wxSize(24, 0));
	sizerBest->Add(new wxStaticText(this, wxID_ANY, "Best No. Generations"), 0, wxALIGN_RIGHT);
	sizerBest->Add(new wxStaticText(this, wxID_ANY, "Best Avg. Population"), 0, wxALIGN_RIGHT);
	sizerBest->Add(new wxStaticText(this, wxID_ANY, "Best Initial Size"), 0, wxALIGN_RIGHT);
	sizerBest->Add(new wxStaticText(this, wxID_ANY, "Best Fitness"), 0, wxALIGN_RIGHT);
	sizerBest->Add(m_TextBestNofGeneration, 0, wxALIGN_RIGHT);
	sizerBest->Add(m_TextBestAvgPopulation, 0, wxALIGN_RIGHT);
	sizerBest->Add(m_TextBestInitialSize, 0, wxALIGN_RIGHT);
	sizerBest->Add(m_TextBestFitness, 0, wxALIGN_RIGHT);

	m_TextElapsed = new wxStaticText(this, wxID_ANY, "Time Elapsed: -");

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(sizerButtons, 0);
	sizer->AddSpacer(8);
	sizer->Add(m_TextEpoch, 0);
	sizer->AddSpacer(8);
	sizer->Add(sizerLast, 0);
	sizer->AddSpacer(8);
	sizer->Add(sizerBest, 0);
	sizer->AddSpacer(8);
	sizer->Add(m_TextElapsed, 0);

	SetSizerAndFit(sizer);
}

void AlgorithmOutput::RunAlgorithm()
{
	m_States = m_InputStates->GetList()->GetStates();
	m_Rules = m_InputRules->GetList()->GetRules();
	m_Neighbors = m_InputNeighbors->GetNeighborsAsVector();

	unordered_map<string, string> states = m_InputStates->GetStates();
	vector<pair<string, Transition>> rules = m_InputRules->GetRules();
	unordered_set<string> neighbors = m_InputNeighbors->GetNeighbors();

	if (states.size() <= 1 || rules.size() == 0)
	{
		EndAlgorithm(false);
		return;
	}

	string errors = CheckValidAutomaton(states, rules, neighbors);
	if (errors.size())
	{
		wxRichMessageDialog dialog(
			this, "Some of the rules appear to be invalid.", "Error",
			wxOK | wxICON_ERROR
		);
		dialog.ShowDetailedText(errors);
		dialog.ShowModal();

		EndAlgorithm(false);
		return;
	}

	GetParameters();

	generator.seed(Clock::now().time_since_epoch().count());

	UpdateTextEpoch(0);

	vector<Chromosome> population = InitializePopulation();
	EvaluatePopulation(population, states, rules, neighbors);

	Chromosome bestChromosome = GetBestChromosome(population, 0);
	m_BestChromosome = bestChromosome;

	UpdateTextLast(bestChromosome);
	UpdateTextBest(bestChromosome);

	int epochs = 0;
	while (++epochs && m_Running)
	{
		wxLogDebug("Epoch %i", epochs);
		UpdateTextEpoch(epochs);

		population = SelectPopulation(population);
		DoCrossover(population);
		DoMutatiton(population);

		UpdateChromosomesMaps(population);
		EvaluatePopulation(population, states, rules, neighbors);

		bestChromosome = GetBestChromosome(population, epochs);
		UpdateTextLast(bestChromosome);

		if (bestChromosome > m_BestChromosome)
		{
			m_BestChromosome = bestChromosome;
			UpdateTextBest(bestChromosome);
		}

		if (epochs == epochsTarget) break;
	}

	EndAlgorithm();
}

void AlgorithmOutput::GetParameters()
{
	rows = Sizes::N_ROWS;
	cols = Sizes::N_COLS;

	popSize = m_AlgorithmParameters->GetPopulationSize();
	pc = m_AlgorithmParameters->GetProbabilityCrossover();
	pm = m_AlgorithmParameters->GetProbabilityMutation();
	selectionMethod = m_AlgorithmParameters->GetSelectionMethod();

	generationMultiplier = m_AlgorithmParameters->GetGenerationMultiplier();
	populationMultiplier = m_AlgorithmParameters->GetPopulationMultiplier();
	initialSizeMultiplier = m_AlgorithmParameters->GetInitialSizeMultiplier();

	generationTarget = m_AlgorithmParameters->GetGenerationTarget();
	populationTarget = m_AlgorithmParameters->GetPopulationTarget();
	epochsTarget = m_AlgorithmParameters->GetEpochsTarget();

	wxLogDebug("%i\n%f %f\n%s\n%i %i %i", popSize, pc, pm, selectionMethod, generationTarget, populationTarget, epochsTarget);
}

vector<Chromosome> AlgorithmOutput::InitializePopulation()
{
	wxLogDebug("Init pop [start]");
	vector<Chromosome> population;

	uniform_real_distribution<double> r01(0, 1);
	uniform_int_distribution<int> i1n(1, m_States.size() - 1);

	for (int i = 0; i < popSize && m_Running; i++)
	{
		double cellProbability = r01(generator);

		vector<int> pattern(rows * cols);
		unordered_map<int, string> cells;
		unordered_map<string, unordered_set<int>> statePositions;
		int initialSize = 0;

		for (int j = 0; j < rows * cols && m_Running; j++)
		{
			double p = r01(generator);

			if (p <= cellProbability)
			{
				int cellType = i1n(generator);

				pattern[j] = cellType;
				initialSize++;

				cells[j] = m_States[cellType];
				statePositions[m_States[cellType]].insert(j);
			}
		}

		Chromosome chromosome;
		chromosome.id = i;
		chromosome.pattern = pattern;
		chromosome.initialPattern = pattern;
		chromosome.initialSize = initialSize;
		chromosome.cells = cells;
		chromosome.statePositions = statePositions;
		chromosome.nOfGenerations = 0;
		chromosome.avgPopulation = 0;
		chromosome.fitness = 1.0;

		//wxLogDebug("Generated chromosome %i with size %i", chromosome.id, initialSize);
		population.push_back(chromosome);
	}

	wxLogDebug("Init pop [end]");
	return population;
}

void AlgorithmOutput::EvaluatePopulation(vector<Chromosome>& population, unordered_map<string, string>& states,
	vector<pair<string, Transition>>& rules, unordered_set<string>& neighbors)
{
	wxLogDebug("Evaluate pop [start]");

	for (int i = 0; i < popSize && m_Running; i++)
	{
		wxLogDebug("Chromosome %i", population[i].id);

		population[i].pattern = population[i].initialPattern;
		//ShowChromosomePattern(population[i]);

		int nOfGenerations = 0;
		double avgPopulation = 0;
		while (++nOfGenerations && m_Running)
		{
			//wxLogDebug("Generation %i", nOfGenerations);

			//ShowChromosomePattern(population[i]);

			pair<vector<pair<string, pair<int, int>>>, string> result =
				ParseAllRules(population[i].cells, population[i].statePositions, states, rules, neighbors);

			if (result.second.size())
			{
				wxLogDebug("error");
				break;
			}

			UpdateGeneration(result.first, population[i].pattern, population[i].cells, population[i].statePositions);

			if (result.first.empty())
			{
				//wxLogDebug("End of universe");
				break;
			}

			avgPopulation += population[i].cells.size();

			if (nOfGenerations && nOfGenerations - 1 >= generationTarget) break;
			if (populationTarget && population[i].cells.size() >= populationTarget) break;
		}
		nOfGenerations--;

		if (nOfGenerations) avgPopulation /= nOfGenerations;
		population[i].nOfGenerations = nOfGenerations;
		population[i].avgPopulation = ceil(avgPopulation);

		//ShowChromosomePattern(population[i]);

		double fitness = 1.0 + generationMultiplier * nOfGenerations + populationMultiplier * avgPopulation;
		if (population[i].initialSize && initialSizeMultiplier) fitness /= (initialSizeMultiplier * population[i].initialSize);

		wxLogDebug("No. Generations: %i\nAvg. Population: %f\nFitness: %f", nOfGenerations, avgPopulation, fitness);

		population[i].fitness = fitness;
	}

	wxLogDebug("Evaluate pop [end]");
}

vector<Chromosome> AlgorithmOutput::SelectPopulation(vector<Chromosome>& population)
{
	if (selectionMethod == "Roulette Wheel") return RouletteWheelSelection(population);

	return {};
}

vector<Chromosome> AlgorithmOutput::RouletteWheelSelection(vector<Chromosome>& population)
{
	wxLogDebug("Select pop [start]");

	double totalFitness = 0.0;
	for (int i = 0; i < popSize; i++) totalFitness += population[i].fitness;

	wxLogDebug("Total fitness: %f", totalFitness);

	vector<double> q(popSize + 1);
	for (int i = 0; i < popSize && m_Running; i++)
	{
		//wxLogDebug("%i. Chromosome %i", i, population[i].id);
		double p = population[i].fitness / totalFitness;
		q[i + 1] = q[i] + p;

		//wxLogDebug("p=%f q=%f", p, q[i + 1]);
	}

	uniform_real_distribution<double> r01(0, 1);

	vector<Chromosome> newPopulation;
	int j = 0;
	while (j != popSize && m_Running)
	{
		for (int i = 0; i < popSize && m_Running; i++)
		{
			double p = r01(generator);
			//wxLogDebug("%i. Generated probability: %f", i, p);
			//wxLogDebug("%i/%i. %.17g < %.17g <= %.17g = %i", i, j, q[j], p, q[j + 1], (p > q[j] && p <= q[j + 1]));

			if (p >= q[j] && p <= q[j + 1])
			{
				//wxLogDebug("Selected into new pop: %i", population[i].id);

				j++;

				Chromosome chromosome = population[i];
				chromosome.id = j;

				newPopulation.push_back(chromosome);

				if (j == popSize) break;
			}
		}
	}

	wxLogDebug("Select pop [end]");
	return newPopulation;
}

void AlgorithmOutput::DoCrossover(vector<Chromosome>& population)
{
	wxLogDebug("Cross-over [start]");
	vector<int> parents;
	int parentsSize = 0;

	uniform_real_distribution<double> r01(0, 1);

	for (int i = 0; i < popSize && m_Running; i++)
	{
		double p = r01(generator);

		//wxLogDebug("%i. Generated probability: %f", i, p);

		if (p < pc)
		{
			//wxLogDebug("Selecting parent %i", population[i].id);
			parents.push_back(i);
			parentsSize++;
		}
	}

	const int N = rows * cols;

	uniform_int_distribution<int> i0n(0, N - 1);

	for (int i = 0; i < parentsSize - 1 && m_Running; i += 2)
	{
		int xp1 = i0n(generator);
		int xp2 = i0n(generator);

		while (xp1 == xp2 && N > 1 && m_Running)
		{
			xp2 = i0n(generator);
		}

		if (xp1 > xp2) swap(xp1, xp2);

		//wxLogDebug("Generated cutpoints: %i, %i", xp1, xp2);

		for (int j = xp1; j <= xp2 && m_Running; j++)
		{
			int p1 = parents[i];
			int p2 = parents[i + 1];

			//wxLogDebug("Swapping {%i}[%i](=%i) with {%i}[%i](=%i)", population[p1].id, j,
			//population[p1].initialPattern[j], population[p2].id, j, population[p2].initialPattern[j]);
			swap(population[p1].initialPattern[j], population[p2].initialPattern[j]);
		}
	}

	wxLogDebug("Cross-over [end]");
}

void AlgorithmOutput::DoMutatiton(vector<Chromosome>& population)
{
	wxLogDebug("Mutation [start]");

	const int N = rows * cols;

	uniform_int_distribution<int> i0n(0, m_States.size() - 1);
	uniform_real_distribution<double> r01(0, 1);

	for (int i = 0; i < popSize && m_Running; i++)
	{
		//wxLogDebug("%i. Chromosome %i", i, population[i].id);
		for (int j = 0; j < N && m_Running; j++)
		{
			double p = r01(generator);
			//wxLogDebug("Generated probability: %f", p);

			if (p <= pm)
			{
				int cellType = i0n(generator);

				//wxLogDebug("Changing [%i](=%i) with %i", j, population[i].initialPattern[j], cellType);
				population[i].initialPattern[j] = cellType;
			}
		}
	}

	wxLogDebug("Mutation [end]");
}

Chromosome AlgorithmOutput::GetBestChromosome(vector<Chromosome>& population, int epoch)
{
	wxLogDebug("Get best [start]");
	Chromosome chromosome = population[0];

	for (int i = 1; i < popSize; i++)
	{
		if (population[i] > chromosome)
		{
			//wxLogDebug("New best %i with fitness %f", population[i].id, population[i].fitness);
			chromosome = population[i];
		}
	}

	wxLogDebug("Best chromosome of epoch %i is %i with fitness %f", epoch, chromosome.id, chromosome.fitness);

	wxLogDebug("Get best [end]");

	return chromosome;
}

void AlgorithmOutput::OnStart(wxCommandEvent& evt)
{
	Start();
}

void AlgorithmOutput::OnStop(wxCommandEvent& evt)
{
	Stop();
}

void AlgorithmOutput::OnSave(wxCommandEvent& evt)
{
	Save();
}

void AlgorithmOutput::OnRender(wxCommandEvent& evt)
{
	m_RenderOnScreen = !m_RenderOnScreen;
}

void AlgorithmOutput::Start()
{
	if (m_Running) return;

	m_Running = true;

	m_Start->Disable();
	m_Stop->Enable();
	m_Save->Disable();

	m_BestChromosome.id = -1;

	m_TimeElapsed = -1;
	m_Timer->Start(1000);

	thread t(&AlgorithmOutput::RunAlgorithm, this);
	t.detach();
}

void AlgorithmOutput::Stop()
{
	if (!m_Running) return;

	m_Running = false;

	m_Start->Enable();
	m_Stop->Disable();
}

void AlgorithmOutput::Save()
{
	wxFileDialog dialogFile(this, "Export Pattern", "", "", "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dialogFile.ShowModal() == wxID_CANCEL) return;

	ofstream out(dialogFile.GetPath().ToStdString());

	out << "[ALGORITHM SETTINGS]\n";
	out << wxString::Format(
		"%s\n%s\n\nPopulation Size: %i\nProbability of Mutation: %f\nProbability of Crossover: %f\nSelection Method: %s\n\n%s",
		m_TextElapsed->GetLabel(), m_TextEpoch->GetLabel(), popSize, pm, pc, selectionMethod,
		wxString::Format(
			"Generation Multiplier: %f\nPopullation Multiplier: %f\nInitial Size Multiplier: %f\n\n%s",
			generationMultiplier, populationMultiplier, initialSizeMultiplier,
			wxString::Format(
				"Epochs Target: %i\nGeneration Target: %i\nPopulation Target: %i\n",
				epochsTarget, generationTarget, populationTarget
			)
		)
	) << "\n";

	out << "[STATES]\n";
	for (int i = 1; i < m_States.size(); i++) out << m_States[i] << ";\n";

	out << "[RULES]\n";
	for (auto& rule : m_Rules) out << rule << "\n";

	out << "[NEIGHBORS]\n";
	for (auto& neighbor : m_Neighbors) out << neighbor << ' ';
	out << '\n';

	out << "[SIZE]\n";
	out << rows << ' ' << cols << '\n';

	out << "[CELLS]\n";
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int k = i * cols + j;

			if (m_BestChromosome.initialPattern[k])
			{
				int x = j - cols / 2;
				int y = i - rows / 2;
				string state = m_States[m_BestChromosome.initialPattern[k]];

				out << x << ' ' << y << ' ' << state << ';' << '\n';
			}
		}
	}
}

void AlgorithmOutput::UpdateTextEpoch(int epoch)
{
	m_TextEpoch->SetLabel(wxString::Format("Epoch: %i", epoch));
}

void AlgorithmOutput::UpdateTextElapsed(int elapsed)
{
	int seconds = elapsed % 60;
	int minutes = elapsed / 60;
	int hours = elapsed / 3600;

	wxString textSeconds = (seconds < 10) ? wxString::Format("0%i", seconds) : wxString::Format("%i", seconds);
	wxString textMinutes = (minutes < 10) ? wxString::Format("0%i", minutes) : wxString::Format("%i", minutes);
	wxString textHours = (hours < 10) ? wxString::Format("0%i", hours) : wxString::Format("%i", hours);

	m_TextElapsed->SetLabel(wxString::Format("Time Elapsed: %s:%s:%s", textHours, textMinutes, textSeconds));
}

void AlgorithmOutput::UpdateTextLast(Chromosome& chromosome)
{
	m_TextLastFitness->SetLabel(to_string(chromosome.fitness));
	m_TextLastNofGeneration->SetLabel(to_string(chromosome.nOfGenerations));
	m_TextLastAvgPopulation->SetLabel(to_string(chromosome.avgPopulation));
	m_TextLastInitialSize->SetLabel(to_string(chromosome.initialSize));
}

void AlgorithmOutput::UpdateTextBest(Chromosome& chromosome)
{
	m_TextBestFitness->SetLabel(to_string(chromosome.fitness));
	m_TextBestNofGeneration->SetLabel(to_string(chromosome.nOfGenerations));
	m_TextBestAvgPopulation->SetLabel(to_string(chromosome.avgPopulation));
	m_TextBestInitialSize->SetLabel(to_string(chromosome.initialSize));
}

pair<vector<pair<string, pair<int, int>>>, string> AlgorithmOutput::ParseAllRules(
	unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions,
	unordered_map<string, string>& states, vector<pair<string, Transition>>& rules, unordered_set<string>& neighbors
)
{
	vector<pair<string, pair<int, int>>> changes;

	for (int i = 0; i < rules.size(); i++)
	{
		if (!m_Running) break;

		//wxLogDebug("RULE=%s/%s:%s", rule.first, rule.second.state, rule.second.condition);

		pair<vector<pair<int, int>>, string> result = ParseRule(rules[i], cells, statePositions, states, neighbors);

		// error
		if (result.second.size())
		{
			// mark the problematic rule index
			result.second += " at rule number " + to_string(i);

			return { {}, result.second };
		}
		// concatenate changes
		else
		{
			string newstate = rules[i].first + "*" + rules[i].second.state + "*";

			for (auto& change : result.first)
			{
				if (!m_Running) break;

				changes.push_back({ newstate , change });
			}
		}
	}

	return { changes,"" };
}

pair<vector<pair<int, int>>, string> AlgorithmOutput::ParseRule(pair<string, Transition>& rule,
	unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions,
	unordered_map<string, string>& states, unordered_set<string>& neighbors)
{
	vector<pair<int, int>> applied;
	unordered_set<int> visited;

	// if state is "FREE", apply rule to all "FREE" cells
	if (rule.first == "FREE")
	{
		// iterate through all cells
		if (rule.second.all || rule.second.condition.empty())
		{
			//wxLogDebug("FREE_ALL");
			const int N = rows * cols;

			for (int i = 0; i < N && m_Running; i++)
			{
				int x = i % rows;
				int y = i / cols;

				if (GetState(x, y, cells) == "FREE" && ApplyOnCell(x, y, rule.second, cells, neighbors))
				{
					//wxLogDebug("111");
					applied.push_back({ x,y });
				}
			}
		}
		// decide if it's faster to iterate through all cells
		// or through the condition states' neighbors
		else
		{
			int n1 = rows * cols - cells.size();
			int n2 = 0;
			for (auto& state : rule.second.states)
			{
				if (!m_Running) break;

				if (statePositions.find(state) == statePositions.end()) continue;
				n2 += statePositions[state].size();
			}
			//wxLogDebug("N1=%i N2=%i",n1,n2);

			// faster to iterate through all cells
			if (n1 <= n2 || rule.second.condition.empty())
			{
				//wxLogDebug("FREE ALL");
				const int N = rows * cols;

				for (int i = 0; i < N && m_Running; i++)
				{
					int x = i % rows;
					int y = i / cols;

					if (GetState(x, y, cells) == "FREE" && ApplyOnCell(x, y, rule.second, cells, neighbors))
					{
						//wxLogDebug("112");
						applied.push_back({ x,y });
					}
				}
			}
			// faster to iterate through the condition states' neighbors
			else
			{
				//wxLogDebug("FREE NEIGHBORS");
				for (auto& state : rule.second.states)
				{
					if (!m_Running) break;

					if (state == "FREE")
					{
						wxLogDebug("FREE ALL");
						const int N = rows * cols;

						for (int i = 0; i < N && m_Running; i++)
						{
							int x = i % rows;
							int y = i / cols;
							int k = i;

							if (visited.find(k) == visited.end() && GetState(x, y, cells) == "FREE" && ApplyOnCell(x, y, rule.second, cells, neighbors))
							{
								wxLogDebug("121");
								applied.push_back({ x,y });
								visited.insert(k);
							}
						}
					}
					// cells of this type are placed on grid
					else if (statePositions.find(state) != statePositions.end())
					{
						//wxLogDebug("FREE ADJACENTS");
						int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
						int dy[8] = { -1,-1,0,1,1,1,0,-1 };

						for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
						{
							if (!m_Running)break;

							int k = *i;
							int x = k % cols;
							int y = k / cols;

							for (int d = 0; d < 8 && m_Running; d++)
							{
								int nx = x + dx[d];
								int ny = y + dy[d];
								int k = ny * cols + nx;

								if (visited.find(k) == visited.end() && InBounds(nx, ny) && GetState(nx, ny, cells) == rule.first && ApplyOnCell(nx, ny, rule.second, cells, neighbors))
								{
									//wxLogDebug("122");
									applied.push_back({ nx,ny });
									visited.insert(k);
								}
							}
						}
					}
				}
			}
		}
	}
	// else, get all cells of that type
	else
	{
		if (statePositions.find(rule.first) == statePositions.end()) return { {},"" };

		// iterate through all cells
		if (rule.second.all || rule.second.condition.empty())
		{
			string state = rule.first;
			//wxLogDebug("STATE ALL");
			for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
			{
				if (!m_Running) break;

				int k = *i;
				int x = k % cols;
				int y = k / cols;

				if (ApplyOnCell(x, y, rule.second, cells, neighbors))
				{
					//wxLogDebug("211");
					applied.push_back({ x,y });
				}
			}
		}
		// decide if it's faster to iterate through all cells
		// or through the condition states' neighbors
		else
		{
			int n1 = statePositions[rule.first].size();
			int n2 = 0;
			for (auto& state : rule.second.states)
			{
				if (!m_Running) break;

				if (statePositions.find(state) == statePositions.end()) continue;
				n2 += statePositions[state].size();
			}

			// faster to iterate through all cells
			if (n1 <= n2 || rule.second.condition.empty())
			{
				//wxLogDebug("STATE ALL");
				string state = rule.first;
				for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
				{
					if (!m_Running) break;

					int k = *i;
					int x = k % cols;
					int y = k / cols;

					if (ApplyOnCell(x, y, rule.second, cells, neighbors))
					{
						//wxLogDebug("221");
						applied.push_back({ x,y });
					}
				}
			}
			// faster to iterate through the condition states' neighbors
			else
			{
				for (auto& state : rule.second.states)
				{
					if (!m_Running) break;

					if (state == "FREE")
					{
						//wxLogDebug("STATE ALL");
						string state = rule.first;
						for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
						{
							int k = *i;
							int x = k % cols;
							int y = k / cols;

							if (visited.find(k) == visited.end() && ApplyOnCell(x, y, rule.second, cells, neighbors))
							{
								//wxLogDebug("231");
								applied.push_back({ x,y });
								visited.insert(k);
							}
						}
					}
					// cells of this type are placed on grid
					else if (statePositions.find(state) != statePositions.end())
					{
						//wxLogDebug("STATE ADJACENTS");
						int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
						int dy[8] = { -1,-1,0,1,1,1,0,-1 };

						for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
						{
							if (!m_Running) break;

							int k = *i;
							int x = k % cols;
							int y = k / cols;

							for (int d = 0; d < 8 && m_Running; d++)
							{
								int nx = x + dx[d];
								int ny = y + dy[d];
								int k = ny * cols + nx;

								if (visited.find(k) == visited.end() && InBounds(nx, ny) && GetState(nx, ny, cells) == rule.first && ApplyOnCell(nx, ny, rule.second, cells, neighbors))
								{
									//wxLogDebug("232");
									applied.push_back({ nx,ny });
									visited.insert(k);
								}
							}
						}
					}
				}
			}
		}
	}

	return { applied, "" };
}

string AlgorithmOutput::CheckValidAutomaton(unordered_map<string, string>& states, vector<pair<string, Transition>>& rules, unordered_set<string>& neighbors)
{
	string errors = "";

	for (int i = 0; i < rules.size(); i++)
	{
		// check if rule might contain invalid states
		if (states.find(rules[i].first) == states.end())
			errors += (wxString::Format("<INVALID FIRST STATE> at rule number %i\n", i).ToStdString());

		if (states.find(rules[i].second.state) == states.end())
			errors += (wxString::Format("<INVALID SECOND STATE at rule number %i\n", i).ToStdString());

		for (auto& state : rules[i].second.states)
		{
			if (states.find(state) == states.end())
				errors += (wxString::Format("<INVALID CONDITION STATE at rule number %i\n", i).ToStdString());
		}
		// check for neighborhood as well
		for (auto& direction : rules[i].second.directions)
		{
			if (neighbors.find(direction) == neighbors.end())
				errors += (wxString::Format("<INVALID NEIGHBORHOOD at rule number %i\n", i).ToStdString());
		}
	}

	return errors;
}

string AlgorithmOutput::GetState(int x, int y, unordered_map<int, string>& cells)
{
	int k = y * cols + x;

	if (cells.find(k) == cells.end()) return "FREE";

	return cells[k];
}

bool AlgorithmOutput::InBounds(int x, int y)
{
	return (x >= 0 && x < cols&& y >= 0 && y < rows);
}

unordered_map<string, string> AlgorithmOutput::GetNeighborhood(int x, int y, unordered_map<int, string>& cells)
{
	unordered_map<string, string> neighborhood;

	unordered_map<string, pair<int, int>> dxy(
		{
			{ "NW",{-1,-1} }, { "N",{0,-1} }, { "NE",{1,-1} },
			{ "W",{-1,0} }, { "C",{0,0} }, { "E",{1,0} },
			{ "SW",{-1,1} }, { "S",{0,1} }, { "SE",{1,1} },
		}
	);

	// iterate through neighbors and mark the state they have
	for (auto& neighbor : m_Neighbors)
	{
		pair<int, int> d = dxy[neighbor];

		int nx = x + d.first;
		int ny = y + d.second;

		if (InBounds(nx, ny)) neighborhood.insert({ neighbor,GetState(nx,ny, cells) });
	}

	return neighborhood;
}

bool AlgorithmOutput::ApplyOnCell(int x, int y, Transition& rule, unordered_map<int, string>& cells, unordered_set<string>& neighbors)
{
	unordered_map<string, string> neighborhood = GetNeighborhood(x, y, cells);

	//wxLogDebug("[CELL_NEIGHBORHOOD]");
	//for (auto& it : neighborhood) wxLogDebug("<%s>=%s", it.first, it.second);

	bool ruleValid = true;
	// iterate through the chain of "OR" rules
	for (auto& rulesOr : rule.orRules)
	{
		if (!m_Running) break;

		ruleValid = true;
		//wxLogDebug("[RULES_OR]");

		// iterate through the chain of "AND" rules
		for (auto& rulesAnd : rulesOr)
		{
			if (!m_Running) break;

			//wxLogDebug("[RULES_AND]");
			vector<string> ruleNeighborhood = rulesAnd.first;
			//wxLogDebug("[RULE_NEIGHBORHOOD]");
			//for (auto& it : ruleNeighborhood)wxLogDebug("<%s>", it);

			bool conditionValid = true;
			// iterate through the chain of "OR" conditions
			for (auto& conditionsOr : rulesAnd.second)
			{
				if (!m_Running) break;

				conditionValid = true;

				// iterate through the chain of "AND" conditions
				for (auto& conditionsAnd : conditionsOr)
				{
					if (!m_Running) break;

					string conditionState = conditionsAnd.second;
					//wxLogDebug("CONDITION_STATE=%s", conditionState);

					int occurences = 0;
					if (ruleNeighborhood[0] == "ALL")
					{
						for (auto& neighbor : neighborhood)
						{
							if (!m_Running) break;

							if (neighbor.second == conditionState) occurences++;
						}
					}
					else for (auto& neighbor : ruleNeighborhood)
					{
						if (!m_Running) break;

						if (neighbors.find(neighbor) != neighbors.end())
						{
							if (neighborhood[neighbor] == conditionState) occurences++;
						}
					}

					//wxLogDebug("OCCURENCES=%i", occurences);

					int conditionNumber = conditionsAnd.first.first;
					int conditionType = conditionsAnd.first.second;

					//wxLogDebug("CONDITION_NUMBER=%i, CONDITION_TYPE=%i", conditionNumber, conditionType);

					switch (conditionType)
					{
					case TYPE_EQUAL:
						if (occurences != conditionNumber) conditionValid = false;
						break;
					case TYPE_LESS:
						if (occurences >= conditionNumber) conditionValid = false;
						break;
					case TYPE_MORE:
						if (occurences <= conditionNumber) conditionValid = false;
						break;
					default:
						break;
					}
				}

				if (conditionValid) break;
			}

			if (!conditionValid)
			{
				ruleValid = false;
				break;
			}
		}

		if (ruleValid) break;
	}

	return ruleValid;
}

void AlgorithmOutput::UpdateGeneration(vector<pair<string, pair<int, int>>>& changes, vector<int>& pattern,
	unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions)
{
	for (auto& change : changes)
	{
		if (!m_Running) break;

		string state = change.first;

		//wxLogDebug("STATE=%s", state);

		state.pop_back();

		string prevState = "";
		string currState = "";
		bool separator = false;
		// regain information of previous and current state
		for (int i = 0; i < state.size() && m_Running; i++)
		{
			if (state[i] == '*')
			{
				separator = true;
				continue;
			}

			if (!separator) prevState.push_back(state[i]);
			else currState.push_back(state[i]);
		}

		//wxLogDebug("PREV=<%s> CURR=<%s>", prevState, currState);

		// insert positions into the current state map and remove them from the previous one
		auto position = change.second;
		int x = position.first;
		int y = position.second;
		int k = y * cols + x;

		if (prevState == "FREE")
		{
			if (currState != "FREE")
			{
				cells[k] = currState;
				statePositions[currState].insert(k);
				pattern[k] = find(m_States.begin(), m_States.end(), currState) - m_States.begin();
			}
		}
		else
		{
			if (currState == "FREE")
			{
				cells.erase(k);
				statePositions[prevState].erase(k);
				pattern[k] = 0;

				// there are no more cells of this state anymore -> remove it from our map
				if (statePositions[prevState].size() == 0)
				{
					statePositions.erase(prevState);
				}
			}
			else if (cells[k] != currState)
			{
				cells.erase(k);
				statePositions[prevState].erase(k);

				// there are no more cells of this state anymore -> remove it from our map
				if (statePositions[prevState].size() == 0)
				{
					statePositions.erase(prevState);
				}

				cells[k] = currState;
				statePositions[currState].insert(k);
				pattern[k] = find(m_States.begin(), m_States.end(), currState) - m_States.begin();
			}
		}
	}
}

void AlgorithmOutput::UpdateChromosomesMaps(vector<Chromosome>& population)
{
	for (int i = 0; i < popSize && m_Running; i++)
	{
		int initialSize = 0;
		unordered_map<int, string> cells;
		unordered_map<string, unordered_set<int>> statePositions;

		for (int j = 0; j < rows * cols && m_Running; j++)
		{
			int cellType = population[i].initialPattern[j];

			if (cellType)
			{
				initialSize++;
				cells[j] = m_States[cellType];
				statePositions[m_States[cellType]].insert(j);
			}
		}

		population[i].initialSize = initialSize;
		population[i].cells = cells;
		population[i].statePositions = statePositions;
	}
}

void AlgorithmOutput::ShowChromosomePattern(Chromosome& chromosome)
{
	wxLogDebug("--- Start Pattern ---");
	for (int i = 0; i < rows; i++)
	{
		string line = "";
		for (int j = 0; j < cols; j++)
		{
			int k = i * cols + j;
			line += to_string(chromosome.pattern[k]);
		}

		wxLogDebug("%s", line);
	}

	wxLogDebug("--- End Pattern ---");
}

void AlgorithmOutput::EndAlgorithm(bool save)
{
	m_Timer->Stop();

	m_Start->Enable();
	m_Stop->Disable();

	if (save && m_BestChromosome.id != -1) m_Save->Enable();

	m_Running = false;
}

void AlgorithmOutput::UpdateTimer(wxTimerEvent& evt)
{
	if (!m_Running) m_Timer->Stop();

	m_TimeElapsed++;

	UpdateTextElapsed(m_TimeElapsed);
}
