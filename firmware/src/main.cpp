#include <WiFi.h>

#include "common.h"
#include "world.h"
#include "clockview.h"
#include "busview.h"
#include "msgview.h"

#define TOUCH_CS

#include <XPT2046_Touchscreen.h>

#define NUM_VIEWS 3

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

TFT_eSPI tft = TFT_eSPI();  

World world(&tft);
View* views[NUM_VIEWS];
int current_view_idx = 0;

const char *ssid = "<SSID>";
const char *password = "<PASSWORD>";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    
    views[0] = new ClockView();
    views[1] = new BusView();
    views[2] = new MsgView();

    ts.begin();
    ts.setRotation(1);

    tft.init();
    tft.setRotation(1);

}

void next_frame() {
    views[current_view_idx]->draw(&world);
    views[current_view_idx]->update(&world);
    world.draw();
    world.step();
}

void loop() {
    /*
    if(ts.touched()) {
        world.flush();
        current_view_idx = (current_view_idx + 1) % NUM_VIEWS;
        views[current_view_idx]->present(&world);
    }*/
    
    next_frame();
}
