#include "../includes/life.h"

void	cleanup(t_life *life);

void	print_gamestats(t_life *life) {
	int actualWidth, actualHeight;
	char **shader = (char *[]){"No shader", "Rainbow", "Purple", "Orange", NULL};
	SDL_GetWindowSize(life->window, &actualWidth, &actualHeight);
	SDL_Log(GRN"\tGame stats\n"CLR
		 GRN"Name:\e[0;39m Game of Life\n"
		 YLW"Window size:\e[0;39m %dx%d\n"
		 CYN"Video driver:\e[0;39m %s\n"
		 PRP"Renderer:\e[0;39m %s\n"
		 BLU"Requested resolution:\e[0;39m %dx%d\n"
		 BLK"Shader Option:\e[0;39m %s\n"
		 RED"Board size:\e[0;39m %dx%d"RST
		 , actualWidth, actualHeight
		 , SDL_GetCurrentVideoDriver()
		 , SDL_GetRendererName(life->renderer)
		 , life->requestedWidth, life->requestedHeight
		 , shader[life->shader]
		 , life->cols, life->row);
}

int parse(const int width, const int height, const int shader, t_life *life) {
	if (width < 640  || width > 3840 || height < 480  || height > 2160)
		return 1;
	else if (width % 10 != 0 || height % 10 != 0)
		return 1;
	life->requestedWidth = width;
	life->requestedHeight = height;
	life->height = height;
	life->width = width;
	life->shader = shader;
	return 0;

}

void	gameInit(t_life *life) {
	SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "vulkan,opengles2,opengl");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, RED"Couldn't initialize SDL: %s"RST, SDL_GetError());
		return;
	}

	SDL_DisplayID primary = SDL_GetPrimaryDisplay();
	life->disp.displays = SDL_GetDisplays(&life->disp.display_num);
	SDL_GetDisplayUsableBounds(primary, &life->disp.DisplayBounds);
	if (life->disp.DisplayBounds.w < life->requestedWidth)
		life->width = life->disp.DisplayBounds.w;
	if (life->disp.DisplayBounds.h < life->requestedHeight)
		life->height = life->disp.DisplayBounds.h;

	life->window = SDL_CreateWindow("Game of Life",
		life->width, life->height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
	if (!life->window){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, RED"Could not create window: %s\n"RST, SDL_GetError());
		cleanup(life);
		return;
	}
	if (SDL_GetWindowSizeInPixels(life->window, &life->width, &life->height) == false) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, RED"Failed to get window size: %s\n"RST, SDL_GetError());
		cleanup(life);
		return;
	}
	life->renderer = SDL_CreateRenderer(life->window, NULL);
	if (!life->renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, RED"Could not create renderer: %s\n"RST, SDL_GetError());
		cleanup(life);
		return;
	}
	generate_map(life);
}

void updateWindow(t_life *life) {
	int win_x = 0, win_y = 0;
	SDL_GetWindowSizeInPixels(life->window, &win_x, &win_y);
	float scale_x = (float)win_x / life->requestedWidth;
	float scale_y = (float)win_y / life->requestedHeight;
	float absolute_s = (scale_x < scale_y) ? scale_x : scale_y;

	if (absolute_s < 0.1f)
		absolute_s = 0.1f;
	SDL_SetRenderScale(life->renderer, absolute_s, absolute_s);
	life->view.scale = absolute_s;
	life->view.off_y = ((float)win_y / absolute_s - (float)life->requestedHeight) * 0.5f;
	life->view.off_x = ((float)win_x / absolute_s - (float)life->requestedWidth) * 0.5f;
}

void	mainLoop(t_life *life) {
	bool paused = false;
	int decoupled_board = 0;

	if (!life->window || !life->renderer)
		return;
	print_gamestats(life);
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
				paused ? SDL_Log(YLW"game is paused"RST) : SDL_Log(GRN"game is unpaused"RST);
			}
			if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
				updateWindow(life);
		}

		if (!paused && decoupled_board >= 7) {
			game_of_life(life);
			printf("\rCurrent Generation: \e[1;33m%zu\e[0m", life->generations);
			fflush(stdout);
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
		fputs(RED BLK"Invalid Arguments provided\n"RST, stderr);
		return 1;
	}
	else if (argc == 4 && parse(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), &life)) {
		fputs(RED BLK"Invalid Arguments provided\n"RST, stderr);
		return 1;
	}

	gameInit(&life);
	mainLoop(&life);
	cleanup(&life);
	return 0;
}
