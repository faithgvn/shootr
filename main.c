#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

#define MAX_COLUMNS 20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3) { 1.0f, 2.0f, 4.0f }; // Camera position
    camera.target = (Vector3) { 0.0f, 2.0f, 0.0f }; // Camera looking at point
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f }; // Camera up vector (rotation towards target)
    camera.fovy = 60.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    // Generates some random columns
    float heights[MAX_COLUMNS] = { 0 };
    Vector3 positions[MAX_COLUMNS] = { 0 };
    Color colors[MAX_COLUMNS] = { 0 };

    for (int i = 0; i < MAX_COLUMNS; i++) {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3) { (float)GetRandomValue(-15, 15), heights[i] / 2.0f, (float)GetRandomValue(-15, 15) };
        colors[i] = (Color) { GetRandomValue(20, 255), GetRandomValue(10, 55), 30, 255 };
    }

    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    cameraMode = CAMERA_FIRST_PERSON;
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f }; // Reset roll

    Vector3 playerPosition = (Vector3) { 0.0f, 0.0f, 0.0f };

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advanced camera controls, it's recommended to compute camera movement manually
        // UpdateCamera(&camera, cameraMode); // Update camera

        // Camera PRO usage example (EXPERIMENTAL)
        // This new camera function allows custom movement/rotation values to be directly provided
        // as input parameters, with this approach, rcamera module is internally independent of raylib inputs
        UpdateCameraPro(&camera,
            (Vector3) {
                0,
                0,
                0.0f // Move up-down
            },
            (Vector3) {
                GetMouseDelta().x * 0.05f, // Rotation: yaw
                GetMouseDelta().y * 0.05f, // Rotation: pitch
                0.0f // Rotation: roll
            },
            0); // Move to target (zoom)

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawPlane((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector2) { 48.0f, 48.0f }, LIME); // Draw ground
        DrawCube((Vector3) { 8.0f, 2.5f, 4.0f }, 2.0f, 2.0f, 2.0f, GOLD); // Draw a yellow wall

        EndMode3D();

        // Draw info boxes
        // Draw crosshair at the center of the screen
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
        int crosshairSize = 10;

        DrawLine(centerX - crosshairSize, centerY, centerX + crosshairSize, centerY, BLACK);
        DrawLine(centerX, centerY - crosshairSize, centerX, centerY + crosshairSize, BLACK);

        DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLUE);

        // Draw weapon at bottom right center like first person view in Counter-Strike
        // Draw a simple 3D-looking weapon at the bottom center, pointing at the crosshair
        // Weapon base (handle)
        int weaponWidth = 60;
        int weaponHeight = 100;
        int weaponX = screenWidth / 2 - weaponWidth / 2;
        int weaponY = screenHeight - weaponHeight - 20;

        // Draw handle (rectangle)
        DrawRectangle(weaponX + weaponWidth / 3, weaponY + weaponHeight / 2, weaponWidth / 3, weaponHeight / 2, DARKGRAY);

        // Draw body (main part)
        DrawRectangle(weaponX, weaponY + weaponHeight / 4, weaponWidth, weaponHeight / 3, GRAY);

        // Draw barrel (pointing to crosshair)
        int barrelWidth = 12;
        int barrelHeight = 40;
        int barrelX = screenWidth / 2 - barrelWidth / 2;
        int barrelY = weaponY + weaponHeight / 4 - barrelHeight + 10;
        DrawRectangle(barrelX, barrelY, barrelWidth, barrelHeight, BLACK);

        // Add some 3D effect with shading
        DrawRectangle(weaponX, weaponY + weaponHeight / 4, weaponWidth, 8, DARKGRAY); // Top shadow
        DrawRectangle(barrelX, barrelY, barrelWidth, 6, DARKGRAY); // Barrel shadow

        // Draw a simple sight on top of the barrel
        DrawRectangle(screenWidth / 2 - 3, barrelY - 10, 6, 10, RED);

        // Draw a dashed line from the weapon's barrel tip to the crosshair (target)
        Vector2 barrelTip = { screenWidth / 2.0f, (float)barrelY }; // Barrel tip (center top of barrel)
        Vector2 crosshair = { screenWidth / 2.0f, screenHeight / 2.0f }; // Crosshair position

        float dashLength = 10.0f;
        float gapLength = 8.0f;
        Vector2 dir = { crosshair.x - barrelTip.x, crosshair.y - barrelTip.y };
        float totalLength = Vector2Length(dir);
        Vector2 normDir = Vector2Normalize(dir);

        float drawn = 0.0f;
        while (drawn < totalLength) {
            float segLen = (drawn + dashLength > totalLength) ? (totalLength - drawn) : dashLength;
            Vector2 start = Vector2Add(barrelTip, Vector2Scale(normDir, drawn));
            Vector2 end = Vector2Add(barrelTip, Vector2Scale(normDir, drawn + segLen));
            DrawLineV(start, end, RED);
            drawn += dashLength + gapLength;
        }

        DrawText("Camera controls:", 15, 15, 10, BLACK);
        DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
        DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
        DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
        DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

        DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(600, 5, 195, 100, BLUE);

        DrawText("Camera status:", 610, 15, 10, BLACK);
        DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" : (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON"
                         : (cameraMode == CAMERA_THIRD_PERSON)                                                       ? "THIRD_PERSON"
                         : (cameraMode == CAMERA_ORBITAL)                                                            ? "ORBITAL"
                                                                                                                     : "CUSTOM"),
            610, 30, 10, BLACK);
        DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" : (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC"
                                                                                                                                                       : "CUSTOM"),
            610, 45, 10, BLACK);
        DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
        DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
        DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}