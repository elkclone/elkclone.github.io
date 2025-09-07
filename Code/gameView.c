#include <SDL.h> // Include SDL2 header

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2 video subsystem

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_WINDOW_SHOWN
    );
    
    SDL_Delay(13000); // Display window for 3 seconds

    SDL_DestroyWindow(window);
    SDL_Quit(); // Quit SDL2

    return 0;
}
