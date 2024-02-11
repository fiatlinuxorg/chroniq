/**
 * Bus view class - busview.h
 *
 * This class is used to display the bus arrival time.
 *
 * Author:  Riccardo Segala, Mirko Lana
 * Date:    11/02/2024
 **/

#ifndef BUSVIEW_H
#define BUSVIEW_H

#include "view.h"
#include "flbustn.h"

class BusView : public View {
public:
    // Methods
    BusView(String stop_id, String bus_line);
    void draw(World* world) override;
    void update(World* world) override;
    void present(World* world) override;
    void click(World* world) override;
private:
    // Variables
    bool to_draw = true;
    unsigned long time;
    FLBusTN api;
    String stop_id, bus_line;

};

#endif
