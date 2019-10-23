#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>


#define AMOUNTOFRECTANGLE 10
#define RECTWIDTH 50
#define RECTHEIGHT 50

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    int **grid;
} Game;

Game *init()
{
    Game *g = malloc(sizeof(Game));
    g->window = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RECTWIDTH * AMOUNTOFRECTANGLE, RECTHEIGHT * AMOUNTOFRECTANGLE, SDL_WINDOW_SHOWN);
    g->renderer = SDL_CreateRenderer(g->window, -1, SDL_RENDERER_ACCELERATED);
    g->width = AMOUNTOFRECTANGLE;
    g->height = AMOUNTOFRECTANGLE;
    srand(time(NULL));
    int y,x;
    int **grid = malloc(sizeof(int *) * AMOUNTOFRECTANGLE);
    for (y = 0; y < AMOUNTOFRECTANGLE; y++)
    {
        grid[y] = malloc(sizeof(int) * AMOUNTOFRECTANGLE);
        for (x = 0; x < AMOUNTOFRECTANGLE; x++)
        {
            int r = rand()%100;
            if (r%20 == 0)
                grid[y][x] = 1;
            else
                grid[y][x] = 0;
        }
    }
    g->grid = grid;

    return g;
}


void draw(Game *g)
{
    SDL_SetRenderDrawColor(g->renderer, 31, 31, 31, 255);
    SDL_RenderClear(g->renderer);
    int y,x;
    for (y = 0; y < g->height; y++)
    {
        for (x = 0; x < g->width; x++)
        {
            SDL_Rect rect;
            rect.x = x*RECTWIDTH;
            rect.y = y*RECTHEIGHT;
            rect.w = RECTWIDTH;
            rect.h = RECTHEIGHT;
            if (g->grid[y][x] == 0)
                SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
            else
                SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(g->renderer, &rect);
        }
    }
    SDL_RenderPresent(g->renderer);
}

int getCell(int **grid, int w, int h, int y, int x)
{
    if (x > 0 && x < w && y > 0 && y < h)
        return grid[y][x];
    else
        return 0;
}

int countNeighbour(int **grid, int w, int h, int y, int x)
{
    int c = 0;
    if (getCell(grid, w, h, y + 1, x))
        c++;
    if (getCell(grid, w, h, y - 1, x))
        c++;
    if (getCell(grid, w, h, y, x + 1))
        c++;
    if (getCell(grid, w, h, y, x - 1))
        c++;
    if (getCell(grid, w, h, y + 1, x + 1))
        c++;
    if (getCell(grid, w, h, y + 1, x - 1))
        c++;
    if (getCell(grid, w, h, y - 1, x + 1))
        c++;
    if (getCell(grid, w, h, y - 1, x - 1))
        c++;
    return c;
}

void update(Game *g)
{
    int **prevGrid = malloc(sizeof(int) * AMOUNTOFRECTANGLE * AMOUNTOFRECTANGLE);
    memcpy(prevGrid, g->grid, sizeof(int) * AMOUNTOFRECTANGLE * AMOUNTOFRECTANGLE);
    int y,x;
    for (y = 0; y < RECTHEIGHT; y++)
    {
        for (x = 0; x < RECTWIDTH; x++)
        {
            int c = countNeighbour(prevGrid, g->width, g->height, y, x);
            printf("%d\t", c);
            if (g->grid[y][x])
            {
                if (c == 2 || c == 3)
                    g->grid[y][x] = 1;
                else if (c > 3)
                    g->grid[y][x] = 0;
                else if (c < 2)
                    g->grid[y][x] = 0;
            }
            else
            {
                if (c == 3)
                    g->grid[y][x] = 1;
            }
        }
        printf("\n");
    }
    printf("---------------------\n");
}

void gameLoop(Game *g)
{
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        update(g);
        draw(g);
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        //usleep(500000);
    }
}

void quit(Game *g)
{
    SDL_DestroyWindow(g->window);
    SDL_Quit();
}

int main (int argc, char** argv)
{
    Game *g = init();
    gameLoop(g);
    quit(g);
    return 0;
}