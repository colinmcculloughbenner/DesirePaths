#include "TileGrid.h"
#include "LDtkLoader/Project.hpp"
#include <iostream>

TileGrid::TileGrid()
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
	const auto& layer = level.getLayer("Tiles");
	const auto& tileset = ldtkProject.getTileset("Tileset");

	// Load the atlas texture from file
	try {
		m_atlasTexture = LoadTexture("resources/Tileset.png");
	} catch (std::exception& e) {
		std::cerr << "Couldn't load tilemap texture.\n";
		return;
	}

	// Initialize the grid
	m_grid = std::make_unique<Grid<TileData>>(level.size.x, level.size.y);

	// Add a new GridNode for each tile
	for (const auto& tile: layer.allTiles()) {
		// Grid coordinates
		int x = tile.getGridPosition().x;
		int y = tile.getGridPosition().y;

		// Material and condition if relevant
		auto data = tileset.getTileCustomData(tile.tileId);
		auto newLineIdx = data.find('\n');
		auto matString = data.substr(0, newLineIdx);
		GroundType groundType;
		float condition = 0;
		if (matString == "GRASS") {
			groundType = GRASS;
			auto conditionString = data.substr(newLineIdx + 1);
			condition = std::stof(conditionString);
		} else if (matString == "SIDEWALK") {
			groundType = SIDEWALK;
		} else if (matString == "CROSSWALK") {
			groundType = CROSSWALK;
		} else if (matString == "STREET") {
			groundType = STREET;
		} else if (matString == "IMPASSABLE") {
			groundType = IMPASSABLE;
		} else {
			std::cerr << "Tile contains invalid GroundType data.\n";
			groundType = IMPASSABLE;
		}

		// Texture source rect
		auto rect = tile.getTextureRect();
		if (groundType == GRASS) rect.x = 0;

		// Add new node
		m_grid->GetData(x, y) = TileData{ groundType, condition, Rectangle{ (float)rect.x, (float)rect.y, (float)rect.height, (float)rect.width}};
	}

	// Add tree data
	const auto& treeLayer = level.getLayer("Trees");
	for (auto& tree: treeLayer.allEntities()) {
		auto& entry = m_grid->GetData(tree.getGridPosition().x, tree.getGridPosition().y);
		entry.tree = true;
	}

	m_renderTexture = LoadRenderTexture(1280, 720);

	RedrawRenderTexture();
}

TileGrid::~TileGrid()
{
	UnloadTexture(m_atlasTexture);
}

void TileGrid::Draw() const
{
	DrawTextureRec(m_renderTexture.texture, Rectangle{0, 0, 1280, -720}, Vector2Zeros, WHITE);
}

TileData& TileGrid::GetNodeAtCoordinate(GridCoordinate c)
{
	return m_grid->GetData(c);
}

void TileGrid::RedrawRenderTexture()
{
	BeginTextureMode(m_renderTexture);

	// Draw ground

	for (int y = 0; y < GetHeight(); ++y) {
		for (int x = 0; x < GetWidth(); ++x) {
			Vector2 position{(float)x * 16.F, (float)y * 16.F};
			auto node = m_grid->GetData(x, y);

			if (node.groundType == GRASS) {
				auto color = ColorLerp(BEIGE, DARKGREEN, node.condition);
				DrawRectangle(position.x, position.y, 16, 16, color);
			} else {
				DrawTextureRec(m_atlasTexture, node.textureSourceRect, position, WHITE);
			}
		}
	}

	// Draw buildings
	ldtk::Project ldtkProject;

	try {
		ldtkProject.loadFromFile("resources/DesirePaths.ldtk");
	} catch (std::exception& e) {
		std::cerr << "Couldn't find LDtk project file.\n";
		return;
	}

	const auto& world = ldtkProject.getWorld();
	const auto& level = world.getLevel(0);

	const auto& buildingLayer = level.getLayer("Buildings");
	for (auto& t: buildingLayer.allTiles()) {
		DrawTextureRec(
				m_atlasTexture,
				Rectangle{(float)t.getTextureRect().x, (float)t.getTextureRect().y, (float)t.getTextureRect().width, (float)t.getTextureRect().height},
				Vector2{(float)t.getGridPosition().x * 16, (float)t.getGridPosition().y * 16},
				WHITE);
	}

	EndTextureMode();
}
void TileGrid::StepOnGrass(GridCoordinate c, float amount)
{
	auto& tile = m_grid->GetData(c);
	if (tile.groundType != GRASS) return;

	tile.condition -= amount;

	BeginTextureMode(m_renderTexture);

	auto position = Vector2{(float)c.x * 16.F, (float)c.y * 16.F};
	auto color = ColorLerp(BEIGE, DARKGREEN, tile.condition);
	DrawRectangle(position.x, position.y, 16, 16, color);

//	float xTex = 64 - (std::floor(tile.condition * 4) * 16);
//	Rectangle textureRect{xTex, 16, 16, 16};
//	DrawTextureRec(m_atlasTexture, textureRect, position, WHITE);
//	float remainder = (tile.condition * 4) - std::floor(tile.condition * 4);
//	if (xTex <= 48 && remainder > 0) {
//		textureRect.x += 16;
//		Color color = ColorAlpha(WHITE, remainder);
//		DrawTextureRec(m_atlasTexture, textureRect, position, color);
//	}

	EndTextureMode();
}
void TileGrid::Reset()
{
	for (int y = 0; y < m_grid->GetHeight(); ++ y) {
		for (int x = 0; x < m_grid->GetWidth(); ++x) {
			auto& cell = m_grid->GetData(x, y);
			cell.condition = 1;
		}
	}
	RedrawRenderTexture();
}
