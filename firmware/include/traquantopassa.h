/*
 * This file contains the class definition for the TraQuantoPassa class, which
 * is used to interact with the traquantopassa.in API.
 * 
 * The class is used to get the next trips from a stop, given its ID.
 * 
 * The class is implemented in the traquantopassa.cpp file.
 *
 * Aknowledgements:
 * - Matteo Contrini (https://github.com/matteocontrini) the creator of the
 *   traquantopassa.in website and API.
 */

#ifndef __TRAQUANTOPASSA_H__
#define __TRAQUANTOPASSA_H__

#include "world.h"

#include <Arduino.h>
#include <vector>

using namespace std;

/**
 * This struct is used to represent a route.
 */
struct route_t {
    String name;
    int minutes;
    World::Color color;

    route_t(String name, int minutes, World::Color color);
};

/**
 * This class is used to interact with the traquantopassa.in API.
 */
class TraQuantoPassa {
    public:
        // Methods
        TraQuantoPassa();
        vector<route_t> get_routes_from_stop(String stop_id,
                String route_name="");

    private:
        // Constants
        const char* API_URL = "https://traquantopassa.in/api/";
        // Methods
        String make_request(String endpoint, String query="");
        bool is_same_route(String name1, String name2);
        World::Color get_route_color(String route_name);
};

#endif // __TRAQUANTOPASSA_H__
