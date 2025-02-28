#ifndef DESIREPATHS_PATHFINDER_H
#define DESIREPATHS_PATHFINDER_H

#include "TileGrid.h"
#include <list>

struct PathfinderNode{
	GridCoordinate coordinate;
	GridCoordinate parent;
	int gCost;
	int hCost;

	[[nodiscard]] int FCost() const {
		return gCost + hCost;
	}

	static bool CompareByCost(const PathfinderNode& a, const PathfinderNode& b) {
		return a.FCost() > b.FCost();
	}
};

struct TerrainCosts {
	int sidewalk = 0;
	int grassBase = 2;
	int grassConditionMultiplier = 4;
	int crosswalk = 5;
	int street = 25;

};

class Pathfinder {
public:
	explicit Pathfinder(TileGrid& grid, TerrainCosts& terrainCosts);
	std::list<GridCoordinate> FindPath(GridCoordinate start, GridCoordinate end);
private:
	static int FindHCost(GridCoordinate source, GridCoordinate target);
	std::list<GridCoordinate> TracePath(GridCoordinate start, GridCoordinate end);
	void ResetGrid();

	TileGrid& m_tileGrid;
	std::unique_ptr<Grid<PathfinderNode>> m_pathGrid;
	TerrainCosts& m_terrainCosts;
};

#endif //DESIREPATHS_PATHFINDER_H
