#include <Pokitto.h>
#ifndef POK_SIM
#include <JoyHat.h>
#endif
#include "Data.h"
#include "Display.h"
#include "Game.h"
#include "music.h"
#include "Types.h"

#define DEBUG_NONE -1
#define DEBUG_NUM_SHAPES 0
#define DEBUG_FRAME_RATE 1
#define DEBUG_FRAME_TIME 2
#define DEBUG_STATE 3
#define DEBUG_NUM_ENEMIES 4
#define DEBUG_STAGE 5
#define DEBUG_PACING_TONE 6
#define GAME_DEBUG DEBUG_NONE

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;

const uint16_t Game::enemyColors[] =
{
  0xf800,0x001f,0xfa00,0xfc00,0x8600,0xfe00,0x87e0,
  0xffe0,0x041f,0x0610,0x061f,0x07f0,0x07ff,0xf810,
  0xf81f,0xfa10,0xfc10,0x8610,0xfe10,0x87f0,0xfff0,
  0xfa1f,0x841f,0xfc1f,0x861f,0xfe1f,0x87ff,0xffff
};
const int Game::numEnemyColors = sizeof(enemyColors) / sizeof(uint16_t);
const VectorF Game::directions[8] = {{1.0,0.0},{0.707106,0.707106},{0.0,1.0},{-0.707106,0.707106},{-1.0,0.0},{-0.707106,-0.707106},{0.0,-1.0},{0.707106,-0.707106}};
const Stage Game::stages[] =
{
  //nextStage, maxEnemies, minSize, maxSize
  {         2,          1,       1,       1},//0
  {         3,          1,       2,       2},//1
  {         6,          2,       1,       2},//2
  {         8,          2,       2,       4},//3
  {        10,          1,       8,       8},//4
  {        11,          3,       2,       4},//5
  {        15,          3,       4,       4},//6
  {        18,          2,       8,       8},//7
  {        20,          4,       1,       4},//8
  {        26,          4,       2,       4},//9
  {        30,          4,       4,       4},//10
  {        34,          1,      16,      16},//11
  {        36,          3,       8,       8},//12
  {        38,          6,       1,       2},//13
  {        45,          2,      14,      14},//14
  {        47,          1,      24,      24},//15
  {        48,          4,       4,       8},//16
  {        52,          8,       2,       2},//17
  {        60,          1,      30,      30},//18
  { STAGE_MAX,          8,       2,       4} //19
};
const Model *Game::mouthOpenModels[] =
{
  &Models::mouthOpen,
  &Models::mouth2Open,
  &Models::mouth3Open
};
const Model *Game::mouthClosedModels[] =
{
  &Models::mouthClosed,
  &Models::mouth2Closed,
  &Models::mouth3Closed
};
const uint8_t Game::numMouthModels = sizeof(mouthOpenModels)/sizeof(Model*);
const Model *Game::tailModels[] =
{
  &Models::tail
};
const uint8_t Game::numTailModels = sizeof(tailModels)/sizeof(Model*);
const Model *Game::bodyModels[] =
{
  &Models::body,
  &Models::body2,
  &Models::body3,
  &Models::body4,
  &Models::body5
};
const uint8_t Game::numBodyModels = sizeof(bodyModels)/sizeof(Model*);
Game::State Game::previousState = Game::State::TitleInit;
Game::State Game::currentState = Game::State::TitleInit;
Game::State Game::nextState = Game::State::TitleIntro;
int Game::counter = 0;
int Game::nextEnemySize = -1;
uint32_t Game::score = 0;
uint32_t Game::scoreAdd = 0;
uint32_t Game::lastSpawn = 0;
uint32_t Game::creaturesDefeated = 0;
uint16_t Game::enemyCounters[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t Game::scoreUpdate = 0;
uint8_t Game::buttonsPreviousState = 0;
uint8_t Game::buttonsJustPressed = 0;
uint8_t Game::currentPacing = 0;
uint8_t Game::currentTone = 0;
uint8_t Game::currentStage = 0;
bool Game::joystickEnabled = false;

bool Game::justPressed(uint8_t mask)
{
  if ((buttonsJustPressed & mask) != 0)
    return true;
  return false;
}

bool Game::isPressed(uint8_t mask)
{
  if ((buttonsPreviousState & mask) != 0)
    return true;
  return false;
}

void Game::scorePoints(uint32_t amount)
{
  scoreAdd += amount;
  scoreUpdate = SCORE_UPDATE_SPEED;
}

void Game::update()
{
#if GAME_DEBUG != DEBUG_NONE
  uint32_t startTime = PC::getTime();
  uint32_t endTime;
#endif
  PB::pollButtons();
  buttonsJustPressed = ~buttonsPreviousState & PB::buttons_state;
  buttonsPreviousState = PB::buttons_state;
  switch (currentState)
  {
    //Title
    case State::TitleInit:
      updateTitleInit();
      [[fallthrough]]
    case State::TitleIntro:
      updateTitleIntro();
      break;
    case State::Title:
      updateTitle();
      break;
    case State::TitleOutro:
      updateTitleOutro();
      break;
    //Score Menu
    case State::ScoreMenuInit:
      updateScoreMenuInit();
      [[fallthrough]]
    case State::ScoreMenuIntro:
      updateScoreMenuIntro();
      break;
    case State::ScoreMenu:
      updateScoreMenu();
      break;
    case State::ScoreMenuOutro:
      updateScoreMenuOutro();
      break;
    //Game
    case State::Game:
      updateGame();
      break;
    case State::GameOutro:
      updateGameOutro();
      break;
    //Game Over
    case State::GameOverInit:
      updateGameOverInit();
      [[fallthrough]]
    case State::GameOverIntro:
      updateGameOverIntro();
      break;
    case State::GameOver:
      updateGameOver();
      break;
    case State::GameOverOutro:
      updateGameOverOutro();
      break;
    //Score Entry
    case State::ScoreEntryInit:
      updateScoreEntryInit();
      [[fallthrough]]
    case State::ScoreEntryIntro:
      updateScoreEntryIntro();
      break;
    case State::ScoreEntry:
      updateScoreEntry();
      break;
    case State::ScoreEntryOutro:
      updateScoreEntryOutro();
      break;
    case State::PausedInit:
      updatePausedInit();
      [[fallthrough]]
    case State::Paused:
      updatePaused();
      break;
  }
  ProceduralMusic::parameter = currentTone * 4 + currentPacing;
  Display::setTonePacing(currentTone, currentPacing);
  Data::setColors(0x3d17, 0x3d17, 0x3d17, 0x3d17);
  Data::setAlphas(0, 0, 32, 32);
#if GAME_DEBUG == DEBUG_NUM_SHAPES
  Data::addNumber(Display::getNumShapes(), 100, 15, {2, 166}, 0.25);
  Display::drawCreature(Data::text[15]);
  Data::clearText(15);
#elif GAME_DEBUG == DEBUG_FRAME_RATE
  endTime = PC::getTime();
  Data::addNumber(1000 / (endTime - startTime), 100, 15, {2, 166}, 0.25);
#elif GAME_DEBUG == DEBUG_FRAME_TIME
  endTime = PC::getTime();
  Data::addNumber(endTime - startTime, 100, 15, {2, 166}, 0.25);
#elif GAME_DEBUG == DEBUG_STATE
  Data::addNumber((int)currentState, 10, 15, {2, 166}, 0.25);
#elif GAME_DEBUG == DEBUG_NUM_ENEMIES
  Data::addNumber(Data::numEnemies, 1, 15, {2, 166}, 0.25);
#elif GAME_DEBUG == DEBUG_STAGE
  Data::addNumber(currentStage, 10, 15, {2, 166}, 0.25);
#elif GAME_DEBUG == DEBUG_PACING_TONE
  Data::addNumber(currentPacing * 10 + currentTone, 10, 15, {2, 166}, 0.25, true);
#endif
  Display::update();
#ifdef POK_SIM
  simulator.refreshDisplay();
#endif
}

void Game::doPlayerMovement()
{
  Point joy = {0, 0};
  uint8_t direction = DIR_NONE;
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
  if (justPressed(BTN_MASK_A) && Data::player.target == 0)
  {
    Data::player.speed = 4.0;
    Data::player.slerpAmount = 0.25;
    Data::player.target = 16;
    Data::player.state = Creature::State_Fleeing;
  }
  #ifndef POK_SIM
  if (joystickEnabled)
  {
    joy.x = joyhat.JoyX() - 50;
    joy.y = 50 - joyhat.JoyY();
  }
  #endif
  if (Data::player.state == Creature::State_Wandering)
  {
    if (Data::player.target > 0)
      --Data::player.target;
    if (joy.x < -10 || joy.x > 10 || joy.y < -10 || joy.y > 10)
    {
      Data::player.speed = 2.0;
      Data::player.slerpAmount = 0.15;
      Data::movePlayer((VectorF(joy) * 0.02f).normalize());
      Data::player.slerpAmount = 0.25;
    }
    else if (direction != DIR_NONE)
    {
      Data::player.speed = 2.0;
      Data::player.slerpAmount = 0.15;
      Data::movePlayer(directions[direction]);
      Data::player.slerpAmount = 0.25;
    }
    else
    {
      Data::player.speed = 0.0;
      Data::player.slerpAmount = 0.1;
    }
  }
  else
  {
    --Data::player.target;
    if (Data::player.target == 0)
    {
      Data::player.speed = 0.0;
      Data::player.slerpAmount = 0.1;
      Data::player.target = 32;
      Data::player.state = Creature::State_Wandering;
    }
    else
      Data::player.head->transform.offset -= Data::player.head->transform.yAxis * Data::player.speed;
  }
}

void Game::updateTitleInit()
{
  Data::setColors(0x3fff, 0x3fff, 0x3d17, 0x3d17);
  Data::setAlphas(0, 0, 0, 0);
  Data::addText("INCIPIENT", 0, {2, 2}, 1);
  Data::setColors(0xf800, 0xf800, 0xffff, 0xf800);
  Data::addText("START", 1, {2, 36}, 0.5);
  Data::addEnemy(0, {55, 114}, &Models::fish, nullptr, nullptr, &Models::tail, 0);
  Data::setColors(0x001f, 0x001f, 0xffff, 0x001f);
  Data::addText("SCORES", 2, {146, 36}, 0.5);
  Data::addEnemy(1, {165, 114}, &Models::fish, nullptr, nullptr, &Models::tail, 0);
  Data::enemies[0].state = Creature::State_Stationary;
  Data::enemies[1].state = Creature::State_Stationary;
  counter = 0;
  currentState = State::TitleIntro;
  score = 0;
  scoreAdd = 0;
  currentPacing = 0;
  currentTone = 0;
  currentStage = 0;
  ProceduralMusic::parameter = currentTone * 4 + currentPacing;
  Display::setTonePacing(currentTone, currentPacing);
}

void Game::updateTitleIntro()
{
  fixed_t scale = ++counter / 32.0;
  Data::scaleText(0, scale);
  Data::text[0].head->transform.offset = VectorF(110, 88).lerp({2, 2}, scale);
  Data::text[1].head->transform.offset = VectorF(110, 88).lerp({2, 36}, scale);
  Data::text[2].head->transform.offset = VectorF(110, 88).lerp({146, 36}, scale);
  scale /= 2.0;
  Data::scaleText(1, scale);
  Data::scaleText(2, scale);
  Data::setAlphas(counter, counter, counter / 2, counter / 2);
  Data::changeTextAlphas(0);
  Data::setAlphas(0, 0, counter, counter);
  Data::changeTextAlphas(1);
  Data::changeTextAlphas(2);
  Data::enemies[0].head->alphas[Shape::Line] = counter / 2;
  Data::enemies[0].head->alphas[Shape::Circle] = counter;
  Data::enemies[0].head->alphas[Shape::Triangle] = counter / 4;
  Data::enemies[0].tail->alphas[Shape::Line] = counter / 2;
  Data::enemies[0].tail->alphas[Shape::Circle] = counter;
  Data::enemies[0].tail->alphas[Shape::Triangle] = counter / 4;
  Data::enemies[1].head->alphas[Shape::Line] = counter / 2;
  Data::enemies[1].head->alphas[Shape::Circle] = counter;
  Data::enemies[1].head->alphas[Shape::Triangle] = counter / 4;
  Data::enemies[1].tail->alphas[Shape::Line] = counter / 2;
  Data::enemies[1].tail->alphas[Shape::Circle] = counter;
  Data::enemies[1].tail->alphas[Shape::Triangle] = counter / 4;
  if (counter == 32)
  {
    currentState = State::Title;
    Data::enemies[0].state = Creature::State_Wandering;
    Data::enemies[1].state = Creature::State_Wandering;
  }
  doPlayerMovement();
  Data::updateCreatures();
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
}

void Game::updateTitle()
{
  BodyPart *exitPart;
  int exitID;
  doPlayerMovement();
  Data::updateCreatures();
  if (Data::checkPlayerCollisions(exitID, exitPart))
  {
    Data::addParticles(Data::enemies[exitID].head->transform.offset, 8, Data::enemies[exitID].head->colors[Shape::Point]);
    Data::clearEnemy(exitID);
    previousState = State::TitleInit;
    currentState = State::TitleOutro;
    if (exitID == 0) //START
      nextState = State::Game;
    else if (exitID == 1)//SCORES
      nextState = State::ScoreMenuInit;
  }
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
}

void Game::updateTitleOutro()
{
  fixed_t scale = --counter / 32.0;
  Data::scaleText(0, scale);
  if (nextState != State::ScoreMenuInit)
  {
    if (Data::enemies[0].head != nullptr)
    {
      Data::scaleEnemy(0, scale);
      Data::enemies[0].head->alphas[Shape::Line] = counter / 2;
      Data::enemies[0].head->alphas[Shape::Circle] = counter;
      Data::enemies[0].head->alphas[Shape::Triangle] = counter / 4;
      Data::enemies[0].tail->alphas[Shape::Line] = counter / 2;
      Data::enemies[0].tail->alphas[Shape::Circle] = counter;
      Data::enemies[0].tail->alphas[Shape::Triangle] = counter / 4;
    }
    if (Data::enemies[1].head != nullptr)
    {
      Data::scaleEnemy(1, scale);
      Data::enemies[1].head->alphas[Shape::Line] = counter / 2;
      Data::enemies[1].head->alphas[Shape::Circle] = counter;
      Data::enemies[1].head->alphas[Shape::Triangle] = counter / 4;
      Data::enemies[1].tail->alphas[Shape::Line] = counter / 2;
      Data::enemies[1].tail->alphas[Shape::Circle] = counter;
      Data::enemies[1].tail->alphas[Shape::Triangle] = counter / 4;
    }
  }
  Data::text[0].head->transform.offset = VectorF(110, 88).lerp({2, 2}, scale);
  Data::text[1].head->transform.offset = VectorF(110, 88).lerp({2, 36}, scale);
  Data::text[2].head->transform.offset = VectorF(110, 88).lerp({146, 36}, scale);
  scale /= 2.0;
  Data::scaleText(1, scale);
  Data::scaleText(2, scale);
  Data::setAlphas(counter, counter, counter / 2, counter / 2);
  Data::changeTextAlphas(0);
  Data::setAlphas(0, 0, counter, counter);
  Data::changeTextAlphas(1);
  Data::changeTextAlphas(2);
  doPlayerMovement();
  Data::updateCreatures();
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
  if (counter == 0)
  {
    currentState = nextState;
    if (nextState == State::Game)
      Data::clearEnemy(1);
    Data::clearText(0);
    Data::clearText(1);
    Data::clearText(2);
  }
}

void Game::updateScoreMenuInit()
{
  Data::setColors(0x001f, 0x001f, 0xffff, 0x001f);
  Data::setAlphas(0, 0, 16, 16);
  Data::addText("SCORES", 0, {38, 2}, 1);
  for (int i = 0; i < 5; ++i)
  {
    Data::addText(Data::cookie.scores[i].name, i * 2 + 1, {56, 46 + 24 * i}, 0.5);
    Data::addNumber(Data::cookie.scores[i].score, 10000, i * 2 + 2, {104, 46 + 24 * i}, 0.5, true);
  }
  currentState = State::ScoreMenuIntro;
  counter = 0;
}

void Game::updateScoreMenuIntro()
{
  fixed_t scale = ++counter / 32.0;
  Data::setAlphas(0, 0, counter, counter);
  Data::changeTextAlphas(0);
  Data::scaleText(0, scale);
  Data::text[0].head->transform.offset = VectorF(110, 88).lerp({38, 2}, scale);
  for (int i = 0; i < 5; ++i)
  {
    if (Data::text[i * 2 + 1].head != nullptr)
      Data::text[i * 2 + 1].head->transform.offset = VectorF(110, 88).lerp({46, 46 + 24 * i}, scale);
    if (Data::text[i * 2 + 2].head != nullptr)
      Data::text[i * 2 + 2].head->transform.offset = VectorF(110, 88).lerp({104, 46 + 24 * i}, scale);
  }
  scale /= 2.0;
  for (int i = 1; i < 11; ++i)
  {
    if (Data::text[i].head != nullptr)
    {
      Data::scaleText(i, scale);
      Data::changeTextAlphas(i);
    }
  }
  if (counter == 32)
    currentState = State::ScoreMenu;
  doPlayerMovement();
  Data::updateCreatures();
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
}

void Game::updateScoreMenu()
{
  BodyPart *exitPart;
  int exitID;
  doPlayerMovement();
  Data::updateCreatures();
  if (Data::checkPlayerCollisions(exitID, exitPart))
  {
    Data::addParticles(Data::enemies[exitID].head->transform.offset, 8, Data::enemies[exitID].head->colors[Shape::Point]);
    Data::clearEnemy(exitID);
    previousState = State::ScoreMenuInit;
    currentState = State::ScoreMenuOutro;
    nextState = State::TitleInit;
  }
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
}

void Game::updateScoreMenuOutro()
{
  fixed_t scale = --counter / 32.0;
  Data::setAlphas(0, 0, counter, counter);
  Data::changeTextAlphas(0);
  Data::scaleText(0, scale);
  Data::text[0].head->transform.offset = VectorF(110, 88).lerp({38, 2}, scale);
  for (int i = 0; i < 5; ++i)
  {
    if (Data::text[i * 2 + 1].head != nullptr)
      Data::text[i * 2 + 1].head->transform.offset = VectorF(110, 88).lerp({46, 46 + 24 * i}, scale);
    if (Data::text[i * 2 + 2].head != nullptr)
      Data::text[i * 2 + 2].head->transform.offset = VectorF(110, 88).lerp({104, 46 + 24 * i}, scale);
  }
  scale /= 2.0;
  for (int i = 1; i < 11; ++i)
  {
    if (Data::text[i].head != nullptr)
    {
      Data::scaleText(i, scale);
      Data::changeTextAlphas(i);
    }
  }
  doPlayerMovement();
  Data::updateCreatures();
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
  if (counter == 0)
    currentState = nextState;
}

void Game::updateGame()
{
  BodyPart *part;
  int enemyID;
  int newPacing = 0;
  int newTone = 0;
  const Stage &stage = stages[currentStage];
  if (PC::getTime() - lastSpawn >= 1000)
  {
    if (Data::numEnemies < stage.maxEnemies)
    {
      Point location;
      uint16_t outlineColor = enemyColors[(rand() >> 6) % numEnemyColors];
      uint16_t fillColor = enemyColors[(rand() >> 5) % numEnemyColors];
      uint8_t angle = (rand() >> 7) % 256;
      uint8_t mouth = (rand() >> 5) % numMouthModels;
      uint8_t body = (rand() >> 3) % numBodyModels;
      uint8_t tail = (rand() >> 6) % numTailModels;
      if (nextEnemySize == -1)
        nextEnemySize = (rand() >> 8) % (stage.maxSize - stage.minSize + 1) + stage.minSize;
      location.x = cosTable[angle] * 256 + 110;
      location.y = sinTable[angle] * 256 + 88;
      Data::setColors(outlineColor, outlineColor, fillColor, fillColor);
      Data::setAlphas(16, 16, 8, 8);
      enemyID = Data::addEnemy(-1, location, mouthOpenModels[mouth], mouthClosedModels[mouth], bodyModels[body], tailModels[tail], nextEnemySize);
      if (enemyID != -1)
      {
        Data::enemies[enemyID].slerpAmount = 1.0;
        Data::enemies[enemyID].speed = 0.0;
        Data::moveEnemy(enemyID, {110, 88});
        Data::enemies[enemyID].slerpAmount = 0.1;
        Data::enemies[enemyID].speed = 1.0;
        lastSpawn = PC::getTime();
        enemyCounters[enemyID] = ((rand() >> 5) % 20 + 1) * 8;
      }
    }
  }
  doPlayerMovement();
  Data::updateCreatures();
  if (Data::checkPlayerCollisions(enemyID, part))
  {
    Data::addParticles(part->weakspot.center, 8, part->colors[Shape::Point]);
    if (part == Data::enemies[enemyID].head)
    {
      scorePoints(SCORE_DESTROY_CREATURE);
      Data::player.head->state = (Data::player.head->state & BodyPart::State_Counter) + BodyPart::State_Digesting;
      Data::food[0].model = Data::enemyBodies[enemyID];
      Data::food[0].colors[Shape::Point] = Data::enemies[enemyID].head->colors[Shape::Point];
      Data::food[0].colors[Shape::Line] = Data::enemies[enemyID].head->colors[Shape::Line];
      Data::food[0].colors[Shape::Circle] = Data::enemies[enemyID].head->colors[Shape::Circle];
      Data::food[0].colors[Shape::Triangle] = Data::enemies[enemyID].head->colors[Shape::Triangle];
      Data::clearEnemy(enemyID);
      lastSpawn = PC::getTime();
      ++creaturesDefeated;
      if (creaturesDefeated >= stage.nextStage)
      {
        nextEnemySize = -1;
        ++currentStage;
      }
    }
    else if (part->alphas[Shape::Triangle] != 0)
    {
      scorePoints(SCORE_DAMAGE_BODYPART);
      part->alphas[Shape::Circle] = 0;
      part->alphas[Shape::Triangle] = 0;
    }
    else
    {
      scorePoints(SCORE_DESTROY_BODYPART);
      part->alphas[Shape::Point] = 0;
      part->alphas[Shape::Line] = 0;
      part->alphas[Shape::Circle] = 0;
      part->alphas[Shape::Triangle] = 0;
      part->state = BodyPart::State_Breaking;
      Data::player.head->state = (Data::player.head->state & BodyPart::State_Counter) + BodyPart::State_Digesting;
      Data::food[0].colors[Shape::Point] = part->colors[Shape::Point];
      Data::enemies[enemyID].state = Creature::State_Fleeing;
      Data::enemies[enemyID].target = 32;
      Data::enemies[enemyID].speed = 4.0;
      Data::enemies[enemyID].slerpAmount = 0.25;
    }
  }
  while (Data::checkEnemyCollisions(enemyID, part))
  {
    Data::addParticles(part->weakspot.center, 8, part->colors[Shape::Point]);
    if (part == Data::player.head)
    {
      part->alphas[Shape::Point] = 0;
      part->alphas[Shape::Line] = 0;
      part->alphas[Shape::Circle] = 0;
      part->alphas[Shape::Triangle] = 0;
      part->next->alphas[Shape::Point] = 0;
      part->next->alphas[Shape::Line] = 0;
      part->next->alphas[Shape::Circle] = 0;
      part->next->alphas[Shape::Triangle] = 0;
      for (auto &enemy : Data::enemies)
      {
        enemy.speed = 1.0;
        enemy.slerpAmount = 0.1;
        enemy.state = Creature::State_Wandering;
      }
      currentState = State::GameOutro;
      nextState = State::GameOverInit;
      counter = 32;
    }
    else if (part->alphas[Shape::Triangle] != 0)
    {
      part->alphas[Shape::Circle] = 0;
      part->alphas[Shape::Triangle] = 0;
    }
    else
    {
      part->alphas[Shape::Point] = 0;
      part->alphas[Shape::Line] = 0;
      part->alphas[Shape::Circle] = 0;
      part->alphas[Shape::Triangle] = 0;
      part->state = BodyPart::State_Breaking;
      Data::enemies[enemyID].head->state = (Data::enemies[enemyID].head->state & BodyPart::State_Counter) + BodyPart::State_Digesting;
    }
  }
  for (auto &enemy : Data::enemies)
  {
    enemyID = &enemy - &Data::enemies[0];
    if (enemy.head == nullptr)
      continue;
    else if (Display::isOnScreen(enemy.head->weakspot.center) && enemy.state != Creature::State_Fleeing)
    {
      --enemyCounters[enemyID];
      if (enemyCounters[enemyID] == 0)
      {
        enemy.state = (rand() >> 13) % 4;
        if (enemy.state == Creature::State_Stationary)
        {
          enemy.state = Creature::State_Wandering;
          enemy.speed = 0.5;
          enemy.slerpAmount = 0.1;
          enemyCounters[enemyID] = ((rand() >> 5) % 8 + 1) * 8;
        }
        else if (enemy.state == Creature::State_Wandering)
        {
          enemy.speed = 1.0;
          enemy.slerpAmount = 0.1;
          enemyCounters[enemyID] = ((rand() >> 5) % 20 + 1) * 8;
        }
        else if (enemy.state == Creature::State_Fleeing)
        {
          enemyCounters[enemyID] = 1;
          enemy.target = 16;
          enemy.speed = 4.0;
          enemy.slerpAmount = 0.25;
        }
        else if (enemy.state == Creature::State_Chasing)
        {
          enemyCounters[enemyID] = ((rand() >> 5) % 16 + 4) * 8;
          enemy.speed = 1.5;
          enemy.slerpAmount = 0.15;
          if (Data::player.head->next == Data::player.tail)
            enemy.target = 0;
          else
          {
            do
            {
              enemy.target = (rand() >> 7) % 22 + 1;
            } while (Data::playerParts[enemy.target].model == nullptr);
          }
        }
      }
    }
    if (enemy.state == Creature::State_Stationary)
      newPacing += 1;
    else if (enemy.state == Creature::State_Wandering)
      newPacing += 2;
    else if (enemy.state == Creature::State_Fleeing)
      newPacing -= 1;
    else if (enemy.state == Creature::State_Chasing)
      newPacing += 4;
  }
  for (part = Data::player.head->next; part != Data::player.tail; part = part->next)
  {
    if (part->alphas[Shape::Triangle] == 0)
      newPacing += 2;
  }
  newPacing /= 8;
  newPacing += (Data::maxPlayerSize - Data::playerSize) / 4;
  newTone += (32 - Data::numAvailableEnemyParts) / 8;
  if (newPacing < 0)
    currentPacing = 0;
  else if (newPacing > 3)
    currentPacing = 3;
  else
    currentPacing = newPacing;
  if (newTone < 0)
    currentTone = 0;
  else if (newTone > 3)
    currentTone = 3;
  else
    currentTone = newTone;
  Data::setColors(0x07e0, 0x07e0, 0x07e0, 0x07e0);
  Data::setAlphas(0, 0, 32, 32);
  if (scoreAdd == 0)
    Data::clearText(1);
  else if (scoreAdd < 10)
    Data::addNumber(scoreAdd, 1, 1, {206, 142}, 0.5, false);
  else if (scoreAdd < 100)
    Data::addNumber(scoreAdd, 10, 1, {194, 142}, 0.5, false);
  else if (scoreAdd < 1000)
    Data::addNumber(scoreAdd, 100, 1, {182, 142}, 0.5, false);
  Data::addNumber(score, 10000, 0, {158, 158}, 0.5, true);
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
  if (justPressed(BTN_MASK_C))
  {
    previousState = currentState;
    currentState = State::PausedInit;
  }
  if (scoreUpdate > 0)
  {
    --scoreUpdate;
    if (scoreUpdate == 0 && scoreAdd > 0)
    {
      ++score;
      --scoreAdd;
      if (scoreAdd > 0)
        scoreUpdate = SCORE_UPDATE_SPEED / 2;
    }
  }
}

void Game::updateGameOutro()
{
  if (counter > 0)
    --counter;
  for (auto &part : Data::enemyParts)
  {
    part.alphas[Shape::Line] = counter / 2;
    part.alphas[Shape::Circle] = counter / 4;
    part.alphas[Shape::Triangle] = counter / 4;
  }
  Data::updateCreatures();
  Data::setColors(0x07e0, 0x07e0, 0x07e0, 0x07e0);
  Data::setAlphas(0, 0, 32, 32);
  if (scoreAdd == 0)
    Data::clearText(1);
  else if (scoreAdd < 10)
    Data::addNumber(scoreAdd, 1, 1, {206, 142}, 0.5, false);
  else if (scoreAdd < 100)
    Data::addNumber(scoreAdd, 10, 1, {194, 142}, 0.5, false);
  else if (scoreAdd < 1000)
    Data::addNumber(scoreAdd, 100, 1, {182, 142}, 0.5, false);
  Data::addNumber(score, 10000, 0, {158, 158}, 0.5, true);
  Data::drawText();
  Data::drawEnemies();
  Data::drawParticles();
  if (scoreUpdate > 0)
  {
    --scoreUpdate;
    if (scoreUpdate == 0)
    {
      ++score;
      --scoreAdd;
      if (scoreAdd > 0)
        scoreUpdate = SCORE_UPDATE_SPEED / 2;
    }
  }
  if (counter == 0 && scoreAdd == 0)
  {
    scoreUpdate = 0;
    currentState = nextState;
  }
}

void Game::updateGameOverInit()
{
  for (int i = 0; i < 8; ++i)
    Data::clearEnemy(i);
  Data::setColors(0xf800, 0xf800, 0xf800, 0xf800);
  Data::setAlphas(0, 0, 0, 0);
  Data::addText("GAME", 1, {16, 2}, 2);
  Data::addText("OVER", 2, {16, 66}, 2);
  currentPacing = 0;
  currentTone = 0;
  currentState = State::GameOverIntro;
  counter = 0;
}

void Game::updateGameOverIntro()
{
  fixed_t scale = ++counter / 32.0;
  Data::scaleText(1, scale * 2.0);
  Data::scaleText(2, scale * 2.0);
  Data::text[1].head->transform.offset = VectorF(110, 88).lerp({16, 2}, scale);
  Data::text[2].head->transform.offset = VectorF(110, 88).lerp({16, 66}, scale);
  Data::setAlphas(0, counter / 2, counter / 4, counter / 4);
  Data::changeTextAlphas(1);
  Data::changeTextAlphas(2);
  Data::updateCreatures();
  Data::drawText();
  if (counter == 32)
    currentState = State::GameOver;
}

void Game::updateGameOver()
{
  Data::updateCreatures();
  Data::drawText();
  if (justPressed(BTN_MASK_A) || justPressed(BTN_MASK_B))
    currentState = State::GameOverOutro;
}

void Game::updateGameOverOutro()
{
  fixed_t scale = --counter / 32.0;
  Data::scaleText(1, scale * 2.0);
  Data::scaleText(2, scale * 2.0);
  Data::text[1].head->transform.offset = VectorF(110, 88).lerp({16, 2}, scale);
  Data::text[2].head->transform.offset = VectorF(110, 88).lerp({16, 66}, scale);
  Data::setAlphas(0, counter / 2, counter / 4, counter / 4);
  Data::changeTextAlphas(1);
  Data::changeTextAlphas(2);
  Data::updateCreatures();
  Data::drawText();
  if (counter == 0)
    currentState = State::ScoreEntryInit;
}

void Game::updateScoreEntryInit()
{
  if (score > Data::cookie.scores[0].score)
    scoreUpdate = 0;
  else if (score > Data::cookie.scores[1].score)
    scoreUpdate = 1;
  else if (score > Data::cookie.scores[2].score)
    scoreUpdate = 2;
  else if (score > Data::cookie.scores[3].score)
    scoreUpdate = 3;
  else if (score > Data::cookie.scores[4].score)
    scoreUpdate = 4;
  else
    scoreUpdate = 5;
  for (int i = 4; i > scoreUpdate; --i)
  {
    Data::cookie.scores[i].name[0] = Data::cookie.scores[i - 1].name[0];
    Data::cookie.scores[i].name[1] = Data::cookie.scores[i - 1].name[1];
    Data::cookie.scores[i].name[2] = Data::cookie.scores[i - 1].name[2];
    Data::cookie.scores[i].score = Data::cookie.scores[i - 1].score;
  }
  Data::setColors(0x001f, 0x001f, 0xffff, 0x001f);
  Data::setAlphas(0, 0, 0, 0);
  Data::addText("SCORES", 0, {38, 2}, 1);
  for (int i = 0; i < 5; ++i)
  {
    if (i == scoreUpdate)
    {
      Data::setColors(0x07e0, 0x07e0, 0x07e0, 0x07e0);
      Data::cookie.scores[i].name[0] = 'A';
      Data::cookie.scores[i].name[1] = 'A';
      Data::cookie.scores[i].name[2] = 'A';
      Data::addText("AAA", i * 2 + 1, {56, 46 + 24 * i}, 0.5);
      Data::setAlphas(0, 0, 32, 32);
      Data::addNumber(score, 10000, i * 2 + 2, {158, 158}, 0.5, true);
    }
    else
    {
      Data::setColors(0x001f, 0x001f, 0xffff, 0x001f);
      Data::setAlphas(0, 0, 0, 0);
      Data::addText(Data::cookie.scores[i].name, i * 2 + 1, {56, 46 + 24 * i}, 0.5);
      Data::addNumber(Data::cookie.scores[i].score, 10000, i * 2 + 2, {104, 46 + 24 * i}, 0.5, true);
    }
  }
  if (scoreUpdate == 5)
  {
    Data::setColors(0x07e0, 0x07e0, 0x07e0, 0x07e0);
    Data::setAlphas(0, 0, 32, 32);
    Data::addNumber(score, 10000, 13, {158, 158}, 0.5, true);
  }
  currentState = State::ScoreEntryIntro;
  counter = 0;
}

void Game::updateScoreEntryIntro()
{
  fixed_t scale = ++counter / 32.0;
  Data::setAlphas(0, 0, counter, counter);
  Data::changeTextAlphas(0);
  Data::scaleText(0, scale);
  Data::text[0].head->transform.offset = VectorF(110, 88).lerp({38, 2}, scale);
  for (int i = 0; i < 5; ++i)
  {
    if (Data::text[i * 2 + 1].head != nullptr)
      Data::text[i * 2 + 1].head->transform.offset = VectorF(110, 88).lerp({46, 46 + 24 * i}, scale);
    if (Data::text[i * 2 + 2].head != nullptr)
    {
      if (scoreUpdate == i)
        Data::text[i * 2 + 2].head->transform.offset = VectorF(158, 158).lerp({104, 46 + 24 * i}, scale);
      else
        Data::text[i * 2 + 2].head->transform.offset = VectorF(110, 88).lerp({104, 46 + 24 * i}, scale);
    }
  }
  scale /= 2.0;
  for (int i = 0; i < 5; ++i)
  {
    if (Data::text[i * 2 + 1].head != nullptr)
    {
      Data::scaleText(i * 2 + 1, scale);
      Data::changeTextAlphas(i * 2 + 1);
    }
    if (Data::text[i * 2 + 2].head != nullptr && i != scoreUpdate)
    {
      Data::scaleText(i * 2 + 2, scale);
      Data::changeTextAlphas(i * 2 + 2);
    }
  }
  if (counter == 32)
  {
    counter = 0;
    lastSpawn = PC::getTime();
    currentState = State::ScoreEntry;
  }
  Data::updateCreatures();
  Data::drawText();
}

void Game::updateScoreEntry()
{
  if (PC::getTime() - lastSpawn >= 250 && scoreUpdate != 5)
  {
    BodyPart *part = Data::text[scoreUpdate * 2 + 1].head;
    if (counter > 0)
      part = part->next;
    if (counter > 1)
      part = part->next;
    if (part->alphas[Shape::Triangle] == 0)
      part->alphas[Shape::Triangle] = 32;
    else
      part->alphas[Shape::Triangle] = 0;
    lastSpawn = PC::getTime();
  }
  if (justPressed(BTN_MASK_LEFT))
  {
    BodyPart *part = Data::text[scoreUpdate * 2 + 1].head;
    if (counter > 0)
      part = part->next;
    if (counter > 1)
      part = part->next;
    part->alphas[Shape::Triangle] = 32;
    counter = (counter + 2) % 3;
  }
  else if (justPressed(BTN_MASK_RIGHT))
  {
    BodyPart *part = Data::text[scoreUpdate * 2 + 1].head;
    if (counter > 0)
      part = part->next;
    if (counter > 1)
      part = part->next;
    part->alphas[Shape::Triangle] = 32;
    counter = (counter + 1) % 3;
  }
  else if (justPressed(BTN_MASK_UP))
  {
    BodyPart *part = Data::text[scoreUpdate * 2 + 1].head;
    int letter = Data::cookie.scores[scoreUpdate].name[counter];
    if (letter >= 'A' && letter <= 'Z')
      letter -= 'A';
    else
      letter -= '0' - 26;
    if (counter > 0)
      part = part->next;
    if (counter > 1)
      part = part->next;
    letter = (letter + 1) % 36;
    if (letter >= 0 && letter < 26)
      Data::cookie.scores[scoreUpdate].name[counter] = 'A' + letter;
    else
      Data::cookie.scores[scoreUpdate].name[counter] = '0' + letter - 26;
    part->model = Models::models[letter];
  }
  else if (justPressed(BTN_MASK_DOWN))
  {
    BodyPart *part = Data::text[scoreUpdate * 2 + 1].head;
    int letter = Data::cookie.scores[scoreUpdate].name[counter];
    if (letter >= 'A' && letter <= 'Z')
      letter -= 'A';
    else
      letter -= '0' - 26;
    if (counter > 0)
      part = part->next;
    if (counter > 1)
      part = part->next;
    letter = (letter + 35) % 36;
    if (letter >= 0 && letter < 26)
      Data::cookie.scores[scoreUpdate].name[counter] = 'A' + letter;
    else
      Data::cookie.scores[scoreUpdate].name[counter] = '0' + letter - 26;
    part->model = Models::models[letter];
  }
  else if (justPressed(BTN_MASK_A))
  {
    if (scoreUpdate != 5)
    {
      BodyPart *part = Data::text[scoreUpdate * 2 + 1].head;
      if (counter > 0)
        part = part->next;
      if (counter > 1)
        part = part->next;
      part->alphas[Shape::Triangle] = 32;
      Data::cookie.scores[scoreUpdate].score = score;
      Data::cookie.saveCookie();
    }
    counter = 32;
    currentState = State::ScoreEntryOutro;
  }
  Data::updateCreatures();
  Data::drawText();
}

void Game::updateScoreEntryOutro()
{
  fixed_t scale = --counter / 32.0;
  Data::setAlphas(0, 0, counter, counter);
  Data::changeTextAlphas(0);
  Data::scaleText(0, scale);
  Data::text[0].head->transform.offset = VectorF(110, 88).lerp({38, 2}, scale);
  for (int i = 0; i < 5; ++i)
  {
    if (Data::text[i * 2 + 1].head != nullptr)
      Data::text[i * 2 + 1].head->transform.offset = VectorF(110, 88).lerp({46, 46 + 24 * i}, scale);
    if (Data::text[i * 2 + 2].head != nullptr)
      Data::text[i * 2 + 2].head->transform.offset = VectorF(110, 88).lerp({104, 46 + 24 * i}, scale);
  }
  scale /= 2.0;
  for (int i = 1; i < 11; ++i)
  {
    if (Data::text[i].head != nullptr)
    {
      Data::scaleText(i, scale);
      Data::changeTextAlphas(i);
    }
  }
  if (scoreUpdate == 5)
    Data::changeTextAlphas(13);
  Data::updateCreatures();
  Data::drawText();
  if (counter == 0)
  {
    currentState = State::TitleInit;
    Data::respawnPlayer();
  }
}

void Game::updatePausedInit()
{
  Data::setColors(0xf800, 0xf800, 0xf800, 0xf800);
  Data::setAlphas(0, 16, 8, 8);
  Data::addText("PAUSED", 2, {4, 64}, 1.5);
  currentState = State::Paused;
}

void Game::updatePaused()
{
  Data::drawText();
  Data::drawEnemies();
  Data::drawPlayer();
  Data::drawParticles();
  if (justPressed(BTN_MASK_C))
  {
    Data::clearText(2);
    currentState = previousState;
  }
}
