/* Trentino Trasporti API - ttapi.h
 *
 * This file contains the declaration of the TTApi class, which is the main
 * class of the Trentino Trasporti API implementation. This class is used to
 * interact with the Trentino Trasporti API, and to retrieve information about
 * the public transport services in the Trentino region.
 *
 * Thanks to Matteo Contrini, for the unofficial documentation of the Trentino
 * Trasporti API. (https://github.com/matteocontrini/trentino-trasporti-api)
 *
 * NOTE:
 * I implemented ONLY useful endpoints for the project (in the future, I can
 * add more endpoints if needed).
 *
 * */

#ifndef __TTAPI_H__
#define __TTAPI_H__

#include "Arduino.h"

struct route_t {
    String  short_name;
    int*    route_id;

    route_t(String short_name="", int* route_id=nullptr);
};

class TTApi {
    public:
        TTApi();
        route_t get_route(String short_name);

    private:
        // Constants
        const char* API_URL = "https://app-tpl.tndigit.it/gtlservice";
        const char* API_USER = "mittmobile";
        const char* API_PASS = "ecGsp.RHB3";
        // Methods
        String make_request(String endpoint, String query="");
};

#endif // __TTAPI_H__
