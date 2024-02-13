/**
 * Clock class - clock.h
 *
 * This file contains the declaration of the Clock class.
 *
 * Author:  Mirko Lana
 * Date:    09/02/2024
 */
 

#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <memory>
#include <WiFiUdp.h>
#include <NTPClient.h>

using namespace std;

/**
 * This class is a singleton that provides the current time.
 *
 * It uses the NTPClient library to get the current time from an NTP server.
 * 
 */
class Clock {
    public:
        static Clock& get_instance();
        int get_hours();
        int get_minutes();
        int get_seconds();

    private:
        static unique_ptr<Clock> instance;
        static WiFiUDP ntp_udp;
        NTPClient ntp_client;
        Clock(WiFiUDP &udp, const char* ntp_server);
};

#endif // __CLOCK_H__
