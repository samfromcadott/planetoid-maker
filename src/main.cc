#include <iostream>
#include <brutus/brutus.h>
#include <raylib-cpp.hpp>
#include <rlgl.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "mesh.hh"
#include "camera.hh"
#include "sdf.hh"
#include "typedefs.hh"
#include "planet.hh"

Brutus::Grid grid(1, 1, 1);

enum PlanetType {
	MOON,
	ASTEROID,
	HEAD
};

int current_planet = MOON;
bool drop_down = false;

void gui_update();
void new_planet();

int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 600;

	raylib::Window window(screenWidth, screenHeight, "Planetoid Maker");

	Camera3D camera = { 0 };
	camera.position = (Vector3){ 0.0f, 0.0f, 8.0f };
	camera.target = (Vector3){ 8.0f, 8.0f, 8.0f };
	camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	SetTargetFPS(60);

	GuiLoadStyleDefault();
	GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);

	float r = grid.total_size().x / 2;
	camera.position = (Vector3){ 0.0f, 0.0f, r };
	camera.target = (Vector3){ r, r, r };

	new_planet();

	while ( !WindowShouldClose() ) {
		update_camera(camera);

		BeginDrawing();

			ClearBackground({32, 32, 32, 255});

			BeginMode3D(camera);
				rlSetCullFace(RL_CULL_FACE_FRONT);
				render_mesh(grid.generate_mesh(0, 0, 0));

				// Draw the bounds of the grid
				const Color clear_grey = {224, 224, 224, 128};
				Vector3 bounds = {(float)grid.total_size().x-1, (float)grid.total_size().y-1, (float)grid.total_size().z-1};
				DrawCubeWiresV( Vector3Scale(bounds, 0.5), bounds, clear_grey );

			EndMode3D();

			gui_update();

		EndDrawing();
	}

	return 0;
}

void gui_update() {
	rlSetCullFace(RL_CULL_FACE_BACK);

	if ( GuiDropdownBox({10, 10, 100, 20}, "MOON;ASTEROID;HEAD", &current_planet, drop_down) )
		drop_down = !drop_down;

	if ( GuiButton({120, 10, 70, 20}, "GENERATE") )
		new_planet();
}

void new_planet() {
	SDF planet = space();

	switch (current_planet) {
		case MOON:
			planet = moon();
			break;
		case ASTEROID:
			planet = asteroid();
			break;
		case HEAD:
			planet = head();
			break;
	}

	// Set voxel weights
	for (int x = 0; x < grid.total_size().x; x++)
	for (int y = 0; y < grid.total_size().y; y++)
	for (int z = 0; z < grid.total_size().z; z++) {
		float w = planet( vec3(x, y, z) );
		grid(x, y, z).weight = Clamp(w * 64, -128, 127 );
	}
}
