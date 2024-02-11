#ifndef BUSVIEW_H
#define BUSVIEW_H

#include "view.h"
#include "traquantopassa.h"

class BusView : public View {
public:
    BusView(String stop_id, String bus_line);
    void draw(World* world) override;
    void update(World* world) override;
    void present(World* world) override;
private:
    bool to_draw = true;
    unsigned long time;
    TraQuantoPassa api;
    String stop_id, bus_line;
};

#endif
