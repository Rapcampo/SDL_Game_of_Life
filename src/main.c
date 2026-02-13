#include "../includes/life.h"
//life should have window x and y, map file to be parsed,
//TODO: implement screen pixel drawing

void	cleanup(t_life *life);

int parse(const int width, const int height, t_life *life) {
	if (width < 640  || width > 3840 || height < 480  || height > 2160)
		return 1;
	else if (width % 10 != 0 || height % 10 != 0)
		return 1;
	life->width = width;
	life->height = height;
	return 0;
}

void	gameInit(t_life *life) {
	SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL: %s", SDL_GetError());
		return;
	}

	SDL_DisplayID primary = SDL_GetPrimaryDisplay();
	life->displays = SDL_GetDisplays(&life->display_num);
	SDL_GetDisplayBounds(primary, &life->DisplayBounds);
	float scale = SDL_GetDisplayContentScale(primary);
	if (life->DisplayBounds.w <= life->width)
		life->width = life->DisplayBounds.w;
	if (life->DisplayBounds.h <= life->height)
		life->height = life->DisplayBounds.h;

	life->window = SDL_CreateWindow("Game of Life",
		life->width, life->height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (!life->window){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		cleanup(life);
		return;
	}
	if (SDL_GetWindowSizeInPixels(life->window, &life->width, &life->height) == false) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to get window size: %s\n", SDL_GetError());
		cleanup(life);
		return;
	}
	life->renderer = SDL_CreateRenderer(life->window, nullptr);
	if (!life->renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
		cleanup(life);
		return;
	}
	SDL_Log("width in px: %d\n", life->width);
	SDL_Log("height in px: %d\n", life->height);
	SDL_Log("Number of Displays: %d", life->display_num);
	generate_map(life);
}

//debug only
void print_life(t_life *life) {
for (int y = 0; y < life->ch; y++){
		for (int x = 0; x < life->cw; x++)
			putchar(life->curGen[y][x] ? '0' : ' ');
		putchar('\n');
	}
}

void	mainLoop(t_life *life) {
	bool paused = false;
	int decoupled_board = 0;

	if (!life->window || !life->renderer)
		return;
	while (life->running){
		SDL_Event event;
		SDL_Delay(2);
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_EVENT_QUIT
				|| (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)){
				life->running = false;
				}
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE){
				paused = paused ? false : true;
				paused ? SDL_Log("game is paused") : SDL_Log("game is unpaused");
			}
		}

		if (!paused && decoupled_board >= 30) {
			game_of_life(life);
			decoupled_board = 0;
		}
		decoupled_board++;
		draw_life(life);
	}
}

void	cleanup(t_life *life) {
	if (life->renderer)
		SDL_DestroyRenderer(life->renderer);
	if (life->window)
		SDL_DestroyWindow(life->window);
	if (life->curGen) {
		for (int i = 0; life->curGen[i]; i++)
			 free(life->curGen[i]);
		free(life->curGen);
	}
	if (life->nextGen) {
		for (int i = 0;life->nextGen[i]; i++)
				free(life->nextGen[i]);
		free(life->nextGen);
	}
	if (life->displays)
		SDL_free(life->displays);
	SDL_Quit();
}

int main(int argc, char **argv){
	t_life life = {0};
	life.running = true;

	if (argc < 3) {
		puts(FORMAT USAGE);
		return 1;
	}
	if (parse(atoi(argv[1]), atoi(argv[2]), &life)) {
		fputs("Invalid Arguments provided", stderr);
		return 1;
	}
	gameInit(&life);
	mainLoop(&life);
	cleanup(&life);
	return 0;
}