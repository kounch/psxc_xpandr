/*
BSD 2-Clause License

Copyright (c) 2019, kounch
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include "display_xpandr.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Bad args received!!. Arg Count:%i\n", argc);
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG);

    SDL_Window *window;
    window = SDL_CreateWindow("", SDL_WINDOW_X, SDL_WINDOW_Y, SDL_WINDOW_W, SDL_WINDOW_H,
                              SDL_WINDOW_FLAGS);
    if (window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 2;
    }
    else
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL)
        {
            printf("Could not create renderer: %s\n", SDL_GetError());
            return 3;
        }
        else
        {
#ifdef _WIN32
            if (argv[1][1] == ':')
#else
            if (argv[1][0] == '/')
#endif
            {
                SDL_Surface *image = IMG_Load(argv[1]);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);

                SDL_Event event;
                int run_display = 1;
                while (run_display)
                {
                    SDL_PollEvent(&event);
                    if (event.type == SDL_QUIT)
                    {
                        run_display = 0;
                    }
                    SDL_Delay(100);
                }
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(image);
            }
            SDL_DestroyRenderer(renderer);
        }
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
    SDL_Quit();
    return 0;
}
