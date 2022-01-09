#include "Interpreter.h"
#include "Sizes.h"

#include <sstream>
#include "wx/log.h"

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

vector<int> Interpreter::Process(string& rules)
{
	// keep count of duplicates/invalid rules
	vector<int> invalid;

	vector<string> AND = { ",", "&", "AND" };
	vector<string> OR = { "|", "OR" };

	string LEFT_SPACED = "-";
	string RIGHT_SPACED = ">";
	string BOTH_SPACED = "|&,@();";
	string NOT_SPACED = "[]";

	// add spaces around specific symbols and convert characters to uppercase
	for (int i = 0; i < rules.size(); i++)
	{
		char c = rules[i];

		if (BOTH_SPACED.find(c) != BOTH_SPACED.npos)
		{
			// add space at the both sides of the symbol
			rules.insert(i + 1, " ");
			rules.insert(i, " ");

			i += 2;
			continue;
		}
		if (LEFT_SPACED.find(c) != LEFT_SPACED.npos)
		{
			// add space before the symbol
			rules.insert(i, " ");

			i++;
			continue;
		}
		if (RIGHT_SPACED.find(c) != RIGHT_SPACED.npos)
		{
			// add space after the symbol
			rules.insert(i + 1, " ");

			i += 2;
			continue;
		}
		if (NOT_SPACED.find(c) != NOT_SPACED.npos)
		{
			continue;
		}

		if (isalnum(c))
		{
			// convert to uppercase
			rules[i] = toupper(rules[i]);

			continue;
		}

		// character is illegal
		invalid.push_back(i);
	}

	stringstream ss(rules);
	int cursor = 0;
	int size = 0;
	int chars = 0;

	while (true)
	{
		string state1;
		string symbol;
		string state2;
		bool valid = true;

		// read state1
		ss >> state1;
		// check if end of file
		if (ss.tellp() == -1)
		// check if state is invalid
		if (!CheckState(state1))
		{
			// mark it in our list
			invalid.push_back(ss.tellg());
			// go to the next transition or break the loop if there are no more
			if (!NextTransition(cursor, rules, ss)) break;

			valid = false;
		}

		// read symbol "->"
		if (valid)
		{
			ss >> symbol;
			// check if it's the right symbol
			if (symbol.compare("->"))
			{
				// mark it in our list
				invalid.push_back(cursor);
				// go to the next transition or break the loop if there are no more
				if (!NextTransition(cursor, rules, ss)) break;

				valid = false;
			}
		}

		// read state2
		if (valid)
		{
			ss >> state2;
			// check if state is invalid
			if (!CheckState(state1))
			{
				// mark it in our list
				invalid.push_back(cursor);
				// go to the next transition or break the loop if there are no more
				if (!NextTransition(cursor, rules, ss)) break;

				valid = false;
			}
		}

		// read symbol ":" or ";"
		if (valid)
		{
			ss >> symbol; FindWord(cursor, rules, symbol);

			if (symbol.compare(";")) continue;
		}
	}
	//wxLogDebug("state1=<%s> symbol=<%s> state2=<%s> cursor=<%i>", state1, symbol, state2, cursor);

	return invalid;
}

unordered_multimap<string, Transition> Interpreter::GetTransitionTable()
{
	return m_TransitionTable;
}

void Interpreter::SetStates(unordered_map<string, pair<string, wxColour>>& states)
{
	m_States = states;
}

void Interpreter::SetNeighbors(unordered_set<string>& neighbors)
{
	m_Neighbors = neighbors;
}

bool Interpreter::FindWord(int& cursor, string &rules, string& s)
{
	if (s.empty()) return false;

	int pos = rules.find(s, cursor);

	if (pos == rules.npos) return false;

	if (pos > 0 && rules[pos - 1] != ' ')
	{
		cursor = pos + s.size();
		return false;
	}
	if (pos + s.size() < rules.size() && rules[pos + s.size()] != ' ')
	{
		cursor = pos + s.size();
		return false;
	}

	cursor = pos + s.size();

	return true;
}

bool Interpreter::NextTransition(stringstream& ss)
{
	string semicolon = ";";

	// end of file
	if (!FindWord(cursor, rules, semicolon)) return false;

	// otherwise, update stream position
	ss.seekg(cursor);

	return true;
}

bool Interpreter::CheckState(string& state)
{
	return state.size() >= Sizes::CHARS_STATE_MIN
			&& state.size() <= Sizes::CHARS_STATE_MAX
			&& find_if(state.begin(), state.end(), [](char c) { return !(isalnum(c) || (c == ' ')); }) == state.end()
			&& m_States.find(state) != m_States.end();
}

bool Interpreter::UpdateChars(int& chars, string& s)
{
	chars += s.size();

	return chars >= Sizes::CHARS_RULE_MIN && chars <= Sizes::CHARS_RULE_MAX;
}

bool Interpreter::UpdateSize(int& size)
{
	size++;

	return size <= Sizes::RULES_MAX;
}
