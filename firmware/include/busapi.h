/**
 * Bus API abstract class - busapi.h
 *
 * This file contains the abstract class for the Bus API.
 *
 * Author:  Mirko Lana
 * Date:    13/02/2024
 */

#ifndef __BUSAPI_H__
#define __BUSAPI_H__

#include "world.h"

#include <Arduino.h>
#include <vector>

using namespace std;

/**
 * This struct represents a route.
 */
struct route_t {
    String name;
    int minutes;
    World::Color color;

    route_t(String name="N/A",int minutes=-1, World::Color color=World::BLACK);
    bool is_valid();
};

/**
 * This class describes the Bus API.
 */
class BusAPI {
    public:
        virtual route_t get_route(String stop_id, String route_name) = 0;
        virtual vector<route_t> get_routes(String stop_id) = 0;

    protected:
        String make_request(String url);
        virtual World::Color get_route_color(String route_name) = 0;
};

#endif // __BUSAPI_H__
