#include "TreeRenderer.h"
#include <raymath.h>
#include <LDtkLoader/Project.hpp>
#include <random>

TreeRenderer::TreeRenderer()
{
	m_renderTexture = LoadRenderTexture(1280, 720);

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
	const auto& layer = level.getLayer("Trees");

	BeginTextureMode(m_renderTexture);
	ClearBackground(ColorAlpha(WHITE, 0));

	std::random_device rd{};
	std::mt19937 generator{ rd() };
	std::normal_distribution<float> radiusDistribution(20, 4);
	std::vector<Vector2> treePoints;

	for (auto& tree: layer.allEntities()) {
		auto c = Vector2{16.F * tree.getGridPosition().x + 8, 16.F * tree.getGridPosition().y + 8};
		auto radius = radiusDistribution(generator);
		std::uniform_real_distribution<float> pointDistribution(radius - 2, radius + 2);

		DrawCircleV(c, 0.25F * radius, BLACK);

		treePoints.clear();
		auto anglePerPoint = PI / 16.F;
		for (int i = 0; i < 32; ++i) {
			float angle = anglePerPoint * i;
			Vector2 position{pointDistribution(generator), 0};
			treePoints.push_back(Vector2Add(c, Vector2Rotate(position, angle)));
		}

		for (int i = 0; i < 32; ++i) {
			auto& from = treePoints[i];
			auto& to = treePoints[(i+1)%32];
			DrawTriangle(c, to, from, ColorAlpha(DARKGREEN, 0.9));
			DrawLineEx(from, to, 1, ColorLerp(DARKGREEN, BLACK, 0.5));
		}
	}

	EndTextureMode();
}

void TreeRenderer::Draw() const
{
	DrawTextureRec(m_renderTexture.texture, Rectangle{0, 0, 1280, -720}, Vector2Zeros, WHITE);
}
