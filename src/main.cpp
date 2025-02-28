#include "raylib.h"
#include "TileGrid.h"
#include "Pathfinder.h"
#include "AgentSpawner.h"
#include "TreeRenderer.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

int screenWidth = 1280;
int screenHeight = 720;


// These variables must be in global scope for compatibility with Emscripten, which needs a zero-argument Update function
std::unique_ptr<TileGrid> grid;
std::unique_ptr<Pathfinder> pathfinder;
std::unique_ptr<AgentSpawner> agentSpawner;
std::unique_ptr<TreeRenderer> treeRenderer;

bool isPlaying = false;
float speedScale = 50;
TerrainCosts terrainCosts;
bool sidewalkEditMode;
bool grassBaseEditMode;
bool grassMultiplierEditMode;
bool crosswalkEditMode;
bool streetEditMode;

// Logic run every frame
void Update() {
	if (isPlaying) {
		agentSpawner->Update(GetFrameTime() * speedScale);
	}

	BeginDrawing();
	ClearBackground(RAYWHITE);
	grid->Draw();
	agentSpawner->Draw();
	treeRenderer->Draw();

	// Draw GUI
	// Play Button
	if (isPlaying) {
		if (GuiButton(Rectangle{ 10, 20, 55, 30 }, "Pause")) {
			isPlaying = false;
		}
	} else {
		if (GuiButton(Rectangle{ 10, 20, 55, 30 }, "Play")) {
			isPlaying = true;
		}
	}

	// Reset button
	if (GuiButton(Rectangle{ 75, 20, 55, 30 }, "Reset")) {
		isPlaying = false;
		grid->Reset();
		agentSpawner->Reset();
	}

	// Play speed
	GuiPanel(Rectangle{10, 60, 120, 20}, "Play speed");
	GuiSlider(Rectangle{10, 80, 120, 30}, nullptr, nullptr, &speedScale, 1, 100);


	// Pathfinding costs
	GuiPanel(Rectangle{10, 120, 120, 230}, "Pathfinding costs");

	GuiLabel(Rectangle{20, 140, 100, 30}, "Sidewalk");
	if (GuiSpinner(Rectangle{ 20, 160, 100, 20 }, nullptr, &terrainCosts.sidewalk,
			0, 50, sidewalkEditMode)) {
		sidewalkEditMode = !sidewalkEditMode;
	}

	GuiLabel(Rectangle{20, 180, 100, 30}, "Grass Base Cost");
	if (GuiSpinner(Rectangle{ 20, 200, 100, 20 }, nullptr, &terrainCosts.grassBase,
			0, 50, grassBaseEditMode)) {
		grassBaseEditMode = !grassBaseEditMode;
	}

	GuiLabel(Rectangle{20, 220, 100, 30}, "Grass Condition Multiplier");
	if (GuiSpinner(Rectangle{ 20, 240, 100, 20 }, nullptr, &terrainCosts.grassConditionMultiplier,
			0, 50, grassMultiplierEditMode)) {
		grassMultiplierEditMode = !grassMultiplierEditMode;
	}

	GuiLabel(Rectangle{20, 260, 100, 30}, "Crosswalk");
	if (GuiSpinner(Rectangle{ 20, 280, 100, 20 }, nullptr, &terrainCosts.crosswalk,
			0, 50, crosswalkEditMode)) {
		crosswalkEditMode = !crosswalkEditMode;
	}

	GuiLabel(Rectangle{20, 300, 100, 30}, "Street");
	if (GuiSpinner(Rectangle{ 20, 320, 100, 20 }, nullptr, &terrainCosts.street,
			0, 50, streetEditMode)) {
		streetEditMode = !streetEditMode;
	}

	EndDrawing();
}


int main()
{
	InitWindow(screenWidth, screenHeight, "Desire Paths");

	// Initialization logic
	grid = std::make_unique<TileGrid>();
	pathfinder = std::make_unique<Pathfinder>(*grid, terrainCosts);
	agentSpawner = std::make_unique<AgentSpawner>(*grid, *pathfinder, 5);
	treeRenderer = std::make_unique<TreeRenderer>();

	#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(Update, 60, 1);
	#else
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		Update();
	}
	#endif

	CloseWindow();
	return 0;
}

