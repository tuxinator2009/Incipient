#include "Data.h"
#include "Display.h"
#include "Game.h"
#include "Types.h"

Data::GameCookie Data::cookie;

BodyPart Data::playerParts[24] =
{
  {{{110.0, 88.0}, {1.0, 0.0}, {0.0, 1.0}}, &Models::mouthOpen, nullptr, &Data::playerParts[1], {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, &Models::body, &Data::playerParts[0], &Data::playerParts[23], {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, &Models::tail, &playerParts[1], nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal}
};

BodyPart Data::enemyParts[32]
{
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0xf984, 0xf984, 0xf984, 0xf984}, {16, 16, 8, 8}, BodyPart::State_Normal}
};

BodyPart Data::textParts[64]
{
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal},
  {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, {0, 0, 0}, 1.0, {0x3d17, 0x3d17, 0x3d17, 0x3d17}, {16, 16, 8, 8}, BodyPart::State_Normal}
};

Food Data::food[24] =
{
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}},
  {nullptr, {0x0000, 0x0000, 0x0000, 0x0000}}
};

Particle Data::particles[32] =
{
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},
  {{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0},{{0,0},{0,0},0,0,0,0}
};

Creature Data::player = {&Models::mouthClosed, &Data::playerParts[0], &Data::playerParts[23], 2.0, 0.1, 0, 0, 0, Creature::State_Wandering};
Creature Data::enemies[8] =
{
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering}
};
const Model *Data::enemyBodies[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
int Data::numAvailableEnemyParts = sizeof(enemyParts) / sizeof(BodyPart);
int Data::numEnemies = 0;
int Data::maxPlayerSize = 1;
int Data::playerSize = 1;
uint16_t Data::colors[4] = {0x3d17, 0x3d17, 0x3d17, 0x3d17};
uint8_t Data::alphas[4] = {16, 16, 8, 8};

Creature Data::text[16] =
{
  {nullptr, &Data::textParts[0], &Data::textParts[8], 0.0, 0.0, 0, 0, 0, Creature::State_Stationary},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering},
  {nullptr, nullptr, nullptr, 1.0, 0.1, 0, 0, 0, Creature::State_Wandering}
};

void Data::setupData()
{
  bool initCookie = !cookie.exists("INCIPIEN");
  cookie.begin("INCIPIEN", sizeof(cookie), (char*)&cookie);
  cookie.init();
  for (auto& part : playerParts)
  {
    if (part.previous != nullptr)
    {
      part.transform.xAxis = part.previous->transform.xAxis * part.scale;
      part.transform.yAxis = part.previous->transform.yAxis * part.scale;
      part.transform.offset = part.previous->transform.offset + VectorF(part.previous->model->link).projected(part.previous->transform.xAxis, part.previous->transform.yAxis);
    }
    else
    {
      part.transform.xAxis *= part.scale;
      part.transform.yAxis *= part.scale;
    }
  }
  for (auto& part : enemyParts)
  {
    if (part.previous != nullptr)
    {
      part.transform.xAxis = part.previous->transform.xAxis * part.scale;
      part.transform.yAxis = part.previous->transform.yAxis * part.scale;
      part.transform.offset = part.previous->transform.offset + VectorF(part.previous->model->link).projected(part.previous->transform.xAxis, part.previous->transform.yAxis);
    }
    else
    {
      part.transform.xAxis *= part.scale;
      part.transform.yAxis *= part.scale;
    }
  }
  for (auto& part : textParts)
  {
    if (part.previous != nullptr)
    {
      part.transform.xAxis = part.previous->transform.xAxis * part.scale;
      part.transform.yAxis = part.previous->transform.yAxis * part.scale;
      part.transform.offset = part.previous->transform.offset + VectorF(part.previous->model->link).projected(part.previous->transform.xAxis, part.previous->transform.yAxis);
    }
    else
    {
      part.transform.xAxis *= part.scale;
      part.transform.yAxis *= part.scale;
    }
  }
}

