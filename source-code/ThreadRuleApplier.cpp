#include "ThreadRuleApplier.h"
#include "Sizes.h"

ThreadRuleApplier::ThreadRuleApplier(std::string mode) : wxThread(wxTHREAD_JOINABLE)
{
    m_Mode = mode;
}

ThreadRuleApplier::~ThreadRuleApplier()
{
    wxDELETE(m_Applied);
    wxDELETE(m_AppliedCS);

    wxDELETE(m_Visited);
    wxDELETE(m_VisitedCS);

    wxDELETE(m_StatePositions);
    wxDELETE(m_StatePositionsCS);

    wxDELETE(m_Rule);
    wxDELETE(m_Cells);
}

void ThreadRuleApplier::SetRange(int rowStart, int rowSize, int colStart, int colSize)
{
    m_RowStart = rowStart;
    m_RowSize = rowSize;
    m_ColStart = colStart;
    m_ColSize = colSize;
}

void ThreadRuleApplier::SetRange(int itStart, int itSize)
{
    m_itStart = itStart;
    m_itSize = itSize;
}

void ThreadRuleApplier::SetRule(std::pair<const std::string, Transition>* rule)
{
    m_Rule = rule;
}

void ThreadRuleApplier::SetState(std::string state)
{
    m_State = state;
}

void ThreadRuleApplier::SetNeighbors(std::unordered_set<std::string>* neighbors)
{
    m_Neighbors = neighbors;
}

void ThreadRuleApplier::SetApplied(std::vector<std::pair<int, int>>* applied)
{
    m_Applied = applied;
}

void ThreadRuleApplier::SetVisited(std::unordered_set<std::pair<int, int>, Hashes::PairInt>* visited)
{
    m_Visited = visited;
}

void ThreadRuleApplier::SetCells(std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>* cells)
{
    m_Cells = cells;
}

void ThreadRuleApplier::SetStatePositions(std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>* statePositions)
{
    m_StatePositions = statePositions;
}

void ThreadRuleApplier::SetCriticalApplied(wxCriticalSection* appliedCS)
{
    m_AppliedCS = appliedCS;
}

void ThreadRuleApplier::SetCriticalVisited(wxCriticalSection* visitedCS)
{
    m_VisitedCS = visitedCS;
}

void ThreadRuleApplier::SetCriticalStatePosition(wxCriticalSection* statePositionsCS)
{
    m_StatePositionsCS = statePositionsCS;
}

wxThread::ExitCode ThreadRuleApplier::Entry()
{
    if (m_Mode == "FREE_ONLY")
    {
        int applies = 0;

        for (int i = m_RowStart; i < m_RowStart + m_RowSize && i < Sizes::N_ROWS; i++)
            for (int j = m_ColStart; j < m_ColStart + m_ColSize && j < Sizes::N_COLS; j++)
            {
                if (TestDestroy()) return (ExitCode)applies;

                if (m_Cells->find({ j,i }) == m_Cells->end())
                {
                    std::string newstate = m_Rule->first + "*" + m_Rule->second.state + "*";

                    wxCriticalSectionLocker lock(*m_StatePositionsCS);
                    m_StatePositions->at(newstate).insert({ j,i });

                    applies++;
                }
            }

        return (ExitCode)applies;
    }

    if (m_Mode == "FREE_FREE")
    {
        for (int i = m_RowStart; i < m_RowStart + m_RowSize && i < Sizes::N_ROWS; i++)
            for (int j = m_ColStart; j < m_ColStart + m_ColSize && j < Sizes::N_COLS; j++)
            {
                if (TestDestroy()) return 0;

                if (GetState(j, i) == "FREE")
                {
                    wxCriticalSectionLocker lock(*m_VisitedCS);
                    if (m_Visited->find({ j,i }) == m_Visited->end())
                    {
                        m_Visited->insert({ j,i });

                        if (ApplyOnCell(j, i))
                        {
                            wxCriticalSectionLocker lock(*m_AppliedCS);
                            m_Applied->push_back({ j,i });
                        }
                    }
                }
            }

        return 0;
    }

    if (m_Mode == "FREE_ADJACENT")
    {
        int dx[8] = { 0,1,1,1,0,-1,-1,-1 };
        int dy[8] = { -1,-1,0,1,1,1,0,-1 };

        // get adjacent cells of type "FREE"
        auto it = m_StatePositions->at(m_State).begin();
        int size = m_StatePositions->at(m_State).size();
        int cnt = 0;
        while (cnt != m_itStart && cnt != size)
        {
            it++;
            cnt++;
        }

        while (m_itSize--)
        {
            auto& cell = *it;

            int x = cell.first;
            int y = cell.second;

            for (int d = 0; d < 8; d++)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];

                // valid position and unvisited yet
                if (InBounds(nx, ny) && GetState(nx, ny) == "FREE")
                {
                    wxCriticalSectionLocker lock(*m_VisitedCS);
                    if (m_Visited->find({ nx,ny }) == m_Visited->end())
                    {
                        m_Visited->insert({ nx,ny });

                        if (ApplyOnCell(nx, ny))
                        {
                            wxCriticalSectionLocker lock(*m_AppliedCS);
                            m_Applied->push_back({ nx,ny });
                        }
                    }
                }
            }
        }

        return 0;
    }

    if (m_Mode == "NON_FREE")
    {
        auto it = m_StatePositions->at(m_State).begin();
        int size = m_StatePositions->at(m_State).size();
        int cnt = 0;
        while (cnt != m_itStart && cnt != size)
        {
            it++;
            cnt++;
        }

        while(m_itSize--)
        {
            auto& cell = *it;

            //wxLogDebug("CELL=%i,%i", cell.first-m_OffsetX, cell.second-m_OffsetY);
            if (ApplyOnCell(cell.first, cell.second))
            {
                wxCriticalSectionLocker lock(*m_AppliedCS);
                m_Applied->push_back({ cell.first,cell.second });
            }
        }

        return 0;
    }

    return 0;
}

