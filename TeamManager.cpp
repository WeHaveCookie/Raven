#include <cstdlib>

#include "TeamManager.h"
#include "misc\Cgdi.h"

TeamManager* TeamManager::s_singleton = NULL;

TeamManager::TeamManager()
{
	s_singleton = this;
 	m_NumberOfBot = 0;
	m_Team = new std::map<int, std::vector<Raven_Bot*>>();
	offsetColor = std::rand() % gdi->NumPenColors();
}

TeamManager::~TeamManager()
{
}

void TeamManager::RegisterTeam(Raven_Bot * bot)
{
	int team = m_NumberOfBot % m_NumberOfTeam;
	if (m_Team->count(team) == 0)
	{
		std::vector<Raven_Bot*> newTeam;
		bot->SetLeader(true);
		newTeam.push_back(bot);
		m_Team->insert(std::make_pair(team, newTeam));
	}
	else
	{
		m_Team->at(team).push_back(bot);
	}

	m_NumberOfBot++;
	bot->SetTeam(team);
}

void TeamManager::UnregisterTeam()
{
	int team = (m_NumberOfBot - 1) % m_NumberOfTeam;
	
	if (m_NumberOfBot > 0)
	{
		m_Team->at(team).pop_back();
	}

	m_NumberOfBot--;
}

int TeamManager::getColor(int team)
{
	auto test = (offsetColor + team) % gdi->NumPenColors();
	return (offsetColor + team) % gdi->NumPenColors();
}

Raven_Bot * TeamManager::getLowestEnnemiesHealth(int team)
{
	Raven_Bot* tmp = m_Team->at((team + 1) % m_NumberOfTeam).front();

	std::map<int, std::vector<Raven_Bot*>>::iterator it = m_Team->begin();
	for (it; it != m_Team->end(); ++it)
	{
		if (it->first != team)
		{
			for each (Raven_Bot* var in it->second)
			{
				if (tmp->Health() > var->Health())
				{
					tmp = var;
				}
			}
		}
	}
	return tmp;
}

Raven_Bot* TeamManager::GetLeaderOfTeam(int team) const
{
	for (auto& bot : m_Team->at(team))
	{
		if (bot->isLeader())
		{
			return bot;
		}
	}
	return NULL;
}

void TeamManager::ChangeLeaderOfTeam(int team)
{
	for (auto& bot : m_Team->at(team))
	{
		if (!bot->isDead())
		{
			bot->SetLeader(true);
			break;
		}
	}
}