void Data::movePlayer(VectorF dir)
{
  player.head->transform.xAxis.slerp(dir, player.slerpAmount, player.head->scale);
  player.head->transform.yAxis.slerp({-dir.y, dir.x}, player.slerpAmount, player.head->scale);
  player.head->transform.offset -= player.head->transform.yAxis * player.speed;
}

void Data::rotatePlayer(VectorF dir)
{
  for (BodyPart *part = player.head; part != nullptr; part = part->next)
  {
    if (part->previous != nullptr)
      part->transform.offset = part->previous->transform.offset + VectorF(part->previous->model->link).projected(part->previous->transform.xAxis, part->previous->transform.yAxis);
    part->transform.xAxis = dir;
    part->transform.yAxis = VectorF(-dir.y, dir.x);
  }
}

void Data::moveEnemy(int index, Point location)
{
  VectorF dir = enemies[index].head->transform.offset - location;
  dir.normalize();
  dir *= enemies[index].head->scale;
  enemies[index].head->transform.xAxis.slerp({dir.y, -dir.x}, enemies[index].slerpAmount, 1.0);
  enemies[index].head->transform.yAxis.slerp(dir, enemies[index].slerpAmount, 1.0);
  enemies[index].head->transform.offset -= enemies[index].head->transform.yAxis * enemies[index].speed;
}

void Data::transformCircle(const Circle &in, const Transform &transform, Circle &out)
{
  out.center.x = (int16_t)round(in.center.x * transform.xAxis.x + in.center.y * transform.yAxis.x + transform.offset.x);
  out.center.y = (int16_t)round(in.center.x * transform.xAxis.y + in.center.y * transform.yAxis.y + transform.offset.y);
  out.radius = round(in.radius * transform.xAxis.length());
}

void Data::updatePartTransforms(BodyPart &part, const fixed_t &slerpAmount)
{
  transformCircle(part.model->weakspot, part.transform, part.weakspot);
  if (part.previous != nullptr)
  {
    part.transform.offset = part.previous->transform.offset + VectorF(part.previous->model->link).projected(part.previous->transform.xAxis, part.previous->transform.yAxis);
    if (part.transform.yAxis != part.previous->transform.yAxis)
    {
      part.transform.xAxis.slerp(part.previous->transform.xAxis.normalized(), slerpAmount, part.scale);
      part.transform.yAxis.slerp(part.previous->transform.yAxis.normalized(), slerpAmount, part.scale);
    }
  }
}

