#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG);

    window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_FULLSCREEN);
    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL)
        {
            printf("Could not create renderer: %s\n", SDL_GetError());
            return 1;
        }
        else
        {
            SDL_Surface *image = IMG_Load(argv[1]);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            while (1)
            {
                SDL_Delay(1000);
            }
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(image);
            SDL_DestroyRenderer(renderer);
        }
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
    SDL_Quit();
    return 0;
}
