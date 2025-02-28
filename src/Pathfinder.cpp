#include "Pathfinder.h"
#include <queue>
#include <unordered_set>
#include <iostream>

Pathfinder::Pathfinder(TileGrid& grid, TerrainCosts& terrainCosts)  :m_tileGrid{ grid}, m_terrainCosts{terrainCosts}
{
 m_pathGrid = std::make_unique<Grid<PathfinderNode>>(grid.GetWidth(), grid.GetHeight());
}

std::list<GridCoordinate> Pathfinder::FindPath(GridCoordinate start, GridCoordinate end)
{
	ResetGrid();

	std::priority_queue<PathfinderNode, std::vector<PathfinderNode>, decltype(&PathfinderNode::CompareByCost)> open(PathfinderNode::CompareByCost);
	std::unordered_set<GridCoordinate> closed;

	PathfinderNode& currentNode = m_pathGrid->GetData(start);
	currentNode.gCost = 0;
	currentNode.hCost = FindHCost(start, end);

	open.push(currentNode);

	while (!open.empty()) {
		currentNode = open.top();
		open.pop();
		closed.insert(currentNode.coordinate);
		if (currentNode.coordinate == end) {
			return TracePath(start, end);
		}

		// Add neighbors to open set where possible
		for (int x = -1; x <=1; ++x) {
			for (int y = -1; y <= 1; ++y) {
				// Ignore the current node
				if (x == 0 && y == 0)
					continue;

				// Create pathfinder node for neighbor
				GridCoordinate c = currentNode.coordinate;
				c.x += x;
				c.y += y;

				// Handle edges
				if (c.x < 0 || c.x >= m_tileGrid.GetWidth() || c.y < 0 || c.y >= m_tileGrid.GetHeight())
					continue;

				// Check that node is passable and not in closed set
				auto node = m_tileGrid.GetNodeAtCoordinate(c);
				if (node.groundType == IMPASSABLE || node.tree || closed.contains(c))
					continue;

				// Calculate the cost
				int gCost = (x != 0 && y != 0)
						? currentNode.gCost + 14
						: currentNode.gCost + 10;

				auto tile = m_tileGrid.GetNodeAtCoordinate(c);
				switch (tile.groundType) {
				case SIDEWALK:
					gCost += m_terrainCosts.sidewalk;
					break;
				case GRASS:
					gCost += m_terrainCosts.grassConditionMultiplier * tile.condition * tile.condition + m_terrainCosts.grassBase;
					break;
				case CROSSWALK:
					gCost += m_terrainCosts.crosswalk;
					break;
				case STREET:
					gCost += m_terrainCosts.street;
				default:
					break;
				}

				int hCost = FindHCost(c, end);

				// If this node doesn't improve on F cost of existing data at its coordinates, don't add it to open set
				auto& existingNode = m_pathGrid->GetData(c);
				if (existingNode.FCost() <= gCost + hCost)
					continue;

				existingNode.gCost = gCost;
				existingNode.hCost = hCost;
				existingNode.parent = currentNode.coordinate;
				open.push(existingNode);
			}
		}
	}
	std::cerr << "Path impossible\n";
	return std::list<GridCoordinate>{};
}
int Pathfinder::FindHCost(GridCoordinate source, GridCoordinate target)
{
	auto xDiff = std::abs(source.x - target.x);
	auto yDiff = std::abs(source.y - target.y);
	if (xDiff > yDiff) {
		return 10 * (xDiff - yDiff) + 14 * yDiff;
	} else {
		return 10 * (yDiff - xDiff) + 14 * xDiff;
	}
}
std::list<GridCoordinate> Pathfinder::TracePath(GridCoordinate start, GridCoordinate end)
{
	std::list<GridCoordinate> path;
	GridCoordinate current = end;
	path.push_front(end);
	while (current != start) {
		current = m_pathGrid->GetData(current).parent;
		path.push_front(current);
	}
	return path;
}

void Pathfinder::ResetGrid()
{
	for (int y = 0; y < m_pathGrid->GetHeight(); ++y) {
		for (int x = 0; x < m_pathGrid->GetWidth(); ++x) {
			auto& data = m_pathGrid->GetData(x, y);
			data.coordinate = GridCoordinate{x,y};
			data.gCost = INT_MAX / 2;
			data.hCost = INT_MAX / 2;
		}
	}
}