void Data::updatePartState(BodyPart &part)
{
  int playerPartID = -1;
  int nextID = -1;
  if (&part >= &playerParts[0] && &part <= &playerParts[23])
    playerPartID = &part - &playerParts[0];
  switch (part.state & BodyPart::State_Flag)
  {
    case BodyPart::State_Normal:
      if (part.alphas[Shape::Triangle] > 8)
      {
        --part.alphas[Shape::Circle];
        --part.alphas[Shape::Triangle];
      }
      break;
    case BodyPart::State_Digesting:
      ++part.alphas[Shape::Circle];
      ++part.alphas[Shape::Triangle];
      if (part.alphas[Shape::Triangle] == 8 || part.alphas[Shape::Triangle] == 32)
      {
        part.state = BodyPart::State_Normal;
        if (part.next == nullptr)
        {
          if (playerPartID == -1)
          {
            addParticles(part.weakspot.center, 8, part.colors[Shape::Point]);
            return;
          }
          if (food[playerPartID].model != nullptr)
          {
            BodyPart *nextPlayerPart = nextAvailablePlayerPart();
            if (nextPlayerPart == nullptr)
            {
              addParticles(part.weakspot.center, 8, food[23].colors[Shape::Point]);
              Game::scorePoints(Game::SCORE_BONUS_CREATURE);
            }
            else
            {
              part.previous->next = nextPlayerPart;
              nextPlayerPart->previous = part.previous;
              nextPlayerPart->next = &part;
              part.previous = nextPlayerPart;
              nextPlayerPart->scale = 0.0;
              nextPlayerPart->transform.xAxis = VectorF(0.0, 0.0);
              nextPlayerPart->transform.yAxis = VectorF(0.0, 0.0);
              nextPlayerPart->transform.offset = part.transform.offset;
              nextPlayerPart->model = food[playerPartID].model;
              nextPlayerPart->colors[0] = food[playerPartID].colors[0];
              nextPlayerPart->colors[1] = food[playerPartID].colors[1];
              nextPlayerPart->colors[2] = food[playerPartID].colors[2];
              nextPlayerPart->colors[3] = food[playerPartID].colors[3];
              nextPlayerPart->alphas[0] = 16;
              nextPlayerPart->alphas[1] = 16;
              nextPlayerPart->alphas[2] = 8;
              nextPlayerPart->alphas[3] = 8;
              nextPlayerPart->state = BodyPart::State_Growing;
              ++playerSize;
              if (playerSize > maxPlayerSize)
                maxPlayerSize = playerSize;
            }
            food[playerPartID].model = nullptr;
          }
          else
          {
            addParticles(part.weakspot.center, 8, food[23].colors[Shape::Point]);
            Game::scorePoints(Game::SCORE_BONUS_BODYPART);
          }
        }
        else if (playerPartID != -1 && food[playerPartID].model != nullptr)
        {
          BodyPart *next = part.next;
          if (part.alphas[Shape::Triangle] == 8)
          {
            part.alphas[Shape::Circle] = 0;
            part.alphas[Shape::Triangle] = 0;
          }
          while ((next->state & BodyPart::State_Flag) != BodyPart::State_Normal)
            next = next->next;
          next->state = BodyPart::State_Digesting;
          nextID = next - &playerParts[0];
          food[nextID].model = food[playerPartID].model;
          food[nextID].colors[Shape::Point] = food[playerPartID].colors[Shape::Point];
          food[nextID].colors[Shape::Line] = food[playerPartID].colors[Shape::Line];
          food[nextID].colors[Shape::Circle] = food[playerPartID].colors[Shape::Circle];
          food[nextID].colors[Shape::Triangle] = food[playerPartID].colors[Shape::Triangle];
          food[playerPartID].model = nullptr;
        }
        else if (part.alphas[Shape::Triangle] == 32)
        {
          BodyPart *next = part.next;
          while ((next->state & BodyPart::State_Flag) != BodyPart::State_Normal)
            next = next->next;
          next->state = BodyPart::State_Digesting;
          if (playerPartID != -1)
            food[next - &playerParts[0]].colors[Shape::Point] = food[playerPartID].colors[Shape::Point];
        }
      }
      break;
    case BodyPart::State_Breaking:
      part.scale -= 0.1;
      if (part.scale < 0.1)
      {
        part.next->transform.offset = part.transform.offset;
        part.previous->next = part.next;
        part.next->previous = part.previous;
        part.model = nullptr;
        part.previous = nullptr;
        part.next = nullptr;
        if (playerPartID == -1)
          ++numAvailableEnemyParts;
        else
          --playerSize;
      }
      else
      {
        part.transform.xAxis.normalize();
        part.transform.xAxis *= part.scale;
        part.transform.yAxis.normalize();
        part.transform.yAxis *= part.scale;
      }
      break;
    case BodyPart::State_Growing:
      part.scale += 0.1;
      if (part.scale >= 1.0)
      {
        part.scale = 1.0;
        part.transform.xAxis.normalize();
        part.transform.yAxis.normalize();
        part.state = BodyPart::State_Normal;
      }
      else
      {
        part.transform.xAxis.normalize();
        part.transform.xAxis *= part.scale;
        part.transform.yAxis.normalize();
        part.transform.yAxis *= part.scale;
      }
      break;
  }
}

