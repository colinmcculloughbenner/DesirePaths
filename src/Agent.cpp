#include "Agent.h"
#include <raymath.h>

#include <utility>

bool Agent::Initialize(std::list<GridCoordinate> path, TileGrid* tileGrid)
{
	if (b_isAlive) return false;

	b_isAlive = true;
	m_path = std::move(path);
	m_tileGrid = tileGrid;
	auto start = m_path.front();
	m_position = Vector2{(float)start.x * 16.F + 8, (float)start.y * 16.F + 8};
	SetNewWaypoint();
	return true;
}


void Agent::Update(float deltaTime)
{
	if (!b_isAlive) return;

	auto timeLeft = deltaTime;
	while (timeLeft > 0 && b_isAlive) {
		auto toNext = Vector2Subtract(m_nextWaypoint, m_position);
		auto distance = Vector2Length(toNext);
		auto distanceLeft = m_speed * timeLeft;

		if (distanceLeft <= distance) {
			auto deltaPos = Vector2Scale(Vector2Normalize(toNext), m_speed * deltaTime);
			m_position = Vector2Add(m_position, deltaPos);
			return;
		} else {
			auto timeToWaypoint = distance / m_speed;
			timeLeft -= timeToWaypoint;
			m_position = m_nextWaypoint;

			SetNewWaypoint();
		}
	}
}

void Agent::Draw()
{
	if (b_isAlive)
		DrawCircleV(m_position, 7, m_color);
}

void Agent::SetNewWaypoint()
{
	auto currentGridPoint = m_path.front();
	m_tileGrid->StepOnGrass(currentGridPoint, 0.05);
	m_path.pop_front();

	if (m_path.empty()) {
		b_isAlive = false;
		return;
	}

	auto nextGridPoint = m_path.front();
	m_nextWaypoint.x = (float)nextGridPoint.x * 16.F + 8;
	m_nextWaypoint.y = (float)nextGridPoint.y * 16.F + 8;

	// If movement is diagonal, "step on" adjacent cells
	auto xDif = nextGridPoint.x - currentGridPoint.x;
	auto yDif = nextGridPoint.y - currentGridPoint.y;
	if (xDif != 0 && yDif != 0) {
		m_tileGrid->StepOnGrass(GridCoordinate{currentGridPoint.x + xDif, currentGridPoint.y}, 0.01);
		m_tileGrid->StepOnGrass(GridCoordinate{currentGridPoint.x, currentGridPoint.y + yDif}, 0.01);
	}
}
