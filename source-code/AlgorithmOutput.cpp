#include "AlgorithmOutput.h"

#include <thread>
#include <fstream>

AlgorithmOutput::AlgorithmOutput(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

AlgorithmOutput::~AlgorithmOutput()
{
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

void AlgorithmOutput::BuildInterface()
{
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
	sizerButtons->Add(m_Stop, 0, wxLEFT | wxRIGHT, 4);
	sizerButtons->Add(m_Save, 0);

	wxCheckBox* checkRender = new wxCheckBox(this, wxID_ANY, "Render on Screen");
	checkRender->Bind(wxEVT_CHECKBOX, &AlgorithmOutput::OnRender, this);

	m_TextEpoch = new wxStaticText(this, wxID_ANY, "Epoch: -");

	m_TextLastGeneration = new wxStaticText(this, wxID_ANY, "-");
	m_TextLastPopulation = new wxStaticText(this, wxID_ANY, "-");
	m_TextLastInitial = new wxStaticText(this, wxID_ANY, "-");
	m_TextLastFitness = new wxStaticText(this, wxID_ANY, "-");

	wxFlexGridSizer* sizerLast = new wxFlexGridSizer(2, 4, wxSize(24, 0));
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last #Generation"), 0, wxALIGN_RIGHT);
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last #Population"), 0, wxALIGN_RIGHT);
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last Initial Size"), 0, wxALIGN_RIGHT);
	sizerLast->Add(new wxStaticText(this, wxID_ANY, "Last Fitness"), 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastPopulation, 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastGeneration, 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastInitial, 0, wxALIGN_RIGHT);
	sizerLast->Add(m_TextLastFitness, 0, wxALIGN_RIGHT);

	m_TextFitness = new wxStaticText(this, wxID_ANY, "Best Fitness: -");
	m_TextElapsed = new wxStaticText(this, wxID_ANY, "Time Elapsed: -");

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(sizerButtons, 0);
	sizer->AddSpacer(4);
	sizer->Add(checkRender, 0);
	sizer->AddSpacer(8);
	sizer->Add(m_TextEpoch, 0);
	sizer->AddSpacer(8);
	sizer->Add(sizerLast, 0);
	sizer->AddSpacer(8);
	sizer->Add(m_TextFitness, 0);
	sizer->AddSpacer(8);
	sizer->Add(m_TextElapsed, 0);

	SetSizerAndFit(sizer);
}

void AlgorithmOutput::RunAlgorithm()
{
	m_States = m_InputStates->GetList()->GetStates();
	m_Rules = m_InputRules->GetList()->GetRules();
	for (int i = 0; i < m_Rules.size(); i++)
	{
		wxLogDebug("RULES %i=%s", i, m_Rules[i]);
	}
	m_Neighbors = m_InputNeighbors->GetNeighborsAsVector();

	unordered_map<string, string> states = m_InputStates->GetStates();
	unordered_multimap<string, Transition> rules = m_InputRules->GetRules();
	unordered_set<string> neighbors = m_InputNeighbors->GetNeighbors();

	n = 30;
	rows = Sizes::N_ROWS;
	cols = Sizes::N_COLS;
	pc = 0.25;
	pm = 0.01;

	srand(time(NULL));

	vector<Chromosome> population = InitializePopulation();
	EvaluatePopulation(population, states, rules, neighbors);

	Chromosome bestChromosome = GetBestChromosome(population);

	int epochs = 0;
	while (epochs++ < 1000)
	{
		wxLogDebug("Epoch %i", epochs);

		SelectPopulation(population);
		DoCrossover(population);
		DoMutatiton(population);

		EvaluatePopulation(population, states, rules, neighbors);
		bestChromosome = GetBestChromosome(population);
	}

	m_BestChromosome = bestChromosome;

	m_Start->Enable();
	m_Stop->Disable();
	m_Save->Enable();
}

vector<Chromosome> AlgorithmOutput::InitializePopulation()
{
	wxLogDebug("Init pop [start]");
	vector<Chromosome> population;

	for (int i = 0; i < n; i++)
	{
		double cellProbability = (rand() % 8 + 1) * 0.1;

		vector<int> pattern(rows * cols);
		unordered_map<int, string> cells;
		unordered_map<string, unordered_set<int>> statePositions;
		int initialSize = 0;

		for (int j = 0; j < rows * cols; j++)
		{
			double p = (rand() % 101) * 0.01;

			if (p <= cellProbability)
			{
				int cellType = rand() % (m_States.size() - 1) + 1;

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
		chromosome.generation = 0;
		chromosome.fitness = 0.0;

		wxLogDebug("Generated chromosome %i with size %i", i, initialSize);
		population.push_back(chromosome);
	}

	wxLogDebug("Init pop [end]");
	return population;
}

void AlgorithmOutput::EvaluatePopulation(vector<Chromosome>& population, unordered_map<string, string>& states,
	unordered_multimap<string, Transition>& rules, unordered_set<string>& neighbors)
{
	wxLogDebug("Evaluate pop [start]");

	const int TARGET_GENERATION = 10;

	for (int i = 0; i < n; i++)
	{
		wxLogDebug("Chromosome %i", population[i].id);

		int generation = 0;
		int popsize = 0;
		while (++generation)
		{
			wxLogDebug("Generation %i", generation);

			pair<vector<pair<string, pair<int, int>>>, string> result = 
				ParseAllRules(population[i].cells, population[i].statePositions, states, rules, neighbors);

			if (result.second.size())
			{
				wxLogDebug("error");
				break;
			}

			UpdateGeneration(result.first, population[i].cells, population[i].statePositions);

			if (result.first.empty())
			{
				wxLogDebug("End of universe");
				if (population[i].cells.size() > popsize) popsize = population[i].cells.size();

				break;
			}

			if (generation == TARGET_GENERATION) break;
		}

		double fitness = 1.0 * generation;// +0.0 * popsize - 1.0 * population[i].initialSize;
		wxLogDebug("Fitness: %f", fitness);

		population[i].fitness = fitness;
	}

	wxLogDebug("Evaluate pop [end]");
}

vector<Chromosome> AlgorithmOutput::SelectPopulation(vector<Chromosome>& population)
{
	wxLogDebug("Select pop [start]");

	double totalFitness = 0.0;
	for (int i = 0; i < n; i++) totalFitness += population[i].fitness;

	wxLogDebug("Total fitness: %f", totalFitness);

	vector<double> q(n + 1);
	for (int i = 0; i < n; i++)
	{
		wxLogDebug("%i. Chromosome %i", population[i].id);
		double p = population[i].fitness / totalFitness;
		q[i + 1] = q[i] + p;

		wxLogDebug("p=%f q=%f", p, q[i + 1]);
	}

	vector<Chromosome> newPopulation;
	int j = 0;
	while (j != n)
	{
		for (int i = 0; i < n; i++)
		{
			double p = (rand() % 10001) * 0.0001;
			//wxLogDebug("%i. Generated probability: %f", i, p);

			if (p >= q[j] && p < q[j + 1])
			{
				wxLogDebug("Selected into new pop: %i", population[i].id);

				j++;

				Chromosome chromosome = population[i];
				chromosome.id = j;

				newPopulation.push_back(chromosome);

				if (j == n) break;
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

	for (int i = 0; i < n; i++)
	{
		double p = (rand() % 1000) * 0.001;

		//wxLogDebug("%i. Generated probability: %f", i, p);

		if (p < pc)
		{
			wxLogDebug("Selecting parent %i", population[i].id);
			parents.push_back(i);
			parentsSize++;
		}
	}

	const int N = rows * cols;

	for (int i = 0; i < parentsSize - 1; i += 2)
	{
		int xp1 = rand() % N;
		int xp2 = rand() % N;

		while (xp1 == xp2 && N > 1)
		{
			xp2 = rand() % N;
		}

		if (xp1 > xp2) swap(xp1, xp2);

		wxLogDebug("Generated cutpoints: %i, %i", xp1, xp2);

		for (int j = xp1; j <= xp2; j++)
		{
			int p1 = parents[i];
			int p2 = parents[i + 1];

			//wxLogDebug("Swapping {%i}[%i](=%i) with {%i}[%i](=%i)",
				//population[p1].id, j, population[p1].initialPattern[j], population[p2].id, j, population[p2].initialPattern[j]);
			swap(population[p1].initialPattern[j], population[p2].initialPattern[j]);
		}
	}

	wxLogDebug("Cross-over [end]");
}

void AlgorithmOutput::DoMutatiton(vector<Chromosome>& population)
{
	wxLogDebug("Mutation [start]");

	const int N = rows * cols;

	for (int i = 0; i < n; i++)
	{
		wxLogDebug("%i. Chromosome %i", i, population[i].id);
		for (int j = 0; j < N; j++)
		{
			double p = (rand() % 1001) * 0.001;
			//wxLogDebug("Generated probability: %f", p);

			if (p <= pm)
			{
				int cellType = rand() % m_States.size();

				wxLogDebug("Changing [%i](=%i) with %i", j, population[i].initialPattern[j], cellType);
				population[i].initialPattern[j] = cellType;
			}
		}
	}

	wxLogDebug("Mutation [end]");
}

Chromosome AlgorithmOutput::GetBestChromosome(vector<Chromosome>& population)
{
	wxLogDebug("Get best [start]");
	Chromosome chromosome = population[0];

	for (int i = 1; i < n; i++)
	{
		if (population[i] > chromosome)
		{
			wxLogDebug("New best %i with fitness %f", population[i].id, population[i].fitness);
			chromosome = population[i];
		}
	}

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

	thread t(&AlgorithmOutput::RunAlgorithm, this);
	t.detach();

	m_Running = true;

	m_Start->Disable();
	m_Stop->Enable();
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
	m_TextEpoch->SetLabel(to_string(epoch));
}

void AlgorithmOutput::UpdateTextFitness(double fitness)
{
	m_TextEpoch->SetLabel(to_string(fitness));
}

void AlgorithmOutput::UpdateTextElapsed(int elapsed)
{
	m_TextEpoch->SetLabel(to_string(elapsed));
}

void AlgorithmOutput::UpdateTextLast(Chromosome chromosome)
{
	m_TextLastFitness->SetLabel(to_string(chromosome.fitness));
	m_TextLastGeneration->SetLabel(to_string(chromosome.generation));
	m_TextLastPopulation->SetLabel(to_string(chromosome.population));
	m_TextLastInitial->SetLabel(to_string(chromosome.initialSize));
}

pair<vector<pair<string, pair<int, int>>>, string> AlgorithmOutput::ParseAllRules(
	unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions,
	unordered_map<string, string>& states, unordered_multimap<string, Transition>& rules, unordered_set<string>& neighbors
)
{
	vector<pair<string, pair<int, int>>> changes;

	for (auto& rule : rules)
	{
		pair<vector<pair<int, int>>, string> result = ParseRule(rule, cells, statePositions, states, neighbors);

		//wxLogDebug("RULE=%s/%s:%s", rule.first, rule.second.state, rule.second.condition);

		// error
		if (result.second.size())
		{
			int index = -1;

			string wholeRule = rule.first + "/" + rule.second.state;
			if (rule.second.condition.size()) wholeRule += rule.second.condition;
			wholeRule += ";";

			// mark the problematic rule index
			for (int i = 0; i < m_Rules.size(); i++)
			{
				if (wholeRule == m_Rules[i])
				{
					index = i;
					break;
				}
			}

			if (index != -1) result.second += " at rule number " + to_string(index + 1);

			return { {}, result.second };
		}
		// concatenate changes
		else
		{
			string newstate = rule.first + "*" + rule.second.state + "*";

			for (auto& change : result.first)
			{
				changes.push_back({ newstate , change });
			}
		}
	}

	return { changes,"" };
}

pair<vector<pair<int, int>>, string> AlgorithmOutput::ParseRule(pair<const string, Transition>& rule,
	unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions,
	unordered_map<string, string>& states, unordered_set<string>& neighbors)
{
	vector<pair<int, int>> applied;

	// check if rule might contain invalid states
	if (states.find(rule.first) == states.end()) return { {}, "<INVALID FIRST STATE>" };
	if (states.find(rule.second.state) == states.end()) return { {}, "<INVALID SECOND STATE>" };
	for (auto& state : rule.second.states)
	{
		if (states.find(state) == states.end()) return { {}, "<INVALID CONDITION STATE>" };
	}
	// check for neighborhood as well
	for (auto& direction : rule.second.directions)
	{
		if (neighbors.find(direction) == neighbors.end()) return { {}, "<INVALID NEIGHBORHOOD>" };
	}

	// if state is "FREE", apply rule to all "FREE" cells
	if (rule.first == "FREE")
	{
		// iterate through all cells
		if (rule.second.all || rule.second.condition.empty())
		{
			//wxLogDebug("FREE_ALL");
			const int N = rows * cols;

			for (int i = 0; i < N; i++)
			{
				int x = i % rows;
				int y = i / cols;

				if (GetState(x, y, cells) == "FREE" && ApplyOnCell(x, y, rule.second, cells, neighbors))
				{
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
				if (statePositions.find(state) == statePositions.end()) continue;
				n2 += statePositions[state].size();
			}
			//wxLogDebug("N1=%i N2=%i",n1,n2);

			// faster to iterate through all cells
			if (n1 <= n2 || rule.second.condition.empty())
			{
				const int N = rows * cols;

				for (int i = 0; i < N; i++)
				{
					int x = i % rows;
					int y = i / cols;

					if (GetState(x, y, cells) == "FREE" && ApplyOnCell(x, y, rule.second, cells, neighbors))
					{
						applied.push_back({ x,y });
					}
				}
			}
			// faster to iterate through the condition states' neighbors
			else
			{
				for (auto& state : rule.second.states)
				{
					if (state == "FREE")
					{
						const int N = rows * cols;

						for (int i = 0; i < N; i++)
						{
							int x = i % rows;
							int y = i / cols;

							if (GetState(x, y, cells) == "FREE" && ApplyOnCell(x, y, rule.second, cells, neighbors))
							{
								applied.push_back({ x,y });
							}
						}
					}
					// cells of this type are placed on grid
					else if (statePositions.find(state) != statePositions.end())
					{
						int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
						int dy[8] = { -1,-1,0,1,1,1,0,-1 };

						for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
						{
							int k = *i;
							int x = k % cols;
							int y = k / cols;

							for (int d = 0; d < 8; d++)
							{
								int nx = x + dx[d];
								int ny = y + dy[d];

								if (InBounds(nx, ny) && GetState(nx, ny, cells) == rule.first && ApplyOnCell(nx, ny, rule.second, cells, neighbors)) applied.push_back({ nx,ny });
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
			for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end();i++)
			{
				int k = *i;
				int x = k % cols;
				int y = k / cols;

				if (ApplyOnCell(x, y, rule.second, cells, neighbors)) applied.push_back({ x,y });
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
				if (statePositions.find(state) == statePositions.end()) continue;
				n2 += statePositions[state].size();
			}

			// faster to iterate through all cells
			if (n1 <= n2 || rule.second.condition.empty())
			{
				string state = rule.first;
				for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end();i++)
				{
					int k = *i;
					int x = k % cols;
					int y = k / cols;

					if (ApplyOnCell(x, y, rule.second, cells, neighbors)) applied.push_back({ x,y });
				}
			}
			// faster to iterate through the condition states' neighbors
			else
			{
				for (auto& state : rule.second.states)
				{
					if (state == "FREE")
					{
						string state = rule.first;
						for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end();i++)
						{
							int k = *i;
							int x = k % cols;
							int y = k / cols;

							if (ApplyOnCell(x, y, rule.second, cells, neighbors)) applied.push_back({ x,y });
						}
					}
					// cells of this type are placed on grid
					else if (statePositions.find(state) != statePositions.end())
					{
						int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
						int dy[8] = { -1,-1,0,1,1,1,0,-1 };

						for (auto i = statePositions.at(state).begin(); i != statePositions.at(state).end(); i++)
						{
							int k = *i;
							int x = k % cols;
							int y = k / cols;

							for (int d = 0; d < 8; d++)
							{
								int nx = x + dx[d];
								int ny = y + dy[d];

								if (InBounds(nx, ny) && GetState(nx, ny, cells) == rule.first && ApplyOnCell(nx, ny, rule.second, cells, neighbors)) applied.push_back({ nx,ny });
							}
						}
					}
				}
			}
		}
	}

	return { applied, "" };
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
		ruleValid = true;
		//wxLogDebug("[RULES_OR]");

		// iterate through the chain of "AND" rules
		for (auto& rulesAnd : rulesOr)
		{
			//wxLogDebug("[RULES_AND]");
			vector<string> ruleNeighborhood = rulesAnd.first;
			//wxLogDebug("[RULE_NEIGHBORHOOD]");
			//for (auto& it : ruleNeighborhood)wxLogDebug("<%s>", it);

			bool conditionValid = true;
			// iterate through the chain of "OR" conditions
			for (auto& conditionsOr : rulesAnd.second)
			{
				conditionValid = true;

				// iterate through the chain of "AND" conditions
				for (auto& conditionsAnd : conditionsOr)
				{
					string conditionState = conditionsAnd.second;
					//wxLogDebug("CONDITION_STATE=%s", conditionState);

					int occurences = 0;
					if (ruleNeighborhood[0] == "ALL")
					{
						for (auto& neighbor : neighborhood)
							if (neighbor.second == conditionState) occurences++;
					}
					else for (auto& neighbor : ruleNeighborhood)
					{
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

void AlgorithmOutput::UpdateGeneration(vector<pair<string, pair<int, int>>>& changes,
	unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions)
{
	for (auto& change : changes)
	{
		string state = change.first;

		//wxLogDebug("STATE=%s", state);

		state.pop_back();

		string prevState = "";
		string currState = "";
		bool separator = false;
		// regain information of previous and current state
		for (int i = 0; i < state.size(); i++)
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
			}
		}
		else
		{
			if (cells[k] != currState)
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
			}
		}
	}
}
