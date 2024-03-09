#include "world.h"

#include <ctype.h>

#define NUM_COLORS 16
const int COLOR_PALETTE[] = {
    ~0x0000,    // BLACK
    ~0xFFFF,    // WHITE
    ~0xF800,    // RED
    ~0x07E0,    // GREEN
    ~0x001F,    // BLUE
    ~0xFFE0,    // YELLOW
    ~0x8400,    // DARK_YELLOW
    ~0xF81F,    // MAGENTA
    ~0xFCA0,    // ORANGE
    ~0xFC9F,    // PINK
    ~0xC618,    // GREY
    ~0x3C8D,    // BROWN
    ~0x03E0,    // DARKGREEN
    ~0x001F,    // DARKBLUE
    ~0x7800,    // DARKRED
    ~0x7BEF,    // DARKGREY
};

World::World(TFT_eSPI* tft) : tft(tft){
    reset();
}

void World::step() {

    for(int i=width*(height)-1; i>=0; i--) {
        if (!is_empty(i) && !is_wall(i)) {
            switch(get_type(i)) {
                case CellType::SAND: {
                    if(i<width*(height-1)) {
                        if(is_empty(i+width) || (get_type(i+width) == CellType::WATER)) {
                            swap(i, i+width);
                        } else if(is_empty(i+width-1)) {
                            swap(i, i+width-1);
                        } else if((i+width+1)<width*height && is_empty(i+width+1)) {
                            swap(i, i+width+1);
                        }
                    }
                } break;

                case CellType::WATER: {
                    
                    if(i<width*(height-1) && is_empty(i+width)) {
                        swap(i, i+width);
                    } else if(i<width*(height-1) && is_empty(i+width-1)) {
                        swap(i, i+width-1);
                    } else if(i<width*(height-1) && (i+width+1)<width*height && is_empty(i+width+1)) {
                        swap(i, i+width+1);
                    } else if(is_empty(i-1)) {
                        swap(i, i-1);
                    } else if((i+1)<width*height && is_empty(i+1)) {
                        swap(i, i+1);
                    } 

                    if(get_type(i+width) == CellType::SAND) {
                        set(i+width,
                                (get(i+width) & 0b01000011) | Color::DARK_YELLOW << 2);
                    }
                    if(get_type(i-1) == CellType::SAND) {
                        set(i-1,
                                (get(i-1) & 0b01000011) | Color::DARK_YELLOW << 2);
                    }
                    if(get_type(i+1) == CellType::SAND) {
                        set(i+1,
                                (get(i+1) & 0b01000011) | Color::DARK_YELLOW << 2);
                    }

                } break;
            }
        }
    }
}

void World::draw() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (!is_empty(x, y)) {
                short color = COLOR_PALETTE[(get(x + y*width) >> 2)%NUM_COLORS];
                tft->fillRect(x*WIDTH/width, y*HEIGHT/height, WIDTH/width, HEIGHT/height, color);
            } else {
                tft->fillRect(x*WIDTH/width, y*HEIGHT/height, WIDTH/width, HEIGHT/height, BG_COLOR);
            }
        }
    }
}

void World::reset() {
    for (int i = 0; i < width*height; i++) {
        grid[i] = 0;
    }
}

char World::get(int idx) {
    return grid[idx];
}

void World::set(int idx, char val) {
    grid[idx] = val;
}

void World::swap(int i1, int i2) {
    char tmp = grid[i1];
    grid[i1] = grid[i2];
    grid[i2] = tmp;
}

bool World::is_empty(int idx) {
    return (grid[idx] & 0b11) == 0;
}
bool World::is_empty(int x, int y) {
    return is_empty(x + y*width);
}

bool World::is_all_empty() {
    for(int i=0; i<width*height; i++) {
        if(!is_empty(i))
            return false;
    }
    return true;
}

bool World::is_wall(int idx) {
    return (grid[idx]>>7)!=0;
}

