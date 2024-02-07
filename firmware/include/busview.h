#ifndef BUSVIEW_H
#define BUSVIEW_H

#include "view.h"

class BusView : public View {
public:
    void draw(World* world) override;
    void update(World* world) override;
    void present(World* world) override;
private:
    bool to_draw = true;
};

#endif
