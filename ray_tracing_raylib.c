#include <stdio.h>
#include <raylib.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 600

#define RAYS_NUMBER 500
#define RAY_THICKNESS 1

struct Circle
{
    double x;
    double y;
    double r;
};

struct LightRay
{
    double x_start, y_start;
    double angle;
    double x_end, y_end;
};

void FillCircleCustom(struct Circle circle, Color color)
{
    double radius_squared = pow(circle.r, 2);

    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distance_squared =
                pow(x - circle.x, 2) +
                pow(y - circle.y, 2);

            if (distance_squared < radius_squared)
            {
                DrawPixel((int)x, (int)y, color);
            }
        }
    }
}

void generate_rays(struct Circle circle, struct LightRay rays[RAYS_NUMBER])
{
    for (int i = 0; i < RAYS_NUMBER; i++)
    {
        double angle = ((double)i / RAYS_NUMBER) * 2 * PI;

        struct LightRay ray =
        {
            circle.x,
            circle.y,
            angle,
            0,
            0
        };

        rays[i] = ray;
    }
}

void FillRays(
    struct LightRay rays[RAYS_NUMBER],
    Color color,
    Color blur_color,
    struct Circle object)
{
    double radius_squared = pow(object.r, 2);

    for (int i = 0; i < RAYS_NUMBER; i++)
    {
        struct LightRay ray = rays[i];

        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1;

        double x_draw = ray.x_start;
        double y_draw = ray.y_start;

        while (!end_of_screen && !object_hit)
        {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            // Optional blur effect
            // DrawRectangle(
            //     (int)x_draw,
            //     (int)y_draw,
            //     (int)(1.5 * RAY_THICKNESS),
            //     (int)(1.5 * RAY_THICKNESS),
            //     blur_color
            // );

            DrawRectangle(
                (int)x_draw,
                (int)y_draw,
                RAY_THICKNESS,
                RAY_THICKNESS,
                color
            );

            if (x_draw < 0 || x_draw > WIDTH)
            {
                end_of_screen = 1;
            }

            if (y_draw < 0 || y_draw > HEIGHT)
            {
                end_of_screen = 1;
            }

            // Does the ray hit the object?
            double distance_squared =
                pow(x_draw - object.x, 2) +
                pow(y_draw - object.y, 2);

            if (distance_squared < radius_squared)
            {
                object_hit = 1;
            }
        }
    }
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Raytracing - Raylib");

    SetTargetFPS(100);

    struct Circle circle = {200, 200, 40};
    struct Circle shadow_circle = {600, 300, 140};

    struct LightRay rays[RAYS_NUMBER];

    generate_rays(circle, rays);

    double obstacle_speed_y = 4;

    while (!WindowShouldClose())
    {
        // Mouse drag movement
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 mouse = GetMousePosition();

            circle.x = mouse.x;
            circle.y = mouse.y;

            generate_rays(circle, rays);
        }

        shadow_circle.y += obstacle_speed_y;

        if (shadow_circle.y - shadow_circle.r < 0)
        {
            obstacle_speed_y = -obstacle_speed_y;
        }

        if (shadow_circle.y + shadow_circle.r > HEIGHT)
        {
            obstacle_speed_y = -obstacle_speed_y;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        FillRays(
            rays,
            GOLD,
            ORANGE,
            shadow_circle
        );

        FillCircleCustom(circle, WHITE);

        FillCircleCustom(shadow_circle, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}