void Data::updateCreatures()
{
  VectorF dir;
  player.t += player.tDir;
  if (player.t >= 128)
    player.tDir /= -4;
  else if (player.t == 0)
  {
    player.tDir = 0;
    if ((player.head->state & BodyPart::State_Counter) > 0)
      --player.head->state;
  }
  for (auto& part : playerParts)
  {
    if (part.model == nullptr)
      continue;
    updatePartTransforms(part, player.slerpAmount);
    updatePartState(part);
  }
  for (auto& enemy : enemies)
  {
    if (enemy.head == nullptr)
      continue;
    for (BodyPart *part = enemy.head; part != nullptr; part = part->next)
    {
      updatePartTransforms(*part, enemy.slerpAmount);
      updatePartState(*part);
    }
    enemy.t += enemy.tDir;
    if (enemy.t >= 128)
      enemy.tDir /= -4;
    else if (enemy.t == 0)
    {
      enemy.tDir = 0;
      if ((enemy.head->state & BodyPart::State_Counter) > 0)
        --enemy.head->state;
    }
    if (enemy.state == Creature::State_Stationary)
      continue;
    else if (enemy.state == Creature::State_Wandering)
      moveEnemy(&enemy - &enemies[0], {rand() % 188 + 16, rand() % 144 + 16});
    else if (enemy.state == Creature::State_Chasing)
    {
      if (playerParts[enemy.target].model == nullptr || (playerParts[enemy.target].state & BodyPart::State_Flag) == BodyPart::State_Breaking)
        enemy.state = Creature::State_Wandering;
      else
        moveEnemy(&enemy - &enemies[0], playerParts[enemy.target].weakspot.center);
    }
    else if (enemy.state == Creature::State_Fleeing)
    {
      enemy.head->transform.offset -= enemy.head->transform.yAxis * enemy.speed;
      --enemy.target;
      if (enemy.target == 0)
      {
        enemy.slerpAmount = 0.1;
        enemy.speed = 1.0;
        enemy.state = Creature::State_Wandering;
      }
    }
  }
  for (auto& part : textParts)
  {
    if (part.previous != nullptr)
    {
      part.transform.offset = part.previous->transform.offset + VectorF(part.previous->model->link).projected(part.previous->transform.xAxis, part.previous->transform.yAxis);
      part.transform.xAxis = part.previous->transform.xAxis;
      part.transform.yAxis = part.previous->transform.yAxis;
    }
  }
}

bool Data::circleCircleCollision(const Circle &c1, const Circle &c2)
{
  if (c1.center.x < 0 || c1.center.x >= 220 || c1.center.y < 0 || c1.center.y >= 176)
    return false;
  if (c2.center.x < 0 || c2.center.x >= 220 || c2.center.y < 0 || c2.center.y >= 176)
    return false;
  if (std::abs(c2.center.x - c1.center.x) <= c1.radius + c2.radius && std::abs(c2.center.y - c1.center.y) <= c1.radius + c2.radius)
    return true;
  //int16_t dist2 = (c2.center.x - c1.center.x) * (c2.center.x - c1.center.x) + (c2.center.y - c1.center.y) * (c2.center.y - c1.center.y);
  //int16_t r2 = (c1.radius + c2.radius) * (c1.radius + c2.radius);
  //if (dist2 <= r2)
  //  return true;
  return false;
}

bool Data::checkPlayerCollisions(int &enemyID, BodyPart *&part)
{
  if (player.t != 0)
    return false;
  if (player.head->alphas[Shape::Triangle] != 8)
    return false;
  for (auto &enemy : enemies)
  {
    if (enemy.head == nullptr)
      continue;
    part = enemy.head;
    if (part->next == enemy.tail || part == enemy.tail)
    {
      Circle link;
      transformCircle({part->model->link, part->model->weakspot.radius}, part->transform, link);
      if (circleCircleCollision(player.head->weakspot, link))
      {
        player.t = 32;
        player.tDir = 32;
        enemyID = &enemy - &enemies[0];
        return true;
      }
      continue;
    }
    for (part = part->next; part != enemy.tail; part = part->next)
    {
      if (circleCircleCollision(player.head->weakspot, part->weakspot))
      {
        player.t = 32;
        player.tDir = 32;
        enemyID = &enemy - &enemies[0];
        return true;
      }
    }
  }
  return false;
}

