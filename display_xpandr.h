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

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef __APPLE__
#define SDL_WINDOW_X SDL_WINDOWPOS_CENTERED
#define SDL_WINDOW_Y SDL_WINDOWPOS_CENTERED
#define SDL_WINDOW_W 1280
#define SDL_WINDOW_H 720
#define SDL_WINDOW_FLAGS SDL_WINDOW_SHOWN
#define SDL_RENDERER_FLAGS SDL_RENDERER_SOFTWARE

#define LOG_FNAME "/tmp/xpandr.log"
#define FONT_PATH "/Users/Shared/SST-Medium.ttf"

#elif _WIN32
#define SDL_WINDOW_X SDL_WINDOWPOS_CENTERED
#define SDL_WINDOW_Y SDL_WINDOWPOS_CENTERED
#define SDL_WINDOW_W 1280
#define SDL_WINDOW_H 720
#define SDL_WINDOW_FLAGS SDL_WINDOW_SHOWN
#define SDL_RENDERER_FLAGS 0

#define LOG_FNAME "C:\\Windows\\Temp\\xpandr.log"
#define FONT_PATH "C:\\Windows\\Fonts\\SST-Medium.ttf"

#else
#define SDL_WINDOW_X SDL_WINDOWPOS_UNDEFINED
#define SDL_WINDOW_Y SDL_WINDOWPOS_UNDEFINED
#define SDL_WINDOW_W 1280
#define SDL_WINDOW_H 720
#define SDL_WINDOW_FLAGS SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
#define SDL_RENDERER_FLAGS 0

#define LOG_FNAME "/tmp/xpandr.log"
#define FONT_PATH "/usr/sony/share/data/font/SST-Medium.ttf"
#endif

#ifdef _WIN32
#define check_path(path) (path[1] == ':')
#else
#define check_path(path) (path[0] == '/')
#endif

#ifdef _WIN32
#define check_file(filepath) (_access(filepath, 0) == 0)
#else
#define check_file(filepath) (access(filepath, R_OK) == 0)
#endif

#define LINEMAX 100
#define LINES 10

void show_logs(SDL_Window *window);
void draw_txt(SDL_Window *window, SDL_Rect box);
void draw_txtline(SDL_Window *window, SDL_Rect *text_box, char *txtline);
void read_txt(char *fname, char *textlines[]);
_Bool starts_with(const char *string, const char *prefix);
