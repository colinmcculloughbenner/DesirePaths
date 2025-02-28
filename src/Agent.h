#ifndef DESIREPATHS_AGENT_H
#define DESIREPATHS_AGENT_H

#include "Pathfinder.h"

class Agent {
public:
	Agent() = default;
	bool Initialize(std::list<GridCoordinate> path, TileGrid* tileGrid);
	void Update(float deltaTime);
	void Draw();
	void Kill() { b_isAlive = false; }
private:
	void SetNewWaypoint();

	TileGrid* m_tileGrid = nullptr;
	std::list<GridCoordinate> m_path;

	bool b_isAlive = false;
	Vector2 m_position{};
	Vector2 m_nextWaypoint{};
	float m_speed = 10;

	constexpr static const Color COLORS[]{MAROON, ORANGE, DARKBLUE, DARKPURPLE, RED, GOLD, BLUE, VIOLET, PINK, YELLOW, PURPLE, WHITE};
	Color m_color = COLORS[rand() % 12];
};

#endif //DESIREPATHS_AGENT_H
