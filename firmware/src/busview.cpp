#include "busview.h"
#include "common.h"
#include "world.h"

#include <vector>

using namespace std;

BusView::BusView(String stop_id, String bus_line)
    : stop_id(stop_id), bus_line(bus_line) {
    time = millis();
}

void BusView::update(World* world) {
    if (millis() - time > 5000) {
        time = millis();
        to_draw = true;
    }
}

void BusView::draw(World* world) {
    if (to_draw) {
        auto routes = api.get_routes_from_stop(stop_id, bus_line);

        if (routes.empty()) {
            world->draw_str(2, 2, "PORCO", 1, World::WHITE);
            world->draw_str(10, 2, "DIO", 1, World::WHITE);
        } else {
            auto route = routes[0];
            String route_name = route.name;
            bool slash = false;

            if (route_name.indexOf("/") != -1) {
                slash = true;
                route_name.replace("/", "");
            }
            
            world->reset();
            world->draw_rect(2, 2, 8, 9, route.color);
            if (slash) {
                world->draw_rect(4, 3, 4, 1, World::WHITE);
                world->draw_str(4, 5, route.name.c_str(), 1, World::WHITE);
            } else {
                world->draw_str(4, 4, route.name.c_str(), 1, World::WHITE);
            }
            world->draw_str(11, 4, ("'" + String(route.minutes)).c_str(), 1,
                    route.minutes == 0 ? World::RED : World::WHITE);
            world->draw_str(2, 14, stop_id.c_str(), 1, World::WHITE);
        }
        to_draw = false;
    }
}

void BusView::present(World* world) {
    to_draw = true;
}