bool ThreadRuleApplier::ApplyOnCell(int x, int y)
{
    std::unordered_map<std::string, std::string> neighborhood = GetNeighborhood(x, y);

    //wxLogDebug("[CELL_NEIGHBORHOOD]");
    //for (auto& it : neighborhood) wxLogDebug("<%s>=%s", it.first, it.second);

    bool ruleValid = true;
    // iterate through the chain of "OR" rules
    for (auto& rulesOr : m_Rule->second.orRules)
    {
        ruleValid = true;
        //wxLogDebug("[RULES_OR]");

        // iterate through the chain of "AND" rules
        for (auto& rulesAnd : rulesOr)
        {
            //wxLogDebug("[RULES_AND]");
            std::vector<std::string> ruleNeighborhood = rulesAnd.first;
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
                    std::string conditionState = conditionsAnd.second;
                    //wxLogDebug("CONDITION_STATE=%s", conditionState);

                    int occurences = 0;
                    if (ruleNeighborhood[0] == "ALL")
                    {
                        for (auto& neighbor : neighborhood)
                            if (neighbor.second == conditionState) occurences++;
                    }
                    else for (auto& neighbor : ruleNeighborhood)
                    {
                        if (m_Neighbors->find(neighbor) != m_Neighbors->end())
                        {
                            if (neighborhood[neighbor] == conditionState) occurences++;
                        }
                        // otherwise, maybe throw error; to do: decide (line 1434)
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

bool ThreadRuleApplier::InBounds(int x, int y)
{
    return (x >= 0 && x < Sizes::N_COLS&& y >= 0 && y < Sizes::N_ROWS);
}

std::string ThreadRuleApplier::GetState(int x, int y)
{
    if (m_Cells->find({ x,y }) == m_Cells->end()) return "FREE";

    return m_Cells->at({x, y}).first;
}

std::unordered_map<std::string, std::string> ThreadRuleApplier::GetNeighborhood(int x, int y)
{
    std::unordered_map<std::string, std::string> neighborhood;
    std::unordered_map<std::string, std::pair<int, int>> dxy(
        {
            { "NW",{-1,-1} }, { "N",{0,-1} }, { "NE",{1,-1} },
            { "W",{-1,0} }, { "C",{0,0} }, { "E",{1,0} },
            { "SW",{-1,1} }, { "S",{0,1} }, { "SE",{1,1} },
        }
    );

    // iterate through neighbors and mark the state they have
    for (auto& neighbor : *m_Neighbors)
    {
        std::pair<int, int> d = dxy[neighbor];

        int nx = x + d.first;
        int ny = y + d.second;

        if (InBounds(nx, ny)) neighborhood.insert({ neighbor,GetState(nx,ny) });
    }

    return neighborhood;
}
