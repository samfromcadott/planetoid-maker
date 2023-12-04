#include <iostream>
#include <brutus/brutus.h>
#include <raylib-cpp.hpp>
#include <rlgl.h>

#include "mesh.hh"
#include "camera.hh"
#include "editing.hh"
#include "sdf.hh"
#include "typedefs.hh"
#include "planet.hh"

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

	rlSetCullFace(RL_CULL_FACE_FRONT);

	Brutus::Grid grid(1, 1, 1);

	float r = grid.total_size().x / 2;
	camera.position = (Vector3){ 0.0f, 0.0f, r };
	camera.target = (Vector3){ r, r, r };


	// SDF planet = sphere(r - 1);
	// // planet = add( planet, box(5, 5, 1) );
	// // SDF planet = box(5, 5, 2);
	// // planet = transform( planet, mat4::RotateXYZ( vec3(0, 45, 45) ) * mat4::Translate(5, 5, 5) );
	// // SDF::sphere(grid, {8,8,8}, 6.0, -1); // Generate a sphere using an SDF
	//
	// for (int i = 0; i < 50; i++) {
	// 	float radius = GetRandomValue(1, 5);
	// 	SDF crater = sphere( radius );
	// 	crater = transform( crater, mat4::Translate(r, 0, 0) );
	// 	vec3 rotation( GetRandomValue(0, 360), GetRandomValue(0, 360), GetRandomValue(0, 360) );
	// 	crater = transform( crater, mat4::RotateXYZ(rotation) );
	//
	// 	planet = sub(crater, planet);
	// }
	//
	// planet = transform( planet, mat4::Translate(r, r, r) );
	SDF planet = asteroid();

	// Set voxel weights
	for (int x = 0; x < grid.total_size().x; x++)
	for (int y = 0; y < grid.total_size().y; y++)
	for (int z = 0; z < grid.total_size().z; z++) {
		float w = planet( vec3(x, y, z) );
		grid(x, y, z).weight = Clamp(w * 64, -128, 127 );
	}

	// Generate the mesh
	// std::vector< Brutus::Mesh > meshes;
	// for (int x = 0; x < grid.chunk_size().x; x++)
	// for (int y = 0; y < grid.chunk_size().y; y++)
	// for (int z = 0; z < grid.chunk_size().z; z++) {
	// 	std::cout << x << " " << y << " " << z << '\n';
	// 	meshes.push_back( grid.generate_mesh(x, y, z) );
	// }

	while ( !WindowShouldClose() ) {
		// get_edit(camera, grid);
		update_camera(camera);

		BeginDrawing();

			ClearBackground({32, 32, 32, 255});

			BeginMode3D(camera);
				render_mesh(grid.generate_mesh(0, 0, 0));
				// render_mesh(grid.generate_mesh(0, 0, 1));
				// render_mesh(grid.generate_mesh(0, 1, 0));
				// render_mesh(grid.generate_mesh(0, 1, 1));
				// render_mesh(grid.generate_mesh(1, 0, 0));
				// render_mesh(grid.generate_mesh(1, 0, 1));
				// render_mesh(grid.generate_mesh(1, 1, 0));
				// render_mesh(grid.generate_mesh(1, 1, 1));
				// for (auto& mesh : meshes) render_mesh(mesh);

				// Draw the bounds of the grid
				const Color clear_grey = {224, 224, 224, 128};
				Vector3 bounds = {(float)grid.total_size().x-1, (float)grid.total_size().y-1, (float)grid.total_size().z-1};
				DrawCubeWiresV( Vector3Scale(bounds, 0.5), bounds, clear_grey );

			EndMode3D();

		EndDrawing();
	}

	return 0;
}
