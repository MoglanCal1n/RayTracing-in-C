#include <stdio.h>
#include <SDL.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define COLOR_GRAY 0xefefefef
#define COLOR_RAY 0xffd43b
#define RAYS_NUMBER 200


struct Circle{
    double x;
    double y;
    double radius;
};

struct Ray{
    double xStart, yStart;
    double angle;
};


/**
 * Function FillCircle uses the surface and the coordinates of
 * the circle so that it can drow to the screen our shape to the designated
 * color we wish
 *
 * We can determine the circle by thinking that the circle is inside of a square
 * Each pixel on this square is located between x - radius - x + radius and y - radius - y + radius
 * We go through each pixel that is located between the coordinates that have been listed earlier
 * and we check if they fit the condition that the point is located inside the circle
 *
 * After this we create a pixel and fill our surface with it
 *
 * @param surface type: SDL_Surface - the surface of our window
 * @param circle type: struct Circle - this struct contains valuable information about the circle
 * @param color type: Unsigned 32 bits Integer - we use this variable for drawing our circle in different colours
 */
void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color){
    double radiusSquared = pow(circle.radius, 2);
    for (double x = circle.x - circle.radius; x <= circle.x + circle.radius; x++){
        for (double y = circle.y - circle.radius; y <= circle.y + circle.radius; y++){
            double distanceFromCenter = pow(x - circle.x, 2) + pow(y-circle.y, 2);
            if (distanceFromCenter < radiusSquared){
                SDL_Rect pixel = (SDL_Rect){x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}



/**
 * Function that generates the ray emited by the circle
 * @param circle the circle that emits the ray
 * @param rays an array in which we store the rays of the circle
 */
void generateRays(struct Circle circle, struct Ray rays[RAYS_NUMBER]){
    //generate rays
    for (int i = 0; i < RAYS_NUMBER; i++){
        double angle = ((double) i / RAYS_NUMBER) * 2 * M_PI;
        struct Ray ray = {circle.x, circle.y, angle};
        rays[i] = ray;
    }

}

/**
 *
 * @param surface
 * @param rays
 * @param color
 */
void FillRays(SDL_Surface* surface, struct Ray rays[RAYS_NUMBER], Uint32 color, struct Circle object){
    double radiusSquared = pow(object.radius, 2);
    for (int i = 0; i < RAYS_NUMBER; i++){
        struct Ray ray = rays[i];

        int endOfScreen = 0;
        int objectHit = 0;

        double xDraw= ray.xStart;
        double yDraw = ray.yStart;
        double step = 1;
        while (!endOfScreen && !objectHit){
            xDraw += step * cos(ray.angle);
            yDraw += step * sin(ray.angle);

            SDL_Rect pixel = (SDL_Rect){xDraw, yDraw, 1, 3};
            SDL_FillRect(surface, &pixel, color);

            if (xDraw < 0 || xDraw > WIDTH)
                endOfScreen = 1;
            if (yDraw < 0 || yDraw > HEIGHT)
                endOfScreen = 1;

            double distanceFromCenter = pow(xDraw - object.x, 2) + pow(yDraw-object.y, 2);
            if (distanceFromCenter < radiusSquared){
                break;
            }
        }
    }
}

//void moveShadowCircle(struct Circle shadowCircle, SDL_Surface* surface){
//    int moving = 1;
//    SDL_Event event;
//    while(moving) {
//        while(SDL_PollEvent(&event)) {
//
//            if (event.type == SDL_QUIT){
//                moving = 0;
//            }
//
//            while (shadowCircle.y != 0) {
//                shadowCircle.y -= 1;
//                SDL_Delay(100);
//                FillCircle(surface, shadowCircle, COLOR_BLACK);
//                FillCircle(surface, shadowCircle, COLOR_WHITE);
//            }
//
//            while (shadowCircle.y != HEIGHT) {
//                shadowCircle.y += 1;
//                SDL_Delay(100);
//                FillCircle(surface, shadowCircle, COLOR_BLACK);
//                FillCircle(surface, shadowCircle, COLOR_WHITE);
//            }
//        }
//    }
//}

int main(int argc, char* argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create an SDL2 window
    SDL_Window* window = SDL_CreateWindow(
            "RayTracing",                     // Window title
            SDL_WINDOWPOS_CENTERED,           // X position
            SDL_WINDOWPOS_CENTERED,           // Y position
            WIDTH,                            // Width of the window
            HEIGHT,                           // Height of the window
            SDL_WINDOW_SHOWN                  // Flags
    );

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // creating a surface
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    //creating a circle
    struct Circle circle= {100, 100, 20};

    //the circle on which we cast the LIGHT!!!
    struct Circle shadowCircle = {600, 400, 140 };

    // variable used, so we can get the imput from the keyboard
    SDL_Event event;


    struct Ray rays[RAYS_NUMBER];
    generateRays(circle, rays);

    int simulationRunning = 1;

    while (simulationRunning) {

        //event proccesuing loop
        while(SDL_PollEvent(&event)){
            // if the x on the window is pressed
            if (event.type == SDL_QUIT){
                simulationRunning = 0;
            }

            // we use this so we can move the circle
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0){
                FillCircle(surface, circle, COLOR_BLACK);
                FillRays(surface, rays, COLOR_BLACK, shadowCircle);
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generateRays(circle, rays);
            }

        }

//        // cover the old circle
//        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

        FillCircle(surface, shadowCircle, COLOR_WHITE);

        // with this method we are filling our circle
        FillCircle(surface, circle ,COLOR_WHITE);


        FillRays(surface, rays, COLOR_RAY, shadowCircle);
        // update this window
        SDL_UpdateWindowSurface(window);


    //    moveShadowCircle(&circle, surface);
        //delay to draw again and again
        SDL_Delay(10);
    }




}