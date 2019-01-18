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
    TTF_Init();

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
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_FLAGS);
        if (renderer == NULL)
        {
            printf("Could not create renderer: %s\n", SDL_GetError());
            return 3;
        }
        else
        {
            if (check_path(argv[1]))
            {
                SDL_Surface *image = IMG_Load(argv[1]);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

                SDL_Event event;
                int run_display = 1;
                while (run_display)
                {
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    show_logs(window);
                    SDL_RenderPresent(renderer);

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

void show_logs(SDL_Window *window)
{
    SDL_Surface *image;
    SDL_Rect box;

    image = SDL_GetWindowSurface(window);
    if (image == NULL)
    {
        printf("Could not get surface: %s\n", SDL_GetError());
        return;
    }
    box.w = image->w / 8 * 6;
    box.h = image->h / 2;
    box.x = image->w / 8;
    box.y = box.h / 4;
    if (check_file(LOG_FNAME))
    {
        draw_txt(window, box);
    }
}

void draw_txt(SDL_Window *window, SDL_Rect box)
{
    char **textlines;
    char **errlines;
    int i;

    textlines = (char **)malloc(LINES * sizeof(char *));
    //allocate space for each string
    for (i = 0; i < LINES; i++)
    {
        textlines[i] = (char *)malloc(LINEMAX * sizeof(char));
    }

    //Initialize text drawing area
    SDL_Rect text_box = box;
    text_box.x += 10;
    text_box.y += 10;

    //Read log
    read_txt(LOG_FNAME, textlines);
    for (i = 0; i < LINES; i++)
    {
        text_box.w = box.w;
        draw_txtline(window, &text_box, textlines[i]);
    }

    for (i = 0; i < LINES; i++)
    {
        free(textlines[i]);
    }
    free(textlines);
}

void draw_txtline(SDL_Window *window, SDL_Rect *text_box, char *txtline)
{
    TTF_Font *font;
    SDL_Renderer *renderer;
    SDL_Surface *surfaceMessage;
    SDL_Texture *Message;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect line_box;

    if (starts_with(txtline, "ERR"))
    {
        textColor.r = 255;
        textColor.g = 0;
        textColor.b = 0;
    }
    else if (starts_with(txtline, "WARN"))
    {
        textColor.r = 255;
        textColor.g = 255;
        textColor.b = 0;
    }

    font = TTF_OpenFont(FONT_PATH, 24);
    if (font == NULL)
    {
        printf("Error opening font: %s\n", TTF_GetError());
        return;
    }

    text_box->w = 0;
    text_box->h = TTF_FontLineSkip(font);
    if (strlen(txtline))
    {
        renderer = SDL_GetRenderer(window);
        surfaceMessage = TTF_RenderText_Blended_Wrapped(font, txtline, textColor, text_box->w);

        Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        text_box->w = surfaceMessage->w;
        text_box->h = surfaceMessage->h;
        SDL_RenderCopy(renderer, Message, NULL, text_box);
        SDL_DestroyTexture(Message);
        SDL_FreeSurface(surfaceMessage);
    }
    text_box->y += text_box->h;
    TTF_CloseFont(font);
}

void read_txt(char *fname, char *txtlines[])
{
    FILE *f;
    int cnt;
    int t;
    int i;
    char buf[LINEMAX];

    //Empty destination strings
    for (i = 0; i < LINES; i++)
    {
        txtlines[i][0] = '\0';
    }

    if (check_file(fname))
    {
        //Count lines in file
        cnt = 0;
        f = fopen(fname, "r");
        while (fgets(buf, LINEMAX, f))
        {
            cnt++;
        }
        //Get last lines of file
        t = 0;
        i = 0;
        fseek(f, 0, SEEK_SET);
        while (fgets(buf, LINEMAX, f))
        {
            buf[strcspn(buf, "\n\r")] = '\0'; //Remove end of line
            t++;
            if (t > cnt - LINES)
                strncpy(txtlines[i++], buf, LINEMAX * sizeof(char));
        }
        fclose(f);
    }
}

_Bool starts_with(const char *string, const char *prefix)
{
    while (*prefix)
    {
        if (*prefix++ != *string++)
            return 0;
    }
    return 1;
}
