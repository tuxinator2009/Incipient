#include <cstdint>
#include <Pokitto.h>
#include <File>
#include <LibAudio>
#include <LibSchedule>
#include "FixedPoints/FixedPoints.h"
#ifndef POK_SIM
#include <JoyHat.h>
#endif
#include "Data.h"
#include "Display.h"
#include "Game.h"
#include "FMSynthSource.h"
#include "music.h"
#include "models.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

void init()
{
  PD::persistence = true;
  PD::fillLCD(0x0000);
  PC::setFrameRate(30);
  PD::enableDirectPrinting(true);
  PD::setFont(font5x7);
  PD::directcolor = 0x07e0;
  PD::directbgcolor = 0x0000;
  Data::setupData();
  PB::pollButtons();
  while (PB::buttons_state != 0)
    PB::pollButtons();
  ProceduralMusic::begin();
#ifndef POK_SIM
  if (joyhat.JoyX() > 10 && joyhat.JoyY() > 10)
    Game::joystickEnabled = true;
#endif
}

void update()
{
  Game::update();
}
