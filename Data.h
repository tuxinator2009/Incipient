#ifndef DATA_H
#define DATA_H

#include <Pokitto.h>
#include <PokittoCookie.h>
#include "models.h"
#include "Types.h"

namespace Data
{
  class GameCookie : public Pokitto::Cookie
  {
    public:
      uint8_t initialized;
      Score scores[5];
      void init()
      {
        if (initialized == 42)
          return;
        initialized = 42;
        for (int i = 0; i < 5; ++i)
        {
          scores[i].name[0] = 'A';
          scores[i].name[1] = 'A';
          scores[i].name[2] = 'A';
          scores[i].name[3] = '\0';
          scores[i].score = 0;
        }
        saveCookie();
      }
  };
  extern GameCookie cookie;
  extern BodyPart playerParts[24];
  extern BodyPart enemyParts[32];
  extern BodyPart textParts[64];
  extern Food food[24];
  extern Particle particles[32];
  extern Creature player;
  extern Creature enemies[8];
  extern const Model *enemyBodies[8];
  extern Creature text[16];
  extern int numAvailableEnemyParts;
  extern int numEnemies;
  extern int maxPlayerSize;
  extern int playerSize;
  extern uint16_t colors[4];
  extern uint8_t alphas[4];
  void setupData();
  void movePlayer(VectorF dir);
  void rotatePlayer(VectorF dir);
  void moveEnemy(int index, Point location);
  void transformCircle(const Circle &in, const Transform &transform, Circle &out);
  void updatePartTransforms(BodyPart &part, const fixed_t &slerpAmount);
  void updatePartState(BodyPart &part);
  void updateCreatures();
  bool circleCircleCollision(const Circle &c1, const Circle &c2);
  bool checkPlayerCollisions(int &enemyID, BodyPart *&part);
  bool checkEnemyCollisions(int &enemyID, BodyPart *&part);
  void drawPlayer();
  void drawEnemies();
  void drawParticles();
  void drawText();
  BodyPart *nextAvailablePlayerPart();
  BodyPart *nextAvailableEnemyPart();
  BodyPart *nextAvailableTextPart();
  Particle *nextAvailableParticle();
  int nextAvailableEnemy();
  void setColors(uint16_t points, uint16_t lines, uint16_t circles, uint16_t triangles);
  void setAlphas(uint8_t points, uint8_t lines, uint8_t circles, uint8_t triangles);
  void addChar(char ch, int index, fixed_t scale);
  void addText(const char *string, int index, Point location, fixed_t scale);
  void addNumber(int number, int max, int index, Point location, fixed_t scale, bool showZeros=false);
  void clearText(int index);
  void changeTextColors(int index);
  void changeTextAlphas(int index);
  void changeTextColorsAndAlphas(int index);
  void scaleText(int index, fixed_t scale);
  int addEnemy(int index, Point location, const Model *mouthOpen, const Model *mouthClosed, const Model *bodyModel, const Model *tailModel, int bodyCount);
  void clearEnemy(int index);
  void addParticles(VectorF offset, int count, uint16_t color);
  void scaleEnemy(int index, fixed_t scale);
  void scalePlayer(fixed_t scale);
  void respawnPlayer();
};

#endif //DATA_H
