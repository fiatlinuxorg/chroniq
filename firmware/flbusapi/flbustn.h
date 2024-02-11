/**
 * FiatLinux BusTN API. - flbustn.h
 *
 * This class is used to interact with the FiatLinux BusTN API.
 *
 * Author:  Mirko Lana
 * Date:    11/02/2024
 **/

#ifndef __FLBUSTN_H__
#define __FLBUSTN_H__

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

    route_t(String name, int minutes, World::Color color);
    bool is_valid();
};

/**
 * This class is used to interact with the FiatLinux BusTN API.
 */
class FLBusTN {
    public:
        // Methods
        FLBusTN();
        route_t get_route(String stop_id, String route_name);
        vector<route_t> get_routes(String stop_id);

    private:
        // Constants
        const String API_URL = "https://fiatlinux.it/api/bustn";
        // Methods
        String make_request(String endpoint);
        World::Color get_route_color(String route_name);

};

#endif // __FLBUSTN_H__
