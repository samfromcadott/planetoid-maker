#include <iostream>
#include <brutus/brutus.h>
#include <raylib-cpp.hpp>

#include "mesh.hh"
#include "camera.hh"
#include "editing.hh"
#include "sdf.hh"
#include "typedefs.hh"

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

	Brutus::Grid grid(2, 2, 2);
	SDF planet = sphere(10.0);
	// SDF::sphere(grid, {8,8,8}, 6.0, -1); // Generate a sphere using an SDF
	for (int x = 0; x < grid.total_size().x; x++)
	for (int y = 0; y < grid.total_size().y; y++)
	for (int z = 0; z < grid.total_size().z; z++) {
		float w = planet( vec3(x, y, z) );
		grid(x, y, z).weight = Clamp(w * 64, -128, 127 );
	}

	while ( !WindowShouldClose() ) {
		// get_edit(camera, grid);
		update_camera(camera);

		BeginDrawing();

			ClearBackground({32, 32, 32, 255});

			BeginMode3D(camera);
				render_mesh(grid.generate_mesh(0, 0, 0));
				render_mesh(grid.generate_mesh(0, 0, 1));
				render_mesh(grid.generate_mesh(0, 1, 0));
				render_mesh(grid.generate_mesh(0, 1, 1));
				render_mesh(grid.generate_mesh(1, 0, 0));
				render_mesh(grid.generate_mesh(1, 0, 1));
				render_mesh(grid.generate_mesh(1, 1, 0));
				render_mesh(grid.generate_mesh(1, 1, 1));

				// Draw the bounds of the grid
				const Color clear_grey = {224, 224, 224, 128};
				Vector3 bounds = {(float)grid.total_size().x-1, (float)grid.total_size().y-1, (float)grid.total_size().z-1};
				DrawCubeWiresV( Vector3Scale(bounds, 0.5), bounds, clear_grey );

			EndMode3D();

		EndDrawing();
	}

	return 0;
}
