/**
 * traquantopassa.in API interaction class - traquantopassa.cpp
 *
 * The class is used to get the next trips from a stop, given its ID.
 *
 * Aknowledgements:
 * - Matteo Contrini (https://github.com/matteocontrini) the creator of the
 *   traquantopassa.in website and API.
 *
 * Author:  Mirko Lana
 * Date:    11/02/2024
 */

#include "busapi.h"
#include "traquantopassa.h"
#include "world.h"

#include <Arduino.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <vector>

using namespace std;

/**
 * Get the next trip from a stop, given its ID and the name of the route.
 * 
 * @param stop_id The ID of the stop.
 * @param route_name The name of the route.
 * 
 * @return A route_t object, containing the name of the route, the minutes until
 *         the next trip and the color of the route.
 */
route_t TraQuantoPassa::get_route(String stop_id, String route_name) {
    auto routes = get_routes(stop_id);

    for (auto route : routes) {
        if (is_same_route(route.name, route_name)) {
            return route;
        }
    }

    return route_t();
}

/**
 * Get the next trips from a stop, given its ID.
 * 
 * @param stop_id The ID of the stop.
 * 
 * @return A vector of route_t objects, containing the name of the route, the
 *         minutes until the next trip and the color of the route.
 */
vector<route_t> TraQuantoPassa::get_routes(String stop_id) {
    vector<route_t> routes;
    String response = make_request(API_URL + "stops/" + stop_id);

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
        String route_name = trips[i]["routeName"];
        int minutes = trips[i]["minutes"];
        World::Color color = get_route_color(route_name);

        routes.push_back(route_t(route_name, minutes, color));
    }

    return routes;
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
        return World::DARK_YELLOW;
    } else if (is_same_route("3", route_name)) {
        return World::RED;
    }

    return World::BLACK;
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


