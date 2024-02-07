#ifndef MSGVIEW_H
#define MSGVIEW_H

#include "common.h"
#include "view.h"

class MsgView : public View {
public:
    void update(World* world) override;
    void draw(World* world) override;
    void present(World* world) override;
private:
    bool to_draw = true;
};

#endif
