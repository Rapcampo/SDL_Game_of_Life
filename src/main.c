#include "../includes/life.h"
//life should have window x and y, map file to be parsed,
//TODO: implement screen pixel drawing

void	cleanup(t_life *life);

int parse(const int width, const int height, const int shader, t_life *life) {
	if (width < 640  || width > 3840 || height < 480  || height > 2160)
		return 1;
	else if (width % 10 != 0 || height % 10 != 0)
		return 1;
	life->req_width = width;
	life->req_height = height;
	life->height = height;
	life->width = width;
	life->shader = shader;
	return 0;
}

void	gameInit(t_life *life) {
	SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL: %s", SDL_GetError());
		return;
	}

	SDL_DisplayID primary = SDL_GetPrimaryDisplay();
	life->disp.displays = SDL_GetDisplays(&life->disp.display_num);
	SDL_GetDisplayUsableBounds(primary, &life->disp.DisplayBounds);
	if (life->disp.DisplayBounds.w < life->req_width)
		life->width = life->disp.DisplayBounds.w;
	if (life->disp.DisplayBounds.h < life->req_height)
		life->height = life->disp.DisplayBounds.h;

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
	generate_map(life);
}

void updateWindow(t_life *life) {
	int win_x = 0, win_y = 0;
	SDL_GetWindowSizeInPixels(life->window, &win_x, &win_y);
	float scale_x = (float)win_x / life->req_width;
	float scale_y = (float)win_y / life->req_height;
	float absolute_s = (scale_x < scale_y) ? scale_x : scale_y;

	if (absolute_s < 0.1f)
		absolute_s = 0.1f;
	SDL_SetRenderScale(life->renderer, absolute_s, absolute_s);
	life->view.scale = absolute_s;
	life->view.off_y = ((float)win_y / absolute_s - (float)life->req_height) * 0.5f;
	life->view.off_x = ((float)win_x / absolute_s - (float)life->req_width) * 0.5f;
}

void	mainLoop(t_life *life) {
	bool paused = false;
	int decoupled_board = 0;

	if (!life->window || !life->renderer)
		return;
	while (life->running){
		SDL_Event event;
		SDL_Delay(16);
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_EVENT_QUIT
				|| (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)){
				life->running = false;
				}
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE){
				paused = paused ? false : true;
				paused ? SDL_Log("game is paused") : SDL_Log("game is unpaused");
			}
			if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
				updateWindow(life);
		}

		if (!paused && decoupled_board >= 7) {
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
	if (life->disp.displays)
		SDL_free(life->disp.displays);
	SDL_Quit();
}

int main(int argc, char **argv){
	t_life life = {0};
	life.running = true;

	if (argc < 3 || argc > 4) {
		SDL_Log(FORMAT USAGE SHADERS);
		return 1;
	}
	if (argc == 3 && parse(atoi(argv[1]), atoi(argv[2]), 1, &life)) {
		fputs("Invalid Arguments provided\n", stderr);
		return 1;
	}
	else if (argc == 4 && parse(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), &life)) {
		fputs("Invalid Arguments provided\n", stderr);
		return 1;
	}

	gameInit(&life);
	mainLoop(&life);
	cleanup(&life);
	return 0;
}