bool Data::checkEnemyCollisions(int &enemyID, BodyPart *&part)
{
  for (auto &enemy : enemies)
  {
    if (enemy.head == nullptr)
      continue;
    if (enemy.t != 0)
      continue;
    if (enemy.head->alphas[Shape::Triangle] != 8)
      continue;
    part = player.head;
    if (part->next == player.tail)
    {
      Circle link;
      transformCircle({part->model->link, part->model->weakspot.radius}, part->transform, link);
      if (circleCircleCollision(enemy.head->weakspot, link))
      {
        enemy.t = 32;
        enemy.tDir = 32;
        enemyID = &enemy - &enemies[0];
        return true;
      }
      continue;
    }
    for (part = part->next; part != player.tail; part = part->next)
    {
      if (circleCircleCollision(enemy.head->weakspot, part->weakspot))
      {
        enemy.t = 32;
        enemy.tDir = 32;
        enemyID = &enemy - &enemies[0];
        return true;
      }
    }
  }
  return false;
}

void Data::drawPlayer()
{
  if (player.head != nullptr)
    Display::drawCreature(player);
}

void Data::drawEnemies()
{
  for (auto &enemy : enemies)
  {
    if (enemy.head != nullptr)
      Display::drawCreature(enemy);
  }
}

void Data::drawParticles()
{
  for (auto &particle : particles)
  {
    if (particle.life > 0)
    {
      particle.distance += particle.velocity;
      Display::drawParticle(particle);
      particle.life -= 4;
    }
  }
}

void Data::drawText()
{
  for (auto &creature : text)
  {
    if (creature.head != nullptr)
      Display::drawCreature(creature, false);
  }
}

BodyPart *Data::nextAvailablePlayerPart()
{
  for (auto &part : playerParts)
  {
    if (part.model == nullptr)
      return &part;
  }
  return nullptr;
}

BodyPart *Data::nextAvailableEnemyPart()
{
  for (auto &part : enemyParts)
  {
    if (part.model == nullptr)
      return &part;
  }
  return nullptr;
}

BodyPart *Data::nextAvailableTextPart()
{
  for (auto &part : textParts)
  {
    if (part.model == nullptr)
      return &part;
  }
  return nullptr;
}

Particle *Data::nextAvailableParticle()
{
  for (auto &particle : particles)
  {
    if (particle.life == 0)
      return &particle;
  }
  return nullptr;
}

int Data::nextAvailableEnemy()
{
  for (auto &enemy : enemies)
  {
    if (enemy.head == nullptr)
      return &enemy - &enemies[0];
  }
  return -1;
}

void Data::setColors(uint16_t points, uint16_t lines, uint16_t circles, uint16_t triangles)
{
  colors[Shape::Point] = points;
  colors[Shape::Line] = lines;
  colors[Shape::Circle] = circles;
  colors[Shape::Triangle] = triangles;
}

void Data::setAlphas(uint8_t points, uint8_t lines, uint8_t circles, uint8_t triangles)
{
  alphas[Shape::Point] = points;
  alphas[Shape::Line] = lines;
  alphas[Shape::Circle] = circles;
  alphas[Shape::Triangle] = triangles;
}

void Data::addChar(char ch, int index, fixed_t scale)
{
  BodyPart *part = nextAvailableTextPart();
  if (part == nullptr)
    return;
  if (text[index].head == nullptr)
  {
    text[index].head = part;
    text[index].tail = part;
    part->previous = nullptr;
    part->next = nullptr;
  }
  else
  {
    text[index].tail->next = part;
    part->previous = text[index].tail;
    text[index].tail = part;
  }
  part->transform.xAxis = VectorF(scale, 0.0);
  part->transform.yAxis = VectorF(0.0, scale);
  part->scale = scale;
  part->colors[0] = colors[0];
  part->colors[1] = colors[1];
  part->colors[2] = colors[2];
  part->colors[3] = colors[3];
  part->alphas[0] = alphas[0];
  part->alphas[1] = alphas[1];
  part->alphas[2] = alphas[2];
  part->alphas[3] = alphas[3];
  if (ch >= 'a' && ch <= 'z')
    part->model = Models::models[ch - 'a'];
  else if (ch >= 'A' && ch <= 'Z')
    part->model = Models::models[ch - 'A'];
  else if (ch >= '0' && ch <= '9')
    part->model = Models::models[ch - '0' + 26];
  else
    part->model = &Models::fontSpace;
}

