#include "raylib.h"
#include <math.h>

void HandleKeys(float deltaTime);
Vector3 Lerp(Vector3 start, Vector3 end, float amount);
float LerpAngle(float start, float end, float amount);

const int screenWidth = 1920;
const int screenHeight = 1080;
float cameraAngle = 45.0f;
float cameraHeight = 10.0f;
float lerpSpeed = 5.0f;
float zoomSpeed = 30.0f;
int gridSpacing = 1;
int gridSize = 250;

Vector3 playerPosition = { 0.0f, 0.5f, 0.0f };
float cameraRotation = 45.0f;
float targetCameraRotation = 45.0f;
float cameraDistance = 15.0f;
float isometricAngle = 45.0f;


int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Rei");
    
    Camera3D camera = { 0 };

    camera.position = (Vector3){ cameraHeight, cameraHeight, cameraHeight };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = cameraAngle;
    camera.projection = CAMERA_PERSPECTIVE;
    
    SetTargetFPS(0);
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        HandleKeys(deltaTime);
        
        // Lerp camera rotation with delta time
        cameraRotation = LerpAngle(cameraRotation, targetCameraRotation, lerpSpeed * deltaTime);
        
        // Calculate camera offset based on rotation and distance
        Vector3 cameraOffset;
        cameraOffset.x = cosf(cameraRotation * DEG2RAD) * cameraDistance;
        cameraOffset.y = sinf(isometricAngle * DEG2RAD) * cameraDistance;
        cameraOffset.z = sinf(cameraRotation * DEG2RAD) * cameraDistance;
        
        Vector3 desiredCameraPosition = {
            playerPosition.x + cameraOffset.x,
            playerPosition.y + cameraOffset.y,
            playerPosition.z + cameraOffset.z
        };
        
        // Lerp camera position and target with delta time
        float lerpAmount = lerpSpeed * deltaTime;
        camera.position = Lerp(camera.position, desiredCameraPosition, lerpAmount);
        camera.target = Lerp(camera.target, playerPosition, lerpAmount);
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        // Adjust grid drawing to offset by half a unit
        float halfUnitOffset = 0.5f;
        for (int i = -gridSize; i <= gridSize; i++) {
            Vector3 startLineX = { playerPosition.x - gridSize + halfUnitOffset, 0.0f, playerPosition.z + i + halfUnitOffset };
            Vector3 endLineX = { playerPosition.x + gridSize + halfUnitOffset, 0.0f, playerPosition.z + i + halfUnitOffset };
            Vector3 startLineZ = { playerPosition.x + i + halfUnitOffset, 0.0f, playerPosition.z - gridSize + halfUnitOffset };
            Vector3 endLineZ = { playerPosition.x + i + halfUnitOffset, 0.0f, playerPosition.z + gridSize + halfUnitOffset };
            DrawLine3D(startLineX, endLineX, LIGHTGRAY);
            DrawLine3D(startLineZ, endLineZ, LIGHTGRAY);
        }
        DrawCube(playerPosition, 1.0f, 1.0f, 1.0f, RED);
        DrawCubeWires(playerPosition, 1.0f, 1.0f, 1.0f, MAROON);
        EndMode3D();
        DrawFPS(10, 10);
        // Debug info
        DrawText(TextFormat("Camera Rotation: %.1f°", cameraRotation), 10, 30, 20, BLACK);
        DrawText(TextFormat("Camera Distance: %.1f", cameraDistance), 10, 50, 20, BLACK);
        DrawText("Controls: HJKL = Move, Shift+H/L = Rotate Camera, Shift+J/K = Distance", 10, 70, 20, BLACK);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

void HandleKeys(float deltaTime) {
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
        if (IsKeyPressed(KEY_H)) targetCameraRotation += 90.0f;
        if (IsKeyPressed(KEY_L)) targetCameraRotation -= 90.0f;
        
        if (IsKeyDown(KEY_J)) {
            cameraDistance -= zoomSpeed * deltaTime;
            if (cameraDistance < 5.0f) cameraDistance = 5.0f;
        }
        if (IsKeyDown(KEY_K)) {
            cameraDistance += zoomSpeed * deltaTime;
        }        
    } else {
        if (IsKeyPressed(KEY_H)) {
            playerPosition.x += cosf((cameraRotation + 135.0f) * DEG2RAD) * gridSpacing;
            playerPosition.z += sinf((cameraRotation + 135.0f) * DEG2RAD) * gridSpacing;
        }
        if (IsKeyPressed(KEY_L)) {
            playerPosition.x += cosf((cameraRotation - 45.0f) * DEG2RAD) * gridSpacing;
            playerPosition.z += sinf((cameraRotation - 45.0f) * DEG2RAD) * gridSpacing;
        }
        if (IsKeyPressed(KEY_J)) {
            playerPosition.x += cosf((cameraRotation + 45.0f) * DEG2RAD) * gridSpacing;
            playerPosition.z += sinf((cameraRotation + 45.0f) * DEG2RAD) * gridSpacing;
        }
        if (IsKeyPressed(KEY_K)) {
            playerPosition.x += cosf((cameraRotation - 135.0f) * DEG2RAD) * gridSpacing;
            playerPosition.z += sinf((cameraRotation - 135.0f) * DEG2RAD) * gridSpacing;
        }
        if (IsKeyPressed(KEY_Z)) isometricAngle = 45.0f;
        if (IsKeyPressed(KEY_X)) isometricAngle = 35.264f;
        if (IsKeyPressed(KEY_Q)) CloseWindow();
        if (IsKeyPressed(KEY_ZERO)) cameraDistance = 15.0f;
        if (IsKeyPressed(KEY_BACKSLASH)) {

        }
    }
}

Vector3 Lerp(Vector3 start, Vector3 end, float amount) {
    if (amount > 1.0f) amount = 1.0f;
    if (amount < 0.0f) amount = 0.0f;
    
    Vector3 result;
    result.x = start.x + amount * (end.x - start.x);
    result.y = start.y + amount * (end.y - start.y);
    result.z = start.z + amount * (end.z - start.z);
    return result;
}

float LerpAngle(float start, float end, float amount) {
    if (amount > 1.0f) amount = 1.0f;
    if (amount < 0.0f) amount = 0.0f;
    
    float difference = end - start;
    
    while (difference > 180.0f) difference -= 360.0f;
    while (difference < -180.0f) difference += 360.0f;
    
    return start + amount * difference;
}
