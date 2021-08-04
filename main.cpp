#include <cstdint>
#include <Pokitto.h>
#include <File>
#include <LibAudio>
#include <LibSchedule>
#include "FixedPoints/FixedPoints.h"
#ifndef POK_SIM
#include <JoyHat.h>
#endif
#include "Display.h"
#include "FMSynthSource.h"
#include "music.h"
#include "models.h"

//mbed::DigitalOut jh_rumble(EXT0);
//mbed::DigitalIn jh_b1(EXT15);
//mbed::DigitalIn jh_b2(EXT14);
//mbed::AnalogIn jh_x(EXT2);
//mbed::AnalogIn jh_y(EXT1);

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

constexpr uint8_t BTN_MASK_UP = 1 << UPBIT;
constexpr uint8_t BTN_MASK_DOWN = 1 << DOWNBIT;
constexpr uint8_t BTN_MASK_LEFT = 1 << LEFTBIT;
constexpr uint8_t BTN_MASK_RIGHT = 1 << RIGHTBIT;
constexpr uint8_t BTN_MASK_A = 1 << ABIT;
constexpr uint8_t BTN_MASK_B = 1 << BBIT;
constexpr uint8_t BTN_MASK_C = 1 << CBIT;
constexpr uint8_t BTN_MASK_DPAD = BTN_MASK_UP | BTN_MASK_DOWN | BTN_MASK_LEFT | BTN_MASK_RIGHT;

constexpr uint8_t DIR_UP = 0;
constexpr uint8_t DIR_UP_RIGHT = 1;
constexpr uint8_t DIR_RIGHT = 2;
constexpr uint8_t DIR_DOWN_RIGHT = 3;
constexpr uint8_t DIR_DOWN = 4;
constexpr uint8_t DIR_DOWN_LEFT = 5;
constexpr uint8_t DIR_LEFT = 6;
constexpr uint8_t DIR_UP_LEFT = 7;
constexpr uint8_t DIR_JOY = 8;
constexpr uint8_t DIR_NONE = 255;

const SQ15x16 directions[8][2] = {{1.0,0.0},{0.707106,0.707106},{0.0,1.0},{-0.707106,0.707106},{-1.0,0.0},{-0.707106,-0.707106},{0.0,-1.0},{0.707106,-0.707106}};

uint32_t lastPress = 0;
uint16_t scanline[220];
uint8_t buttonsPreviousState = 0;
uint8_t buttonsJustPressed = 0;
uint8_t globalAlpha = 0;
uint8_t currentPacing = 0;
uint8_t currentTone = 0;
int8_t tDir = 16;
bool joystickEnabled = false;

