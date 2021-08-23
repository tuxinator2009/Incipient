#ifndef GAME_H
#define GAME_H

namespace Game
{
  enum class State : uint8_t
  {
    TitleInit, TitleIntro, Title, TitleOutro,
    ScoreMenuInit, ScoreMenuIntro, ScoreMenu, ScoreMenuOutro,
    Game, GameOutro,
    GameOverInit, GameOverIntro, GameOver, GameOverOutro,
    ScoreEntryInit, ScoreEntryIntro, ScoreEntry, ScoreEntryOutro,
    PausedInit, Paused
  };
  extern const uint16_t enemyColors[];
  extern const int numEnemyColors;
  extern const VectorF directions[8];
  extern const Stage stages[];
  extern const Model *mouthOpenModels[];
  extern const Model *mouthClosedModels[];
  extern const uint8_t numMouthModels;
  extern const Model *tailModels[];
  extern const uint8_t numTailModels;
  extern const Model *bodyModels[];
  extern const uint8_t numBodyModels;
  extern State previousState;
  extern State currentState;
  extern State nextState;
  extern int counter;
  extern int nextEnemySize;
  extern uint32_t score;
  extern uint32_t scoreAdd;
  extern uint32_t lastSpawn;
  extern uint32_t creaturesDefeated;
  extern uint16_t enemyCounters[8];
  extern uint8_t scoreUpdate;
  extern uint8_t buttonsPreviousState;
  extern uint8_t buttonsJustPressed;
  extern uint8_t currentPacing;
  extern uint8_t currentTone;
  extern uint8_t currentStage;
  extern bool joystickEnabled;
  bool justPressed(uint8_t mask);
  bool isPressed(uint8_t mask);
  void scorePoints(uint32_t amount);
  void update();
  void doPlayerMovement();
  void updateTitleInit();
  void updateTitleIntro();
  void updateTitle();
  void updateTitleOutro();
  void updateScoreMenuInit();
  void updateScoreMenuIntro();
  void updateScoreMenu();
  void updateScoreMenuOutro();
  void updateGame();
  void updateGameOutro();
  void updateGameOverInit();
  void updateGameOverIntro();
  void updateGameOver();
  void updateGameOverOutro();
  void updateScoreEntryInit();
  void updateScoreEntryIntro();
  void updateScoreEntry();
  void updateScoreEntryOutro();
  void updatePausedInit();
  void updatePaused();
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
  constexpr uint8_t DIR_NONE = 255;
  constexpr uint8_t SCORE_UPDATE_SPEED = 4;
  constexpr uint32_t SCORE_DAMAGE_BODYPART = 1;
  constexpr uint32_t SCORE_DESTROY_BODYPART = 3;
  constexpr uint32_t SCORE_DESTROY_CREATURE = 5;
  constexpr uint32_t SCORE_BONUS_BODYPART = 10;
  constexpr uint32_t SCORE_BONUS_CREATURE = 15;
  constexpr uint32_t STAGE_MAX = 0xFFFFFFFF;
};

#endif //GAME_H
