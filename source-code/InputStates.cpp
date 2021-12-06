#include "InputStates.h"

#include <algorithm>
#include <random>
#include <chrono>
#include <unordered_set>

#define cout(x) wxMessageBox(x, "debug")

InputStates::InputStates(wxWindow* parent) : wxPanel(parent)
{
    BuildInterface();

    InitializeColors();
}

InputStates::~InputStates()
{
	
}

wxListView* InputStates::GetList()
{
	return m_List;
}

std::unordered_map<std::string, std::string>& InputStates::GetStates()
{
	return m_States;
}

void InputStates::SetStates(std::vector<std::string> states)
{
    // states appear in our map but not in the given list -> they got recently deleted
    for (auto it = m_States.begin(); it != m_States.end();)
    {
        //cout(it->first);
        if (std::find(states.begin(), states.end(), it->first) == states.end())
        {
            //cout("deleted");
            MakeColorAvailable(it->second);
            it = m_States.erase(it);
        }
        else
        {
            it++;
        }
    }

    // states appear in the given list but not in our map -> they got recently introduced
    for (int i = 0; i < states.size(); i++)
    {
        if (m_States.find(states[i]) == m_States.end())
        {
            m_States.insert({ states[i], m_Colors[0]});
            MakeColorUnavailable(m_Colors[0]);
        }
    }

    // update list display
    int nOfItems = m_List->GetItemCount();
    int i = 0;
    for (i = 0; i < states.size(); i++)
    {
        wxString id = std::to_string(i);
        wxString state = states[i];
        wxColour color = wxColour(m_States[states[i]]);
        wxColour blackwhite = (color.Red() * 0.299 + color.Green() * 0.587 + color.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");

        if (i > nOfItems - 1)
        {
            m_List->InsertItem(i, id);
            m_List->SetItem(i, 1, state);
            m_List->SetItemBackgroundColour(i, color);
            m_List->SetItemTextColour(i, blackwhite);
            m_List->SetItemFont(i, m_Font);

            continue;
        }

        wxString itmId = m_List->GetItemText(i, 0);
        wxString itmState = m_List->GetItemText(i, 1);
        wxColour itmColor = m_List->GetItemBackgroundColour(i);

        if (itmId != id)
        {
            m_List->SetItem(i, 0, id);
        }
        if (itmState != state)
        {
            m_List->SetItem(i, 1, state);
        }
        if (itmColor != color)
        {
            m_List->SetItemBackgroundColour(i, color);
            m_List->SetItemTextColour(i, blackwhite);
        }
    }

    while (i < nOfItems--)
    {
        m_List->DeleteItem(i);
    }

    // update ToolModes (state, color) list
    std::vector<std::pair<std::string, wxColour>> statesColors;
    for (auto it : states)
    {
        statesColors.push_back({ it, wxColour(m_States[it]) });
    }

    m_ToolStates->SetStates(statesColors);
}

void InputStates::SetToolStates(ToolStates* toolStates)
{
    m_ToolStates = toolStates;
}

void InputStates::BuildInterface()
{
    wxButton* button = new wxButton(this, Ids::ID_EDIT_STATES, wxString("Edit States"));

    m_List = new wxListView(this, Ids::ID_LIST_STATES, wxDefaultPosition, wxSize(128, 128));
    m_List->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
    m_List->AppendColumn("State");

    m_List->InsertItem(0, "0");
    m_List->SetItem(0, 1, "FREE");
    m_List->SetItemBackgroundColour(0, wxColour("white"));
    m_List->SetItemFont(0, m_Font);

    wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, "States");
    sizer->Add(button, 0, wxEXPAND);
    sizer->Add(m_List, 1, wxEXPAND);

    this->SetSizer(sizer);
}

