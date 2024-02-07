#ifndef VIEW_H
#define VIEW_H

#include "world.h"

class View {
public:
    virtual void draw(World* world);
    virtual void update(World* world);
    virtual void present(World* world);
};

#endif
