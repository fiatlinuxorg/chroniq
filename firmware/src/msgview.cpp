#include "msgview.h"

void MsgView::update(World* world) {
    to_draw = true;
}

void MsgView::draw(World* world) {
    if (to_draw) {
        world->draw_rect(0, 0, W_WIDTH, 7, World::Color::WHITE);
        world->draw_str(1, 1, "BISMY", 1, World::Color::BLUE);
        world->draw_str(1, 10, "HELLO, WORLD!", 1, World::Color::WHITE);
        to_draw = false;
    }
}

void MsgView::present(World* world) {
    to_draw = true;
}
