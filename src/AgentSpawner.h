#ifndef DESIREPATHS_AGENTSPAWNER_H
#define DESIREPATHS_AGENTSPAWNER_H

#include "Agent.h"

class AgentSpawner {
public:
	AgentSpawner(TileGrid& tileGrid, Pathfinder& pathfinder, float spawnInterval);
	void Update(float deltaTime);
	void Draw();
	void Reset();
private:
	void SpawnNew();

	static const unsigned int POOL_SIZE = 100;
	Agent m_agents[POOL_SIZE];
	TileGrid& m_tileGrid;
	Pathfinder& m_pathfinder;
	std::vector<GridCoordinate> m_spawnPoints{};
	float m_spawnInterval;
	float m_timeToNextSpawn = m_spawnInterval;
};

#endif //DESIREPATHS_AGENTSPAWNER_H
