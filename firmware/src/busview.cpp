#include "busview.h"
#include "common.h"
#include "world.h"


void BusView::update(World* world) {
    to_draw = true;
}

void BusView::draw(World* world) {
    if (to_draw) {
        world->draw_rect(2, 2, 8, 9, World::Color::RED);
        world->draw_font(4, 4, (char*)FONTSET+25, 1, World::Color::WHITE);
        to_draw = false;
    }
}

void BusView::present(World* world) {
    to_draw = true;
}