Display::Transform titleTransform[9];
const Display::Model *titleModels[9] = {&Models::fontI, &Models::fontN, &Models::fontC, &Models::fontI, &Models::fontP, &Models::fontI, &Models::fontE, &Models::fontN, &Models::fontT};
Display::BodyPart bodyParts[64] =
{
  {{{110.0, 88.0}, {2.0, 0.0}, {0.0, 2.0}}, &Models::mouthOpen, nullptr, &bodyParts[1], 2.0, 0x07e0, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[0], &bodyParts[2], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[1], &bodyParts[3], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[2], &bodyParts[4], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[3], &bodyParts[5], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[4], &bodyParts[6], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[5], &bodyParts[7], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[6], &bodyParts[8], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[7], &bodyParts[9], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[8], &bodyParts[10], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[9], &bodyParts[11], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[10], &bodyParts[12], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[11], &bodyParts[13], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[12], &bodyParts[14], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[13], &bodyParts[15], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[14], &bodyParts[16], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[15], &bodyParts[17], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[16], &bodyParts[18], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[17], &bodyParts[19], 1.0, 0x57ea, 16},
  {{0.0, 0.0, {2.0, 0.0}, {0.0, 2.0}}, &Models::body, &bodyParts[18], nullptr, 1.0, 0x57ea, 16},
  {{165.0, 132.0, {1.0, 0.0}, {0.0, 1.0}}, &Models::exit, nullptr, &bodyParts[21], 1.0, 0xffff, 16},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, &Models::exitIndicator, &bodyParts[20], nullptr, 1.0, 0x001f, 32},
  {{55.0, 132.0, {1.0, 0.0}, {0.0, 1.0}}, &Models::exit, nullptr, &bodyParts[23], 1.0, 0xffff, 16},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, &Models::exitIndicator, &bodyParts[22], nullptr, 1.0, 0xf800, 32},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0},
  {{0.0, 0.0, {1.0, 0.0}, {0.0, 1.0}}, nullptr, nullptr, nullptr, 1.0, 0x0000, 0}
};
constexpr int maxBodyParts = sizeof(bodyParts) / sizeof(Display::BodyPart);
int numBodyParts = 24;
Display::Creature player =
{
  &Models::mouthClosed,
  &bodyParts[0],
  &bodyParts[19],
  {0, 0, 4},
  0
};
Display::Creature upExit =
{
  &Models::exit,
  &bodyParts[20],
  &bodyParts[21],
  {0, 0, 4},
  0
};
Display::Creature downExit =
{
  &Models::exit,
  &bodyParts[22],
  &bodyParts[23],
  {0, 0, 4},
  0
};
Display::Circle circle = {110, 88, 8};
Display::Particle particles[16]{(int16_t)0};

bool justPressed(uint8_t mask)
{
  if ((buttonsJustPressed & mask) != 0)
    return true;
  return false;
}

bool isPressed(uint8_t mask)
{
  if ((buttonsPreviousState & mask) != 0)
    return true;
  return false;
}

void init()
{
  PD::persistence = true;
  PD::fillLCD(0x0000);
  PC::setFrameRate(30);
  PD::enableDirectPrinting(true);
  PD::setFont(font5x7);
  PD::directcolor = 0x07e0;
  PD::directbgcolor = 0x0000;
  for (int i = 0; i < 9; ++i)
  {
    titleTransform[i].offset[0] = int16_t(i * 24 + 2);
    titleTransform[i].offset[1] = int16_t(2);
    titleTransform[i].xVector[0] = int16_t(1);
    titleTransform[i].xVector[1] = int16_t(0);
    titleTransform[i].yVector[0] = int16_t(0);
    titleTransform[i].yVector[1] = int16_t(1);
  }
  for (Display::BodyPart *part = player.head->next; part != nullptr; part = part->next)
  {
    part->transform.offset[0] = part->previous->transform.offset[0] + SQ15x16(part->previous->model->link[0]) * part->previous->transform.xVector[0] + SQ15x16(part->previous->model->link[1]) * part->previous->transform.yVector[0];
    part->transform.offset[1] = part->previous->transform.offset[1] + SQ15x16(part->previous->model->link[0]) * part->previous->transform.xVector[1] + SQ15x16(part->previous->model->link[1]) * part->previous->transform.yVector[1];
  }
  PB::pollButtons();
  while (PB::buttons_state != 0)
    PB::pollButtons();
  ProceduralMusic::begin();
#ifndef POK_SIM
  if (joyhat.JoyX() > 10 && joyhat.JoyY() > 10)
    joystickEnabled = true;
#endif
}

