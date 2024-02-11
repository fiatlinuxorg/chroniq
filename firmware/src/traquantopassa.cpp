/*
 * This file contains the implementation of the TraQuantoPassa class.
 *
 */

#include "traquantopassa.h"
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
 * Constructor for the TraQuantoPassa class.
 */
TraQuantoPassa::TraQuantoPassa() { /* Empty constructor */ }

/**
 * Get the next trips from a stop, given its ID.
 * 
 * @param stop_id The ID of the stop.
 * @param route_name The name of the route to filter the results. If empty, all
 *                  the routes are returned.
 * 
 * @return A vector of route_t objects, containing the name of the route, the
 *         minutes until the next trip and the color of the route.
 */
vector<route_t> TraQuantoPassa::get_routes_from_stop(String stop_id,
        String route_name) {
    vector<route_t> routes;
    String response = make_request("stops/"+stop_id);

    if (response == "{}") {
        return routes;
    }

    JSONVar data = JSON.parse(response);
    if (JSON.typeof(data) == "undefined") {
        return routes;
    }

    JSONVar trips = data["directions"][0]["trips"];
    if (JSON.typeof(trips) == "undefined") {
        return routes;
    }

    for(int i = 0; i < trips.length(); ++i) {
        String _route_name = trips[i]["routeName"];
        if (route_name == "" || is_same_route(route_name, _route_name)) {
            int minutes = trips[i]["minutes"];
            routes.push_back(route_t(_route_name, minutes,
                        get_route_color(_route_name)));
        }
    }

    return routes;
}

/**
 * Make a request to the traquantopassa.in API.
 * 
 * @param endpoint The endpoint of the API.
 * @param query The query to append to the endpoint.
 * 
 * @return The response from the API.
 */
String TraQuantoPassa::make_request(String endpoint, String query) {
    HTTPClient http;
    String url = API_URL + endpoint + query;
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

    http.end();
    return response;
}

/**
 * Check if two route names are the same.
 * 
 * @param name1 The first route name.
 * @param name2 The second route name.
 * 
 * @return True if the two route names are the same, false otherwise.
 */
bool TraQuantoPassa::is_same_route(String name1, String name2) {
    if (name1 == name2) {
        return true;
    }

    return (name1 + "/") == name2 ? true : (name2 + "/") == name1;
}

/**
 * Get the color of a route.
 * 
 * @param route_name The name of the route.
 * 
 * @return The color of the route.
 */
World::Color TraQuantoPassa::get_route_color(String route_name) {
    if (is_same_route("5", route_name)) {
        return World::YELLOW;
    } else if (is_same_route("3", route_name)) {
        return World::RED;
    }

    return World::BLACK;
}
