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

#include "busapi.h"
#include "world.h"

#include <Arduino.h>
#include <vector>

using namespace std;

/**
 * This class is used to interact with the FiatLinux BusTN API.
 */
class FLBusTN : public BusAPI {
    public:
        // Methods
        route_t get_route(String stop_id, String route_name) override;
        vector<route_t> get_routes(String stop_id) override;

    private:
        // Constants
        const String API_URL = "https://fiatlinux.it/api/bustn/";
        // Methods
        World::Color get_route_color(String route_name) override;

};

#endif // __FLBUSTN_H__
