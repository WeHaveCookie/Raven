#pragma once
#include <map>
#include <vector>

#include "Raven_Bot.h"

class TeamManager
{
private:
	static TeamManager* s_singleton;

	int m_NumberOfTeam;
	int m_NumberOfBotPerTeam;

	std::map < int, std::vector<Raven_Bot*>> * m_Team;

	int m_NumberOfBot;
	int offsetColor;
public:
	static TeamManager* GetSingleton() { return s_singleton; }
	TeamManager();
	~TeamManager();

	void RegisterTeam(Raven_Bot* bot);
	void UnregisterTeam();
	void SetNumTeam(int nb) { m_NumberOfTeam = nb; }

	int getColor(int team);

	std::vector<Raven_Bot*> getAlly(int team) { return m_Team->at(team); }
	int getSizeTeam(int team) { return m_Team->at(team).size(); }

	Raven_Bot* getLowestEnnemiesHealth(int team);

	Raven_Bot* GetLeaderOfTeam(int team) const;

	void ChangeLeaderOfTeam(int team);
};