void InputStates::InitializeColors()
{
    m_Colors = std::deque<std::string>({
        "#000000", "#FFFF00", "#1CE6FF", "#FF34FF", "#FF4A46", "#008941", "#006FA6", "#A30059",
        "#FFDBE5", "#7A4900", "#0000A6", "#63FFAC", "#B79762", "#004D43", "#8FB0FF", "#997D87",
        "#5A0007", "#809693", "#FEFFE6", "#1B4400", "#4FC601", "#3B5DFF", "#4A3B53", "#FF2F80",
        "#61615A", "#BA0900", "#6B7900", "#00C2A0", "#FFAA92", "#FF90C9", "#B903AA", "#D16100",
        "#DDEFFF", "#000035", "#7B4F4B", "#A1C299", "#300018", "#0AA6D8", "#013349", "#00846F",
        "#372101", "#FFB500", "#C2FFED", "#A079BF", "#CC0744", "#C0B9B2", "#C2FF99", "#001E09",
        "#00489C", "#6F0062", "#0CBD66", "#EEC3FF", "#456D75", "#B77B68", "#7A87A1", "#788D66",
        "#885578", "#FAD09F", "#FF8A9A", "#D157A0", "#BEC459", "#456648", "#0086ED", "#886F4C",
        "#34362D", "#B4A8BD", "#00A6AA", "#452C2C", "#636375", "#A3C8C9", "#FF913F", "#938A81",
        "#575329", "#00FECF", "#B05B6F", "#8CD0FF", "#3B9700", "#04F757", "#C8A1A1", "#1E6E00",
        "#7900D7", "#A77500", "#6367A9", "#A05837", "#6B002C", "#772600", "#D790FF", "#9B9700",
        "#549E79", "#FFF69F", "#201625", "#72418F", "#BC23FF", "#99ADC0", "#3A2465", "#922329",
        "#5B4534", "#FDE8DC", "#404E55", "#0089A3", "#CB7E98", "#A4E804", "#324E72", "#6A3A4C",
        "#83AB58", "#001C1E", "#D1F7CE", "#004B28", "#C8D0F6", "#A3A489", "#806C66", "#222800",
        "#BF5650", "#E83000", "#66796D", "#DA007C", "#FF1A59", "#8ADBB4", "#1E0200", "#5B4E51",
        "#C895C5", "#320033", "#FF6832", "#66E1D3", "#CFCDAC", "#D0AC94", "#7ED379", "#012C58",
        "#7A7BFF", "#D68E01", "#353339", "#78AFA1", "#FEB2C6", "#75797C", "#837393", "#943A4D",
        "#B5F4FF", "#D2DCD5", "#9556BD", "#6A714A", "#001325", "#02525F", "#0AA3F7", "#E98176",
        "#DBD5DD", "#5EBCD1", "#3D4F44", "#7E6405", "#02684E", "#962B75", "#8D8546", "#9695C5",
        "#E773CE", "#D86A78", "#3E89BE", "#CA834E", "#518A87", "#5B113C", "#55813B", "#E704C4",
        "#00005F", "#A97399", "#4B8160", "#59738A", "#FF5DA7", "#F7C9BF", "#643127", "#513A01",
        "#6B94AA", "#51A058", "#A45B02", "#1D1702", "#E20027", "#E7AB63", "#4C6001", "#9C6966",
        "#64547B", "#97979E", "#006A66", "#391406", "#F4D749", "#0045D2", "#006C31", "#DDB6D0",
        "#7C6571", "#9FB2A4", "#00D891", "#15A08A", "#BC65E9", "#FFFFFE", "#C6DC99", "#203B3C",
        "#671190", "#6B3A64", "#F5E1FF", "#FFA0F2", "#CCAA35", "#374527", "#8BB400", "#797868",
        "#C6005A", "#3B000A", "#C86240", "#29607C", "#402334", "#7D5A44", "#CCB87C", "#B88183",
        "#AA5199", "#B5D6C3", "#A38469", "#9F94F0", "#A74571", "#B894A6", "#71BB8C", "#00B433",
        "#789EC9", "#6D80BA", "#953F00", "#5EFF03", "#E4FFFC", "#1BE177", "#BCB1E5", "#76912F",
        "#003109", "#0060CD", "#D20096", "#895563", "#29201D", "#5B3213", "#A76F42", "#89412E",
        "#1A3A2A", "#494B5A", "#A88C85", "#F4ABAA", "#A3F3AB", "#00C6C8", "#EA8B66", "#958A9F",
        "#BDC9D2", "#9FA064", "#BE4700", "#658188", "#83A485", "#453C23", "#47675D", "#3A3F00",
        "#DFFB71", "#868E7E", "#98D058", "#6C8F7D", "#D7BFC2", "#3C3E6E", "#D83D66"
        });

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(m_Colors.begin(), m_Colors.end(), std::default_random_engine(seed));
    m_Colors.push_back("#FFFFFF");

    m_States.insert({ "FREE", "#FFFFFF" });
}

void InputStates::MakeColorAvailable(std::string color)
{
    for (auto it = m_Colors.rbegin(); it != m_Colors.rend(); it++)
    {
        if (*it == color)
        {
            m_Colors.erase((++it).base());
            m_Colors.push_front(color);
            return;
        }
    }
}

void InputStates::MakeColorUnavailable(std::string color)
{
    m_Colors.pop_front();
    m_Colors.push_back(color);
}
