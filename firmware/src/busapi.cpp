/**
 * Implementation of the BusAPI class.
 * 
 * This class is responsible for making requests to the bus API and parsing the
 * response.
 * 
 * Author:  Mirko Lana
 * Date:    13/02/2024
 */

#include "busapi.h"
#include "world.h"

#include <Arduino.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <vector>

using namespace std;

/**
 * Constructor for the route_t struct.
 * 
 * @param name The name of the route.
 * @param minutes The minutes until the next trip.
 * @param color The color of the route.
 */
route_t::route_t(String name, int minutes, World::Color color) :
    name(name), minutes(minutes), color(color) { /* Empty constructor */ }

/**
 * Check if the route is valid.
 *
 * @return True if the route is valid, false otherwise.
 */
bool route_t::is_valid() {
    return name != "N/A" && minutes != -1;
}

/**
 * Make a request to the API.
 * 
 * @param url The URL to make the request to.
 * 
 * @return The response from the API.
 */
String BusAPI::make_request(String url) {
    HTTPClient http;
    String response = "{}";

    Serial.println("Making request to " + url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        Serial.println("Request successful");
        response = http.getString();
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n",
                http.errorToString(httpCode).c_str());
    }

    http.end();
    return response;
}
