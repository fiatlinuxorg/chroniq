#include "clockview.h"
#include <arduino.h>

ClockView::ClockView() {

    colors[0] = World::Color::YELLOW;
    colors[1] = World::Color::DARK_YELLOW;

    WiFiUDP ntpUDP;
    timeClient = new NTPClient(ntpUDP, "ntp1.inrim.it", 3600, 0);
    timeClient->begin();

    hour = timeClient->getHours();
    min = timeClient->getMinutes();
}

void ClockView::draw(World* world) {

    if (timeClient->getHours() != hour || timeClient->getMinutes() != min) {
        to_update = true;
        hour = timeClient->getHours();
        min = timeClient->getMinutes();
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
}

void ClockView::present(World* world) {
    to_update = true;
}
