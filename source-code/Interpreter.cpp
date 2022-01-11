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
	string BOTH_SPACED = "|&,@();[]";

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

	while (false)
	{
		int chars = 0;
		Transition transition = {};

		string state1;
		string symbol;
		string state2;
		bool valid = true;

		// read transitory state
		ss >> state1;

		// check if end of file
		if (!FindWord(cursor, rules, state1)) break;
		//wxLogDebug("state1=%s", state1);
		// check if state is invalid
		if (!CheckState(state1)) MarkInvalid(valid, invalid, cursor);

		// check if rule is within the size limits
		if (valid && !UpdateChars(chars, state1)) MarkInvalid(valid, invalid, cursor);

		// read symbol "->"
		if (valid)
		{
			ss >> symbol; FindWord(cursor, rules, symbol);

			//wxLogDebug("symbol=%s", symbol);
			// check if it's the right symbol
			if (symbol != "->") MarkInvalid(valid, invalid, cursor);

			// check if rule is within the size limits
			if (valid && !UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);
		}

		// read transition state
		if (valid)
		{
			ss >> state2; FindWord(cursor, rules, state2);

			// check if state is invalid
			if (!CheckState(state2)) MarkInvalid(valid, invalid, cursor);

			// check if rule is within the size limits
			if (valid && !UpdateChars(chars, state2)) MarkInvalid(valid, invalid, cursor);

			if (valid)
			{
				// check if transition is a duplicate
				auto er = m_Transitions.equal_range(state1);
				for (auto it = er.first; it != er.second; it++)
				{
					// it's a duplicate
					if (it->second.state == state2)
					{
						MarkInvalid(valid, invalid, cursor);
						break;
					}
				}

				// assign to transition
				if (valid) transition.state = state2;
			}
		}

		// read symbol ":" or ";"
		if (valid)
		{
			ss >> symbol; FindWord(cursor, rules, symbol);

			// check if rule is within the size limits
			if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);

			if (valid)
			{
				// end of transition rules
				if (symbol == ";")
				{
					// check if rule is within the size limits
					if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);

					// add to transition table
					if (valid) m_Transitions.insert({ state1,transition });
				}
				// indicates that a list of rules will follow
				else if (symbol == ":")
				{
					transition.orRules.clear();
					
					// read every rule until ";" is detected
					while (valid)
					{
						transition.andRules.clear();

						ss >> symbol; FindWord(cursor, rules, symbol);

						// check if rule is marked accordingly with a "("
						if (symbol != "(") MarkInvalid(valid, invalid, cursor);

						// check if rule is within the size limits
						if (valid && !UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);

						// read neighborhood and conditions
						if (valid)
						{
							ss >> symbol; FindWord(cursor, rules, symbol);

							// check if symbol is "@"
							if (symbol != "@") MarkInvalid(valid, invalid, cursor);

							// check if rule is within the size limits
							if (valid && !UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);

							if (valid)
							{
								transition.andRules.push_back({});

								// could indicate either a group of directions or a specific one
								string neighborhood;
								ss >> neighborhood; FindWord(cursor, rules, neighborhood);

								if (valid && !UpdateChars(chars, neighborhood)) MarkInvalid(valid, invalid, cursor);

								// a group of directions, eg. "[n,s,w,e]"
								if (valid && neighborhood == "[")
								{
									NEIGHBORS neighbors;

									// read every direction until "]" is detected
									while (valid)
									{
										string direction;
										ss >> direction; FindWord(cursor, rules, direction);

										// check if rule is within the size limits
										if (valid && !UpdateChars(chars, direction)) MarkInvalid(valid, invalid, cursor);

										// check if direction is valid according to the current neighborhood
										if (valid && !CheckDirection(direction)) MarkInvalid(valid, invalid, cursor);

										if (valid) neighbors.push_back(direction);

										if (!valid) break;

										// expect to read either "," or "]"
										ss >> symbol; FindWord(cursor, rules, direction);

										// more directions to follow
										if (symbol == ",")
										{
											if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);
											continue;
										}
										// neighborhood completed
										else if (symbol == "]")
										{
											if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid, cursor);
											// check if neighborhood is within the size limit
											if (valid && neighbors.size() > m_Neighbors.size()) MarkInvalid(valid, invalid, cursor);

											// assign to transition
											if (valid) transition.andRules.back().first = neighbors;
										}
										// invalid symbol
										else
										{
											MarkInvalid(valid, invalid, cursor);
											break;
										}
									}
								}
								// a specific direction or "ALL" meaning every possible valid direction
								else if (valid && (neighborhood == "ALL" || CheckDirection(neighborhood)))
								{
									// assign to transition
									transition.andRules.back().first = { neighborhood };
								}
								// invalid token
								else if (valid) MarkInvalid(valid, invalid, cursor);

								if (!valid) break;

								// read assignment symbol "="
								ss >> symbol; FindWord(cursor, rules, symbol);

								if (valid && !UpdateChars(chars, neighborhood)) MarkInvalid(valid, invalid, cursor);

								if (!valid) break;

								int count = 0;

								// read conditions until ")" is detected
								while (valid)
								{
									// condition could be of different forms
									// eg. "<number>#<state>" / "<sign><number>#<state>"
									// or  "#<state>" / "<sign>#<state>"
									string condition;
									ss >> condition;

									if (!UpdateChars(chars, condition)) MarkInvalid(valid, invalid, cursor);

									if (!valid) break;

									// split by "#"
									stringstream tokenizer(condition);
									vector<string> tokens;
									string token;

									while (getline(tokenizer, token, '#')) tokens.push_back(token);

									// "#<state>"
									if (tokens.size() == 2)
									{
										string state = tokens.back();
									}
									// "<number>#<state>" or "<sign><number>#<state>" or "<sign>#<state>"
									else if (tokens.size() == 3)
									{
										string sign = "";
										string number = tokens.front();
										string state = tokens.back();

										// <sign> is present
										if (number.size() && (number[0] == '+' || number[0] == '-'))
										{
											sign.push_back(number[0]);
											number.erase(number.begin());

											// no number specified
											if (number.empty()) number.push_back('1');
										}

										// check if number is valid
										int n = CheckNumber(number, transition, count);
										if (n == -1) MarkInvalid(valid, invalid, cursor);

										// check if state is valid
										if (valid && !CheckState(state)) MarkInvalid(valid, invalid, cursor);

										// assign to transition
										if (valid)
										{
											count += n;
										}
									}
									// invalid tokens
									else MarkInvalid(valid, invalid, cursor);
								}
							}
						}
					}
				}
			}
		}
		
		//wxLogDebug("state1=<%s> symbol=<%s> state2=<%s> cursor=<%i>", state1, symbol, state2, cursor);

		// go to the next transition if there's any left
		if (!valid) NextTransition(cursor, rules, ss);
	}

	return invalid;
}

