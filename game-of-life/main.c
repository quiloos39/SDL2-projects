#include <SDL2/SDL.h>
#include <stdbool.h>
#define N 30
#define RECTW 20
#define RECTH 20
#define OFFSET 2

int getState(int **grid, int w, int h, int x, int y)
{
    if (x > 0 && x < w && y > 0 && y < h)
        return grid[y][x];
    else
        return 0;
}
int countNeighbour(int **grid, int w, int h, int x, int y)
{
    int c = 0;
    if (getState(grid, w, h, x + 1, y))
        c++;
    if (getState(grid, w, h, x - 1, y))
        c++;
    if (getState(grid, w, h, x, y + 1))
        c++;
    if (getState(grid, w, h, x, y - 1))
        c++;
    if (getState(grid, w, h, x + 1, y + 1))
        c++;
    if (getState(grid, w, h, x + 1, y - 1))
        c++;
    if (getState(grid, w, h, x - 1, y + 1))
        c++;
    if (getState(grid, w, h, x - 1, y - 1))
        c++;
    return c;
}

int main (int argc, char** argv)
{
    SDL_Window *window = NULL;
    window = SDL_CreateWindow("Game-Of-Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 640, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int **grid = malloc(sizeof(int *) * N);
    int y,x;
    int r;
    srand(0);

    for (y = 0; y < N; y++)
    {
        grid[y] = malloc(sizeof(int) * N);
        for (x = 0; x < N; x++)
        {
            r = rand()%100;
            if (r%5 == 0)
                grid[y][x] = 1;
            else
                grid[y][x] = 0;
        }
    }

    int **tmpGrid = malloc(sizeof(int *) * N);

    SDL_Event event;
    bool quit = false;
    int c;


    while (!quit)
    {
        //Backing up grid.
        for (y = 0; y < N; y++)
        {
            tmpGrid[y] = malloc(sizeof(int) * N);
            memcpy(tmpGrid[y], grid[y], sizeof(int) * N);
        }

        // Physics
        for (x = 0; x < N; x++)
        {
            for (y = 0; y < N; y++)
            {
                c = countNeighbour(tmpGrid, N, N, x, y);
                if (grid[y][x])
                {
                    if (c < 2)
                        grid[y][x] = 0;
                    else if (c == 2 || c == 3)
                        grid[y][x] == 1;
                    else if (c > 3)
                        grid[y][x] = 0;
                }
                else
                if (c == 3)
                    grid[y][x] = 1;
            }
        }

        //Rendering
        SDL_SetRenderDrawColor(renderer, 31, 31, 31, 255);
        SDL_RenderClear(renderer);
        for (y = 0; y < N; y++)
        {
            for (x = 0; x < N; x++)
            {
                SDL_Rect rect = {x*RECTW, y*RECTH, RECTW - OFFSET, RECTH - OFFSET };
                if (grid[y][x])
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                else
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_Delay(100);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