enum World::CellType World::get_type(int idx) {
    return (enum CellType)(grid[idx] & 0b11);
}

char World::gen_cell(enum World::Color color, enum World::CellType type) {
    return (type) | (color << 2);
}

char World::gen_wall_cell(enum World::Color color) {
    return 0x01 | (color<<2) | (1 << 7);
}

void World::draw_font(int x, int y, const char* buf, int scale, enum World::Color color) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            if(x+j>=width) {
                continue;
            }
            if (buf[i] & (1 << (7-j))) {
                for (int k = 0; k < scale; k++) {
                    for (int l = 0; l < scale; l++) {
                        grid[x+j*scale + k + (y+i*scale + l)*width] = gen_wall_cell(color);
                    }
                }
            }
        }
        if(y+i>=height) {
            break;
        }
    }
}

int World::draw_char(int x, int y, char c, int scale, enum World::Color color) {
    c = toupper(c);
    int font_idx=-1;
    for (int i = 0; FONTSET_STR[i]; i++) {
        if (c == FONTSET_STR[i]) {
            font_idx = i;
            break;
        }
    }

    if(font_idx==-1) {
        return -1;
    }
    
    draw_font(x, y, FONTSET+font_idx*5, scale, color);
    return 0;
}

int World::draw_str(int x, int y, const char* str, int scale, enum World::Color color) {
    int i=0;
    while(str[i]) {
        if(x+4*scale+1>=width) {
            break;
        }
        int res = draw_char(x, y, str[i], scale, color);
        if(res == -1) {
            x += 3;
        } else {
            x += 4*scale+1;
        }
        i++;
    }
    return i;
}

void World::draw_rect(int x, int y, int w, int h, enum World::Color color) {
    for(int i=0; i<w; i++) {
        for(int j=0; j<h; j++) {
            if(x+i>=width || y+j>=height) {
                continue;
            }
            grid[x+i + (y+j)*width] = gen_wall_cell(color);
        }
    }
    grid[x+y*width] = 0;
    grid[x+w-1 + y*width] = 0;
    grid[x+(h+1)*width] = 0;
    grid[x+w-1 + (h+1)*width] = 0;
}

void World::flush() {
    remove_walls();
    while(!is_all_empty()) {
        step();
        draw();
        step();
        draw();
        for(int j=0; j<width; j++) {
            set(j + (height-1)*width, 0);
        }
    }
}

void World::remove_rand_wall() {
    bool removed = false;
    for(int i=0; i<20 && !removed; i++) {
        int idx = random(width*height);
        if(is_wall(idx)) {
            grid[idx] = grid[idx] ^ (1<<7);
            removed=true;
        }
    }
}

void World::remove_walls() {
    for(int i=0; i<width*height; i++) {
        if(is_wall(i))
            grid[i] = grid[i] ^ (1<<7);
    }
}


static void ledcAnalogWrite(uint8_t channel, uint32_t value,
        uint32_t valueMax) {
    // calculate duty, 4095 from 2 ^ 12 - 1
    uint32_t duty = (4095 / valueMax) * min(value, valueMax);

    Serial.print("Setting duty to: ");
    Serial.println(duty);

    // write duty to LEDC
    ledcWrite(channel, duty);
}

/**
 * Set the back light object
 * 
 * @param brightness 
 * @param fade 
 */
extern void set_backlight(bool on, bool fade) {
    uint8_t brightness = on ? 255 : 0;
    if (fade) {
        if (brightness == 0) {
            Serial.println("Fading out");
            for (int i = 255; i >= 0; i -= 5) {
                ledcAnalogWrite(0, i);
                delay(30);
            }
        } else {
            Serial.println("Fading in");
            for (int i = 0; i <= 255; i += 5) {
                ledcAnalogWrite(0, i);
                delay(30);
            }
        }
    } else {
        Serial.println("Setting brightness directly");

        ledcAnalogWrite(0, brightness);
    }
}