void update()
{
  SQ15x16 dirX, dirY, len;
  int joyX = 0;
  int joyY = 0;
  uint32_t startTime = PC::getTime();
  uint32_t endTime;
  uint8_t direction = 255;
  PB::pollButtons();
  buttonsJustPressed = ~buttonsPreviousState & PB::buttons_state;
  buttonsPreviousState = PB::buttons_state;
  if (PB::upBtn())
  {
    if (PB::leftBtn())
      direction = DIR_UP_LEFT;
    else if (PB::rightBtn())
      direction = DIR_UP_RIGHT;
    else
      direction = DIR_UP;
  }
  else if (PB::downBtn())
  {
    if (PB::leftBtn())
      direction = DIR_DOWN_LEFT;
    else if (PB::rightBtn())
      direction = DIR_DOWN_RIGHT;
    else
      direction = DIR_DOWN;
  }
  else if (PB::leftBtn())
    direction = DIR_LEFT;
  else if (PB::rightBtn())
    direction = DIR_RIGHT;
  if (PB::cBtn())
  {
    for (int i = 0; i < 16; ++i)
    {
      uint8_t dir = (rand() >> 3) & 0xFF;
      particles[i].offset[0] = player.head->transform.offset[0];
      particles[i].offset[1] = player.head->transform.offset[1];
      particles[i].vector[0] = Display::cosTable[dir] * 0.1;
      particles[i].vector[1] = Display::sinTable[dir] * 0.1;
      particles[i].velocity = ((rand() >> 5) & 0xF) + 1;
      particles[i].distance = 0;
      particles[i].color = 0x001F;
      particles[i].life = 64;
    }
  }
#ifndef POK_SIM
  if (joystickEnabled)
  {
    joyX = joyhat.JoyX() - 50;
    joyY = 50 - joyhat.JoyY();
  }
#endif
  if (joyX < -10 || joyX > 10 || joyY < -10 || joyY > 10)
  {
    dirX = joyX * 0.02;
    dirY = joyY * 0.02;
    len = Display::sqrt(dirX * dirX + dirY * dirY);
    direction = DIR_JOY;
    dirX /= len;
    dirY /= len;
    Display::slerpVector(player.head->transform.xVector[0], player.head->transform.xVector[1], dirY, -dirX, 0.25, player.head->scale);
    Display::slerpVector(player.head->transform.yVector[0], player.head->transform.yVector[1], dirX, dirY, 0.25, player.head->scale);
    player.head->transform.offset[0] -= player.head->transform.yVector[0];
    player.head->transform.offset[1] -= player.head->transform.yVector[1];
  }
  else if (direction != DIR_NONE)
  {
    Display::slerpVector(player.head->transform.xVector[0], player.head->transform.xVector[1], directions[direction][0], directions[direction][1], 0.25, player.head->scale);
    Display::slerpVector(player.head->transform.yVector[0], player.head->transform.yVector[1], directions[(direction + 2) % 8][0], directions[(direction + 2) % 8][1], 0.25, player.head->scale);
    player.head->transform.offset[0] -= player.head->transform.yVector[0];
    player.head->transform.offset[1] -= player.head->transform.yVector[1];
  }
  else
  {
    dirX = player.head->transform.offset[0] - SQ15x16((int16_t)(rand() % 188 + 16));
    dirY = player.head->transform.offset[1] - SQ15x16((int16_t)(rand() % 144 + 16));
    len = Display::sqrt(dirX * dirX + dirY * dirY);
    dirX /= len;
    dirY /= len;
    Display::slerpVector(player.head->transform.xVector[0], player.head->transform.xVector[1], dirY, -dirX, 0.25, player.head->scale);
    Display::slerpVector(player.head->transform.yVector[0], player.head->transform.yVector[1], dirX, dirY, 0.25, player.head->scale);
    player.head->transform.offset[0] -= player.head->transform.yVector[0];
    player.head->transform.offset[1] -= player.head->transform.yVector[1];
  }
  dirX = upExit.head->transform.offset[0] - SQ15x16((int16_t)(rand() % 188 + 16));
  dirY = upExit.head->transform.offset[1] - SQ15x16((int16_t)(rand() % 144 + 16));
  len = Display::sqrt(dirX * dirX + dirY * dirY);
  dirX /= len;
  dirY /= len;
  Display::slerpVector(upExit.head->transform.xVector[0], upExit.head->transform.xVector[1], dirY, -dirX, 0.1, upExit.head->scale);
  Display::slerpVector(upExit.head->transform.yVector[0], upExit.head->transform.yVector[1], dirX, dirY, 0.1, upExit.head->scale);
  upExit.head->transform.offset[0] -= upExit.head->transform.yVector[0];
  upExit.head->transform.offset[1] -= upExit.head->transform.yVector[1];
  dirX = downExit.head->transform.offset[0] - SQ15x16((int16_t)(rand() % 188 + 16));
  dirY = downExit.head->transform.offset[1] - SQ15x16((int16_t)(rand() % 144 + 16));
  len = Display::sqrt(dirX * dirX + dirY * dirY);
  dirX /= len;
  dirY /= len;
  Display::slerpVector(downExit.head->transform.xVector[0], downExit.head->transform.xVector[1], dirY, -dirX, 0.1, downExit.head->scale);
  Display::slerpVector(downExit.head->transform.yVector[0], downExit.head->transform.yVector[1], dirX, dirY, 0.1, downExit.head->scale);
  downExit.head->transform.offset[0] -= downExit.head->transform.yVector[0];
  downExit.head->transform.offset[1] -= downExit.head->transform.yVector[1];
  for (auto& part : bodyParts)
  {
    if (part.previous != nullptr)
    {
      part.transform.offset[0] = part.previous->transform.offset[0] + SQ15x16(part.previous->model->link[0]) * part.previous->transform.xVector[0] + SQ15x16(part.previous->model->link[1]) * part.previous->transform.yVector[0];
      part.transform.offset[1] = part.previous->transform.offset[1] + SQ15x16(part.previous->model->link[0]) * part.previous->transform.xVector[1] + SQ15x16(part.previous->model->link[1]) * part.previous->transform.yVector[1];
      if (!Display::floatsEqual(part.transform.yVector[0], part.previous->transform.yVector[0]) && !Display::floatsEqual(part.transform.yVector[1], part.previous->transform.yVector[1]))
      {
        Display::slerpVector(part.transform.xVector[0], part.transform.xVector[1], part.previous->transform.xVector[0], part.previous->transform.xVector[1], 0.25, part.scale);
        Display::slerpVector(part.transform.yVector[0], part.transform.yVector[1], part.previous->transform.yVector[0], part.previous->transform.yVector[1], 0.25, part.scale);
      }
    }
  }
  player.t += tDir;
  if (player.t == 0 || player.t == 128)
    tDir *= -1;
  //ProceduralMusic::parameter = currentTone * 4 + currentPacing;
  //Display::setTonePacing(currentTone, currentPacing);
  Display::drawCreature(downExit);
  Display::drawCreature(upExit);
  for (int i = 0; i < 9; ++i)
    Display::drawModel(titleModels[i], titleTransform[i], 1.0, 0x3d17, globalAlpha);
  Display::drawCreature(player);
  player.offscreen.center[0] = (int16_t)player.head->transform.offset[0];
  player.offscreen.center[1] = (int16_t)player.head->transform.offset[1];
  if (Display::isOffScreen(player.offscreen.center[0], player.offscreen.center[1]))
  {
    Display::clipToScreen(player.offscreen.center[0], player.offscreen.center[1]);
    Display::drawCircle(player.offscreen, player.head->color, 32);
  }
  //for (int i = 0; i < 16; ++i)
  //  Display::drawCircle(circle, 0x001F, 16);
  for (int i = 0; i < 8; ++i)
  {
    if (particles[i].life > 0)
    {
      particles[i].distance += particles[i].velocity;
      Display::drawParticle(particles[i]);
      particles[i].life -= 4;
    }
  }
  startTime = PC::getTime();
  Display::update();
  endTime = PC::getTime();
  PD::setCursor(0, 176 - PD::fontHeight);
  PD::print(endTime - startTime);
  #ifdef POK_SIM
  simulator.refreshDisplay();
  #endif
  if (globalAlpha < 32)
    ++globalAlpha;
}
