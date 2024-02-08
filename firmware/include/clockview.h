#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include "view.h"
#include "world.h"

#include <WiFiUdp.h>
#include <NTPClient.h>

class ClockView : public View {
public:
    ClockView();
    void draw(World* world) override;
    void update(World* world) override;
    void present(World* world) override;
private:
    static WiFiUDP ntpUDP;
    static NTPClient timeClient;
    int hour;
    int min;
    bool to_update = true;
    World::Color colors[2];
};

#endif
