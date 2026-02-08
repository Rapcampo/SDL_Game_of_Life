#include "../includes/life.h"
//life should have window x and y, map file to be parsed,
//TODO: parsing function to verify initial map
//TODO: implement screen pixel drawing
//TODO: implement game of life logic

void	cleanup(t_life *life);

int parse(const int width, const int height, const char *map, t_life *life) {

	//FILE *map = fopen(map, "r");
	(void)map;
	if (width < 640  || width > 3840 || height < 480  || height > 2160)
		return 1;
	else if (width % 10 != 0 || height % 10 != 0)
		return 1;
	life->width = width;
	life->height = height;
	return 0;
}

void	gameInit(t_life *life) {
	if (!SDL_Init(SDL_INIT_VIDEO))
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL: %s", SDL_GetError());

	life->window = SDL_CreateWindow("Game of Life",
		life->width, life->height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (life->window == nullptr){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		cleanup(life);
	}
	life->cellsize = 10;
	life->cw = life->width / life->cellsize;
	life->ch = life->height / life->cellsize;
}

void	mainLoop(t_life *life) {
	const bool *keys = SDL_GetKeyboardState(nullptr);
	bool paused = false;

	while (life->running){
		SDL_Event event;
		SDL_Delay(16);
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_EVENT_QUIT
				|| (event.type == SDL_EVENT_KEY_DOWN && keys[SDL_SCANCODE_ESCAPE])){
				life->running = false;
				}
			else if (event.type == SDL_EVENT_KEY_DOWN && keys[SDL_SCANCODE_SPACE]){
				paused = paused ? false : true;
				paused ? SDL_Log("game is paused") : SDL_Log("game is unpaused");
			}
		}
		if (paused)
			continue;
		game_of_life(life);
		draw_life(life);
	}
}

void	cleanup(t_life *life) {
	if (life->window)
		SDL_DestroyWindow(life->window);
	SDL_Quit();
}

int main(int argc, char **argv){
	t_life life = {0};
	life.running = true;

	if (argc < 4) {
		fputs("Insufficient arguments provided", stderr);
		puts(FORMAT USAGE);
		return 1;
	}
	if (parse(atoi(argv[1]), atoi(argv[2]), argv[3], &life)) {
		fputs("Invalid Arguments provided", stderr);
		return 1;
	}
	gameInit(&life);
	mainLoop(&life);
	cleanup(&life);
	return 0;
}