unordered_multimap<string, Transition> Interpreter::GetTransitions()
{
	return m_Transitions;
}

void Interpreter::SetStates(unordered_map<string, string>& states)
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

bool Interpreter::NextTransition(int& cursor, string& rules, stringstream& ss)
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

	return chars <= Sizes::CHARS_RULE_MAX;
}

bool Interpreter::UpdateSize(int& size)
{
	size++;

	return size <= Sizes::RULES_MAX;
}

void Interpreter::MarkInvalid(bool& valid,vector<int>& invalid, int& cursor)
{
	// mark it in our list
	invalid.push_back(cursor);

	valid = false;
}

bool Interpreter::CheckDirection(string& direction)
{
	return m_Neighbors.find(direction) != m_Neighbors.end();
}

int Interpreter::CheckNumber(string& number, Transition& transition, int& count)
{
	int n = -1;

	// convert to number if valid
	if (number.size() && number.find_first_not_of("0123456789") == string::npos) n = stoi(number);

	// check that it does not surpass the neighborhood size limit
	// also keep count of the total number tracked by the previous "AND" condition
	if (transition.andRules.back().first[0] == "C" && n + count > m_Neighbors.size()) n = -1;
	else if (transition.andRules.back().first[0] != "C" && n + count > transition.andRules.back().first.size()) n = -1;

	return n;
}
