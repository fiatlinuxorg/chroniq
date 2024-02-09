#include <iostream>
#include <memory>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "clock.h"

using namespace std;

unique_ptr<Clock> Clock::instance;
WiFiUDP Clock::ntp_udp;
/**
 * This method returns the instance of the Clock singleton.
 * 
 * If the instance does not exist, it creates it.
 * 
 * @return The instance of the Clock singleton.
 */
Clock& Clock::get_instance() {
    if (!instance) {
        instance = unique_ptr<Clock>(new Clock(ntp_udp, "ntp1.inrim.it"));
    }
    return *instance;
}

/**
 * This is the constructor of the Clock class.
 * 
 * It initializes the NTPClient with the given UDP object and NTP server.
 * 
 * @param udp The UDP object to use.
 * @param ntp_server The NTP server to use.
 */
Clock::Clock(WiFiUDP &udp, const char* ntp_server) : 
    ntp_client(udp, ntp_server, 3600, 0) {
    ntp_client.begin();
}

/**
 * This method returns the current hour.
 * 
 * @return The current hour.
 */
int Clock::get_hours() {
    ntp_client.update();
    return ntp_client.getHours();
}

/**
 * This method returns the current minute.
 * 
 * @return The current minute.
 */
int Clock::get_minutes() {
    ntp_client.update();
    return ntp_client.getMinutes();
}

/**
 * This method returns the current second.
 * 
 * @return The current second.
 */
int Clock::get_seconds() {
    ntp_client.update();
    return ntp_client.getSeconds();
}
