/* Trentino Trasporti API - ttapi.cpp
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
 *  I implemented ONLY useful endpoints for the project (in the future, I can
 *  add more endpoints if needed).
 *
 * */

#include "ttapi.h"
#include <Arduino.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>

route_t::route_t(String short_name="", int* route_id=nullptr) 
    : short_name(short_name), route_id(route_id) {

TTApi::TTApi() { }

route_t TTApi::get_route(String short_name) {
    String response;

    response = make_request("/routes", "?areas=23");
    if (response == "{}") {
        return route_t();
    }

    JSONVar json_root = JSON.parse(response);
    if (JSON.typeof(json_root) == "undefined") {
        Serial.print("Parsing input failed!");<Arduino_JSON.h>
        return route_t();
    } <Arduino_JSON.h>

    Serial.print(JSON.typeof(json_root))

    return route_t();
}

/**
 * This method makes a request to the Trentino Trasporti API, and returns the
 * response as a string.
 *
 * @param endpoint The endpoint to which the request should be made.
 * @return The response from the API as a string.
 */
String TTApi::make_request(String endpoint, String query) {
    HTTPClient http;
    String url = API_URL + endpoint + query;
    String response = "{}";
    
    http.begin(url.c_str());
    http.setAuthorization(API_USER, API_PASS);

    int httpCode = http.GET();
    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            response = http.getString();
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n",
                http.errorToString(httpCode).c_str());
    }

    http.end();
    return response;
}

