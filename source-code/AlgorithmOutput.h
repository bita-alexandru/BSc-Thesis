#pragma once
#include "wx/wx.h"

#include "Grid.h"
#include "InputStates.h"
#include "InputRules.h"
#include "InputNeighbors.h"
#include "Chromosome.h"

class AlgorithmOutput : public wxPanel
{
public:
	AlgorithmOutput(wxWindow* parent);
	~AlgorithmOutput();

	void SetGrid(Grid* grid);
	void SetInputStates(InputStates* inputStates);
	void SetInputRules(InputRules* inputRules);
	void SetInputNeighbors(InputNeighbors* inputNeighbors);
private:
	Grid* m_Grid = nullptr;

	wxButton* m_Start = nullptr;
	wxButton* m_Stop = nullptr;
	wxButton* m_Save = nullptr;
	wxStaticText* m_TextEpoch = nullptr;
	wxStaticText* m_TextFitness = nullptr;
	wxStaticText* m_TextElapsed = nullptr;
	wxStaticText* m_TextLastPopulation = nullptr;
	wxStaticText* m_TextLastGeneration = nullptr;
	wxStaticText* m_TextLastInitial = nullptr;
	wxStaticText* m_TextLastFitness = nullptr;

	InputStates* m_InputStates = nullptr;
	InputRules* m_InputRules = nullptr;
	InputNeighbors* m_InputNeighbors = nullptr;

	vector<string> m_States;
	vector<string> m_Rules;
	vector<string> m_Neighbors;

	bool m_RenderOnScreen;
	bool m_Running;

	int m_Epoch;
	int m_TimeElapsed;

	double m_LastPopulation;
	double m_LastGeneration;
	double m_LastInitial;
	double m_LastFitness;
	double m_BestFitness;

	int n;
	int rows;
	int cols;
	double pc;
	double pm;
	Chromosome m_BestChromosome;

	void BuildInterface();
	void RunAlgorithm();

	vector<Chromosome> InitializePopulation();
	void EvaluatePopulation(vector<Chromosome>& population, unordered_map<string, string>& states,
		unordered_multimap<string, Transition>& rules, unordered_set<string>& neighbors);
	vector<Chromosome> SelectPopulation(vector<Chromosome>& population);
	void DoCrossover(vector<Chromosome>& population);
	void DoMutatiton(vector<Chromosome>& population);
	Chromosome GetBestChromosome(vector<Chromosome>& population);

	void OnStart(wxCommandEvent& evt);
	void OnStop(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);
	void OnRender(wxCommandEvent& evt);

	void Start();
	void Stop();
	void Save();

	void UpdateTextEpoch(int epoch);
	void UpdateTextFitness(double fitness);
	void UpdateTextElapsed(int elapsed);
	void UpdateTextLast(Chromosome chromosome);

	pair<vector<pair<string, pair<int, int>>>, string> ParseAllRules(
		unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions,
		unordered_map<string, string>& states, unordered_multimap<string, Transition>& rules, unordered_set<string>& neighbors);
	pair<vector<pair<int, int>>, string> ParseRule(pair<const string, Transition>& rule,
		unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions,
		unordered_map<string, string>& states, unordered_set<string>& neighbors);

	string GetState(int x, int y, unordered_map<int, string>& cells);
	bool InBounds(int x, int y);
	unordered_map<string, string> GetNeighborhood(int x, int y, unordered_map<int, string>& cells);
	bool ApplyOnCell(int x, int y, Transition& rule, unordered_map<int, string>& cells, unordered_set<string>& neighbors);
	void UpdateGeneration(vector<pair<string, pair<int, int>>>& changes, vector<int>& pattern,
		unordered_map<int, string>& cells, unordered_map<string, unordered_set<int>>& statePositions);

	void UpdateChromosomesMaps(vector<Chromosome>& population);
	void ShowChromosomePattern(Chromosome& chromosome);
};
