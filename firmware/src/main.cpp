/**
 * Main file for the ESP32 - main.cpp
 *
 * This file is used to run the main loop of the ESP32.
 *
 * Author:  Riccardo Segala, Mirko Lana
 * Date:    11/02/2024
 **/

#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <vector>

#include "common.h"
#include "world.h"
#include "clock.h"
#include "clockview.h"
#include "busview.h"
#include "msgview.h"
#include "scheduler.h"

#define NUM_VIEWS 3

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

#define LCD_BACK_LIGHT_PIN 21

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

const unsigned long interval = 60000;

using namespace std;

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();  

World world(&tft);
View* views[NUM_VIEWS];
int current_view_idx = 0;
Scheduler scheduler(interval);


const char *ssid = "Wifi Goliardica";
const char *password = "Serata_scam123";

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void backlight() {
    for (int i = 5; i <= 255; i += 5) {
        ledcAnalogWrite(LEDC_CHANNEL_0, i);
        delay(30);
    }
}

void no_backlight() {
    for (int i = 255; i >= 0; i -= 5) {
        ledcAnalogWrite(LEDC_CHANNEL_0, i);
        delay(30);
    }
}

void next_frame() {
    views[current_view_idx]->draw(&world);
    views[current_view_idx]->update(&world);
    world.draw();
    world.step();
}

void set_bus_view() {
    current_view_idx = 1;
    next_frame();
    backlight();
}

void set_clock_view() {
    current_view_idx = 0;
    world.flush();
    views[current_view_idx]->present(&world);
}

void setup() {
    Serial.begin(115200);
    
    // Init touch screen
    mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);

    ts.begin(mySpi);
    ts.setRotation(1);

    tft.init();

    // Setting up the LEDC and configuring the Back light pin
    // NOTE: this needs to be done after tft.init()
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(LCD_BACK_LIGHT_PIN, LEDC_CHANNEL_0);

    tft.setRotation(1);
    tft.fillScreen(TFT_WHITE);

    // Init wifi
    Serial.println("\nConnecting");

    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // Init views
    views[0] = new ClockView();
    views[1] = new BusView("fiera", "5");
    views[2] = new BusView("fiera", "3");
    //views[2] = new MsgView();
    
    scheduler.add_task(sched_task(&set_bus_view, 7, 0));
    scheduler.add_task(sched_task(&set_clock_view, 9, 0));
    scheduler.add_task(sched_task(&no_backlight, 23, 0));

    ledcAnalogWrite(LEDC_CHANNEL_0, 255);
}

void loop() {
    Clock clock = Clock::get_instance();
    unsigned long current_time = millis(); 

    scheduler.check_tasks();

    if (ts.tirqTouched() && ts.touched()) {
        TS_Point p = ts.getPoint();

        if (p.x >= 2466) {
            current_view_idx = (current_view_idx + 1) % NUM_VIEWS;
            world.flush();
            views[current_view_idx]->present(&world);
        } else if (p.x <= 1233) {
            current_view_idx = (current_view_idx - 1 + NUM_VIEWS) % NUM_VIEWS;
            world.flush();
            views[current_view_idx]->present(&world);
        } else {
            views[current_view_idx]->click(&world);
        }

    }
    next_frame();
}
