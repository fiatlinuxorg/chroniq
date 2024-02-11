#ifndef WORLD_H
#define WORLD_H

#include <TFT_eSPI.h> 
#include <SPI.h>

#include "common.h"

class World {
public:

    enum Color {
        BLACK = 0,
        WHITE = 1,
        RED = 2,
        GREEN = 3,
        BLUE = 4,
        YELLOW = 5,
        DARK_YELLOW = 6,
        MAGENTA = 7,
        ORANGE = 8,
        PINK = 9,
        GREY = 10,
        BROWN = 11,
        DARKGREEN = 12,
        DARKBLUE = 13,
        DARKRED = 14,
        DARKGREY = 15
    };

    enum CellType {
        EMPTY = 0,
        SAND = 1,
        WATER = 2
    };

    World(TFT_eSPI* tft);
    void step();
    void draw();
    void reset();

    char get(int idx);
    void set(int idx, char val);
    void swap(int i1, int i2);
    bool is_empty(int idx);
    bool is_empty(int x, int y);

    bool is_all_empty();

    bool is_wall(int idx);
    enum World::CellType get_type(int idx);

    void remove_rand_wall();
    void remove_walls();

    char gen_cell(enum World::Color color, enum World::CellType type);
    char gen_wall_cell(enum World::Color color);
    void draw_font(int x, int y, const char* buf, int scale, enum World::Color color);

    int draw_char(int x, int y, char c, int scale, enum World::Color color);
    int draw_str(int x, int y, const char* str, int scale, enum World::Color color);

    void draw_rect(int x, int y, int w, int h, enum World::Color color);
    void flush();

    

private:
    TFT_eSPI* tft;
    int grid[W_WIDTH*W_HEIGHT];
    int width = W_WIDTH;
    int height = W_HEIGHT;
};

#endif
