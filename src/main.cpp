#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window* window = SDL_CreateWindow("sdl3 test", 640, 480, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0);
	bool done = false;

	if (window == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "couldn't make window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
