#include "clockview.h"
#include "clock.h"

#include <Arduino.h>

ClockView::ClockView() : clock(Clock::get_instance()) {
    colors[0] = World::Color::YELLOW;
    colors[1] = World::Color::DARK_YELLOW;

    hour = clock.get_hours();
    min = clock.get_minutes();
    
    Serial.println("time: " + String(hour) + ":" + String(min));
}

void ClockView::draw(World* world) {
    if (clock.get_hours() != hour || clock.get_minutes() != min) {
        to_update = true;
        hour = clock.get_hours();
        min = clock.get_minutes();
        Serial.println("time: " + String(hour) + ":" + String(min));
        world->flush();
    }

    if (to_update) {
        world->draw_font(1, 1, (char*)FONTSET+(hour/10)*5, 2, World::YELLOW);
        world->draw_font(10, 1, (char*)FONTSET+(hour%10)*5, 2, World::YELLOW);
        world->draw_font(14, 12, (char*)FONTSET+(min/10)*5, 2, World::YELLOW);
        world->draw_font(14+9, 12, (char*)FONTSET+(min%10)*5, 2, World::YELLOW);
        to_update = false;
    }
}

void ClockView::update(World* world) {
    if(random(40)==0)
        world->remove_rand_wall();

    if (last_click != 0 && millis() - last_click > 1000) {
        world->flush();
        to_update = true;
        last_click = 0;
    }
}

void ClockView::present(World* world) {
    to_update = true;
}

void ClockView::click(World* world) {
    world->set(random(W_WIDTH), 
            world->gen_cell(World::Color::BLUE, World::CellType::WATER));
    last_click = millis();
}
