/**
 * FiatLinux BusTN API. - flbustn.cpp
 *
 * This class is used to interact with the FiatLinux BusTN API.
 *
 * Author:  Mirko Lana
 * Date:    11/02/2024
 **/

#include "flbustn.h"
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
 * Constructor for the FLBusTN class.
 */
FLBusTN::FLBusTN() { /* Empty constructor */ }

/**
 * Get the route for a specific stop and route name.
 * 
 * @param stop_id The ID of the stop.
 * @param route_name The name of the route.
 * 
 * @return The route for the stop and route name.
 */
route_t FLBusTN::get_route(String stop_id, String route_name) {
    String response = make_request("/" + stop_id + "/" + route_name);

    if (response == "{}") {
        return route_t("N/A", -1, World::Color::BLACK);
    }

    JSONVar data = JSON.parse(response);
    if (JSON.typeof(data) == "undefined") {
        Serial.println("Parsing input failed!");
        return route_t("N/A", -1, World::Color::BLACK);
    }

    String name = data["routeName"];
    int minutes = data["minutes"];
    World::Color color = get_route_color(route_name);
    
    return route_t(name, minutes, color);
}

/**
 * Get the routes for a specific stop.
 * 
 * @param stop_id The ID of the stop.
 * 
 * @return The routes for the stop.
 */
vector<route_t> FLBusTN::get_routes(String stop_id) {
    String response = make_request("/" + stop_id);

    Serial.println(response);

    if (response == "{}") {
        return vector<route_t>();
    }

    JSONVar data = JSON.parse(response);
    if (JSON.typeof(data) == "undefined") {
        Serial.println("Parsing input failed!");
        return vector<route_t>();
    }

    vector<route_t> routes;
    for (int i = 0; i < data.length(); i++) {
        String name = data[i]["routeName"];
        int minutes = data[i]["minutes"];
        World::Color color = get_route_color(name);

        routes.push_back(route_t(name, minutes, color));
    }

    return routes;
}

/**
 * Make a request to the FiatLinux BusTN API.
 * 
 * @param endpoint The endpoint of the API.
 * 
 * @return The response from the API.
 */
String FLBusTN::make_request(String endpoint) {
    HTTPClient http;
    String url = API_URL + endpoint;
    String response = "{}";
    
    http.begin(url.c_str());

    int httpCode = http.GET();
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
        response = http.getString();
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n",
                http.errorToString(httpCode).c_str());
    }

    Serial.println(response);

    http.end();
    return response;
}

/**
 * Get the color of the route.
 * 
 * @param route_name The name of the route.
 * 
 * @return The color of the route.
 */
World::Color FLBusTN::get_route_color(String route_name) {
    if (route_name == "5" || route_name == "5/") {
        return World::DARK_YELLOW;
    } else if (route_name == "3" || route_name == "3/") {
        return World::RED;
    }

    return World::BLACK;
}
