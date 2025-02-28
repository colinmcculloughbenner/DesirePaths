#include "AgentSpawner.h"
#include <LDtkLoader/Project.hpp>


AgentSpawner::AgentSpawner(TileGrid& tileGrid, Pathfinder& pathfinder, float spawnInterval)
	:m_tileGrid{ tileGrid}, m_pathfinder{ pathfinder}, m_spawnInterval{ spawnInterval}
{
	// Get data from LDtk
	ldtk::Project ldtkProject;

	try {
		ldtkProject.loadFromFile("resources/DesirePaths.ldtk");
	} catch (std::exception& e) {
		std::cerr << "Couldn't find LDtk project file.\n";
		return;
	}

	const auto& world = ldtkProject.getWorld();
	const auto& level = world.getLevel(0);
	const auto& layer = level.getLayer("SpawnPoints");

	for (auto& e: layer.allEntities()) {
		m_spawnPoints.push_back(GridCoordinate{e.getGridPosition().x, e.getGridPosition().y});
	}
}

void AgentSpawner::Update(float deltaTime)
{
	for (auto & agent : m_agents) {
		agent.Update(deltaTime);
	}

	m_timeToNextSpawn -= deltaTime;
	while (m_timeToNextSpawn <= 0) {
		m_timeToNextSpawn += m_spawnInterval;
		SpawnNew();
	}
}

void AgentSpawner::Draw()
{
	for (auto & m_agent : m_agents) {
		m_agent.Draw();
	}
}

void AgentSpawner::SpawnNew()
{
	auto start = m_spawnPoints[std::rand() % m_spawnPoints.size()];
	auto end = m_spawnPoints[std::rand() % m_spawnPoints.size()];
	while (start == end) {
		end = m_spawnPoints[std::rand() % m_spawnPoints.size()];
	}
	auto path = m_pathfinder.FindPath(start, end);

	for (int i = 0; i < POOL_SIZE; ++i) {
		auto result = m_agents[i].Initialize(path, &m_tileGrid);
		if (result) break;
	}
}

void AgentSpawner::Reset()
{
	for (int i = 0; i < POOL_SIZE; ++i) {
		m_agents[i].Kill();
	}

	m_timeToNextSpawn = m_spawnInterval;
}
