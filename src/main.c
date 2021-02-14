#include "functions.h"

//the main function
int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if ((window = SDL_CreateWindow("Labyrinthe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthWindow, heightWindow, 0)) == NULL)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED)) == NULL)
    {
        printf("Error: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    gameLoop(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}