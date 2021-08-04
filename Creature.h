#ifndef CREATURE_H
#define CREATURE_H

#include <Pokitto.h>
#include "FixedPoints.h"
#include "FixedPointsCommon.h"

class Creature
{
  public:
    initCreature(const Model *mouthOpen, const Model *mouthClosed, const Model *segment, const Model *tail)
  private:
    const Model *mouthClosed;
    BodyPart *head;
    BodyPart *tail;
    Circle offscreen;
    uint8_t t;
};

#endif //CREATURE_H