void Data::addText(const char *string, int index, Point location, fixed_t scale)
{
  clearText(index);
  for (const char *pChar = string; *pChar != '\0'; ++pChar)
    addChar(*pChar, index, scale);
  if (text[index].head == nullptr)
    return;
  for (BodyPart *part = text[index].head; part != nullptr; part = part->next)
  {
    if (part->previous == nullptr)
      part->transform.offset = location;
    else
      part->transform.offset = part->previous->transform.offset + VectorF(part->previous->model->link) * scale;
  }
  text[index].head->transform.xAxis = VectorF(scale, 0.0);
  text[index].head->transform.yAxis = VectorF(0.0, scale);
  text[index].head->scale = scale;
}

void Data::addNumber(int number, int max, int index, Point location, fixed_t scale, bool showZeros)
{
  clearText(index);
  while (max > 0)
  {
    int value = (number / max) % 10;
    if (text[index].head != nullptr || value > 0 || showZeros || max == 1)
      addChar('0' + value, index, scale);
    max /= 10;
  }
  if (text[index].head == nullptr)
    return;
  for (BodyPart *part = text[index].head; part != nullptr; part = part->next)
  {
    if (part->previous == nullptr)
      part->transform.offset = location;
    else
      part->transform.offset = part->previous->transform.offset + VectorF(part->previous->model->link) * scale;
  }
  text[index].head->transform.xAxis = VectorF(scale, 0.0);
  text[index].head->transform.yAxis = VectorF(0.0, scale);
  text[index].head->scale = scale;
}

void Data::clearText(int index)
{
  BodyPart *part = text[index].head;
  BodyPart *next;
  while (part != nullptr)
  {
    next = part->next;
    part->model = nullptr;
    part->previous = nullptr;
    part->next = nullptr;
    part = next;
  }
  text[index].head = nullptr;
  text[index].tail = nullptr;
}

void Data::changeTextColors(int index)
{
  BodyPart *part = text[index].head;
  while (part != nullptr)
  {
    part->colors[0] = colors[0];
    part->colors[1] = colors[1];
    part->colors[2] = colors[2];
    part->colors[3] = colors[3];
    part = part->next;
  }
}

void Data::changeTextAlphas(int index)
{
  BodyPart *part = text[index].head;
  while (part != nullptr)
  {
    part->alphas[0] = alphas[0];
    part->alphas[1] = alphas[1];
    part->alphas[2] = alphas[2];
    part->alphas[3] = alphas[3];
    part = part->next;
  }
}

void Data::changeTextColorsAndAlphas(int index)
{
  BodyPart *part = text[index].head;
  while (part != nullptr)
  {
    part->colors[0] = colors[0];
    part->colors[1] = colors[1];
    part->colors[2] = colors[2];
    part->colors[3] = colors[3];
    part->alphas[0] = alphas[0];
    part->alphas[1] = alphas[1];
    part->alphas[2] = alphas[2];
    part->alphas[3] = alphas[3];
    part = part->next;
  }
}

void Data::scaleText(int index, fixed_t scale)
{
  text[index].head->scale = scale;
  text[index].head->transform.xAxis.x = scale;
  text[index].head->transform.yAxis.y = scale;
}

