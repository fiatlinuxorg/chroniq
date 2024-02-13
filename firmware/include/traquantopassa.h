/**
 * traquantopassa.in API interaction class - traquantopassa.h
 *
 * The class is used to get the next trips from a stop, given its ID.
 * 
 * The class is implemented in the traquantopassa.cpp file.
 *
 * Aknowledgements:
 * - Matteo Contrini (https://github.com/matteocontrini) the creator of the
 *   traquantopassa.in website and API.
 *
 * Author:  Mirko Lana
 * Date:    11/02/2024
 */

#ifndef __TRAQUANTOPASSA_H__
#define __TRAQUANTOPASSA_H__

#include "world.h"
#include "busapi.h"

#include <Arduino.h>
#include <vector>

using namespace std;



/**
 * This class is used to interact with the traquantopassa.in API.
 */
class TraQuantoPassa : public BusAPI {
    public:
        route_t get_route(String stop_id, String route_name) override;
        vector<route_t> get_routes(String stop_id) override;

    private:
        // Constants
        const String API_URL = "https://traquantopassa.in/api/";
        // Methods
        World::Color get_route_color(String route_name) override;
        bool is_same_route(String name1, String name2);
};

#endif // __TRAQUANTOPASSA_H__
