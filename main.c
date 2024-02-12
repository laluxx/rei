#include "raylib.h"

void HandleKeys();
Vector3 Lerp(Vector3 start, Vector3 end, float amount);

const int screenWidth = 1920;
const int screenHeight = 1080;

float cameraAngle = 45.0f;
float cameraHeight = 10.0f;
float lerpAmount = 0.05f;
int gridSpacing = 1;
int gridSize = 50; // around the cube

Vector3 playerPosition = { 0.0f, 0.5f, 0.0f };


int main(void) {
    InitWindow(screenWidth, screenHeight, "Rei");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ cameraHeight, cameraHeight, cameraHeight };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = cameraAngle;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 cameraOffset = (Vector3){ -cameraHeight * 1.5f, cameraHeight, -cameraHeight * 1.5f };

    SetTargetFPS(144);
    while (!WindowShouldClose()) {

        HandleKeys();
        
        Vector3 desiredCameraPosition = {
            playerPosition.x + cameraOffset.x,
            playerPosition.y + cameraOffset.y,
            playerPosition.z + cameraOffset.z
        };

        camera.position = Lerp(camera.position, desiredCameraPosition, lerpAmount);
        camera.target = Lerp(camera.target, playerPosition, lerpAmount);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                // Adjust grid drawing to offset by half a unit
                float halfUnitOffset = 0.5f; // Offset to align grid with cube corner
                for (int i = -gridSize; i <= gridSize; i++) {
                    Vector3 startLineX = { playerPosition.x - gridSize + halfUnitOffset, 0.0f, playerPosition.z + i + halfUnitOffset };
                    Vector3 endLineX = { playerPosition.x + gridSize + halfUnitOffset, 0.0f, playerPosition.z + i + halfUnitOffset };
                    Vector3 startLineZ = { playerPosition.x + i + halfUnitOffset, 0.0f, playerPosition.z - gridSize + halfUnitOffset };
                    Vector3 endLineZ = { playerPosition.x + i + halfUnitOffset, 0.0f, playerPosition.z + gridSize + halfUnitOffset };
                    DrawLine3D(startLineX, endLineX, LIGHTGRAY);
                    DrawLine3D(startLineZ, endLineZ, LIGHTGRAY);
                }

                DrawCube(playerPosition, 1.0f, 1.0f, 1.0f, RED); // Draw the cube
                DrawCubeWires(playerPosition, 1.0f, 1.0f, 1.0f, MAROON);

            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void HandleKeys() {
    if (IsKeyPressed(KEY_H)) playerPosition.x += gridSpacing;
    if (IsKeyPressed(KEY_L)) playerPosition.x -= gridSpacing;
    if (IsKeyPressed(KEY_J)) playerPosition.z -= gridSpacing;
    if (IsKeyPressed(KEY_K)) playerPosition.z += gridSpacing;
}


Vector3 Lerp(Vector3 start, Vector3 end, float amount) {
    Vector3 result;
    result.x = start.x + amount * (end.x - start.x);
    result.y = start.y + amount * (end.y - start.y);
    result.z = start.z + amount * (end.z - start.z);
    return result;
}