int Data::addEnemy(int index, Point location, const Model *mouthOpen, const Model *mouthClosed, const Model *bodyModel, const Model *tailModel, int bodyCount)
{
  if (index == -1)
    index = nextAvailableEnemy();
  if (index == -1)
    return -1;
  if (bodyCount + 2 > numAvailableEnemyParts)
    return -1;
  numAvailableEnemyParts -= bodyCount + 2;
  clearEnemy(index);
  BodyPart *head = nextAvailableEnemyPart();
  BodyPart *tail;
  BodyPart *previous = head;
  head->model = mouthOpen;
  ++numEnemies;
  while (bodyCount--)
  {
    BodyPart *body = nextAvailableEnemyPart();
    body->previous = previous;
    previous->next = body;
    body->model = bodyModel;
    body->transform.xAxis = VectorF(1.0, 0.0);
    body->transform.yAxis = VectorF(0.0, 1.0);
    body->transform.offset = previous->transform.offset + previous->model->link;
    transformCircle(bodyModel->weakspot, body->transform, body->weakspot);
    body->scale = 1.0;
    for (int i = 0; i < 4; ++i)
    {
      body->colors[i] = colors[i];
      body->alphas[i] = alphas[i];
    }
    body->state = BodyPart::State_Normal;
    previous = body;
  }
  tail = nextAvailableEnemyPart();
  tail->model = tailModel;
  head->previous = nullptr;
  previous->next = tail;
  tail->previous = previous;
  tail->next = nullptr;
  for (int i = 0; i < 4; ++i)
  {
    head->colors[i] = colors[i];
    head->alphas[i] = alphas[i];
    tail->colors[i] = colors[i];
    tail->alphas[i] = alphas[i];
  }
  head->transform.xAxis = VectorF(1.0, 0.0);
  head->transform.yAxis = VectorF(0.0, 1.0);
  head->transform.offset = location;
  head->scale = 1.0;
  head->state = BodyPart::State_Normal;
  tail->transform.xAxis = VectorF(1.0, 0.0);
  tail->transform.yAxis = VectorF(0.0, 1.0);
  tail->transform.offset = location;
  tail->scale = 1.0;
  tail->state = BodyPart::State_Normal;
  enemies[index] = {mouthClosed, head, tail, 1, 0.1, 0, 0, 0, Creature::State_Wandering};
  enemyBodies[index] = bodyModel;
  return index;
}

void Data::clearEnemy(int index)
{
  if (enemies[index].head != nullptr)
    --numEnemies;
  BodyPart *part = enemies[index].head;
  BodyPart *next;
  while (part != nullptr)
  {
    next = part->next;
    part->model = nullptr;
    part->previous = nullptr;
    part->next = nullptr;
    part = next;
    ++numAvailableEnemyParts;
  }
  enemies[index].head = nullptr;
  enemies[index].tail = nullptr;
  enemyBodies[index] = nullptr;
}

void Data::addParticles(VectorF offset, int count, uint16_t color)
{
  while (count--)
  {
    Particle *particle = nextAvailableParticle();
    uint8_t angle = rand() % 256;
    if (particle == nullptr)
      return;
    particle->offset = offset;
    particle->vector = VectorF(cosTable[angle], sinTable[angle]);
    particle->velocity = (((rand() >> 5) & 0xF) + 1) / 4.0;
    particle->distance = 0;
    particle->color = color;
    particle->life = 64;
  }
}

void Data::scaleEnemy(int index, fixed_t scale)
{
  enemies[index].head->scale = scale;
  enemies[index].head->transform.xAxis.normalize();
  enemies[index].head->transform.xAxis *= scale;
  enemies[index].head->transform.yAxis.normalize();
  enemies[index].head->transform.yAxis *= scale;
}

void Data::scalePlayer(fixed_t scale)
{
  player.head->scale = scale;
  player.head->transform.xAxis.normalize();
  player.head->transform.xAxis *= scale;
  player.head->transform.yAxis.normalize();
  player.head->transform.yAxis *= scale;
}

void Data::respawnPlayer()
{
  playerParts[0] = {{{110.0, 88.0}, {1.0, 0.0}, {0.0, 1.0}}, &Models::mouthOpen, nullptr, &Data::playerParts[1], {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal};
  playerParts[1] = {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, &Models::body, &Data::playerParts[0], &Data::playerParts[23], {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal};
  playerParts[23] = {{{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, &Models::tail, &playerParts[1], nullptr, {0, 0, 0}, 1.0, {0x07e0, 0x07e0, 0x07e0, 0x07e0}, {16, 16, 8, 8}, BodyPart::State_Normal};
  player = {&Models::mouthClosed, &Data::playerParts[0], &Data::playerParts[23], 2.0, 0.1, 0, 0, 0, Creature::State_Wandering};
}
