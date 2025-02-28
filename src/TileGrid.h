#ifndef DESIREPATHS_TILEGRID_H
#define DESIREPATHS_TILEGRID_H

#include <unordered_map>
#include <utility>
#include "raylib.h"
#include "raymath.h"
#include <string>
#include <vector>
#include <memory>
#include "Grid.h"

enum GroundType {
	GRASS,
	SIDEWALK,
	CROSSWALK,
	STREET,
	IMPASSABLE
};

struct TileData {
	GroundType groundType{};
	float condition = 1;
	Rectangle textureSourceRect;
	bool tree = false;
};

class TileGrid {
public:
	TileGrid();
	~TileGrid();
	void Draw() const;
	TileData& GetNodeAtCoordinate(GridCoordinate c);
	void StepOnGrass(GridCoordinate c, float amount);
	void Reset();

	[[nodiscard]] unsigned int GetWidth() const { return m_grid->GetWidth(); }
	[[nodiscard]] unsigned int GetHeight() const { return m_grid->GetHeight(); }
private:
	void RedrawRenderTexture();
	Texture2D m_atlasTexture;
	std::unique_ptr<Grid<TileData>> m_grid;
	RenderTexture2D m_renderTexture;
};

#endif //DESIREPATHS_TILEGRID_H
