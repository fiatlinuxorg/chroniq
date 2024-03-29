/**
 * Bus View class - busview.cpp
 *
 * This file contains the implementation of the Bus View class.
 *
 * Author:  Mirko Lana
 * Date:    11/02/2024
 */

#include "busview.h"
//#include "flbustn.h"
#include "traquantopassa.h"
#include "common.h"
#include "world.h"

#include <vector>

using namespace std;

/**
 * Constructor for the BusView class.
 *
 * @param stop_id The ID of the stop.
 * @param bus_line The name of the bus line.
 */
BusView::BusView(String stop_id, String bus_line)
    : stop_id(stop_id), bus_line(bus_line) {
    time = millis();
    
//  NOTES: In future, the BusView API will be set by the user inside a
//  configuration file. For now, we manually set it to TraQuantoPassa.
//
//  api = new FLBusTN(); // This is the FiatLinux BusTN API.
    api = new TraQuantoPassa();
}

/**
 * Update the view.
 *
 * @param world The world to update.
 */
void BusView::update(World* world) {
    if (millis() - time > 5000) {
        time = millis();
        to_draw = true;
    }
}

/**
 * Draw the view.
 *
 * @param world The world to draw on.
 */
void BusView::draw(World* world) {
    if (to_draw) {
        route_t route = api->get_route(stop_id, bus_line);

        if (!route.is_valid()) {
            world->draw_str(2, 2, "PORCO", 1, World::WHITE);
            world->draw_str(2, 10, "DIO", 1, World::WHITE);
        } else {
            bool slash = false;

            if (route.name.indexOf("/") != -1) {
                slash = true;
                route.name.replace("/", "");
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

/**
 * Present the view.
 *
 * @param world The world to present on.
 */
void BusView::present(World* world) {
    to_draw = true;
}

/**
 * Handle a click on the view.
 *
 * @param world The world to handle the click on.
 */
void BusView::click(World* world) {
    // TODO: implement
}

/**
 * Destructor for the BusView class.
 */
BusView::~BusView() {
    delete api;
}
