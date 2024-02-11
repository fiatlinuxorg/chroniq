#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include "view.h"
#include "world.h"
#include "clock.h"

class ClockView : public View {
public:
    ClockView();
    void draw(World* world) override;
    void update(World* world) override;
    void present(World* world) override;
    void click(World* world) override;
private:
    Clock& clock;
    int hour;
    int min;

    unsigned long last_click = 0;

    bool to_update = true;
    World::Color colors[2];
};

#endif
