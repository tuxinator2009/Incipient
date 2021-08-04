#ifndef MUSIC_H
#define MUSIC_H

#include <cstdint>
#include <Pokitto.h>
#include <miloslav.h>
#include <File>
#include <LibAudio>
#include <LibSchedule>
#include "FMSynthSource.h"

using Chord=uint8_t[3];

namespace ProceduralMusic
{
  struct Measure
  {
    uint8_t numNotes;
    uint8_t melody[8];
    uint8_t durations[8];
    Chord chord;
  };
  struct GeneratorParameters
  {
    bool bassChords;
    uint8_t durationChances[4]; //8(whole), 4(half), 2(quarter), 1(eighth)
    uint8_t movementChances[3]; //step, skip, leap
  };
  constexpr uint8_t Pacing_Slowest = 0;
  constexpr uint8_t Pacing_Slow = 1;
  constexpr uint8_t Pacing_Fast = 2;
  constexpr uint8_t Pacing_Fastest = 3;
  constexpr uint8_t Tone_Methodical = 0;
  constexpr uint8_t Tone_SemiMethodical = 4;
  constexpr uint8_t Tone_SemiChaotic = 8;
  constexpr uint8_t Tone_Chaotic = 12;
  constexpr uint8_t numNotes = 12;
  void begin();
  void nextNote();
  void generateMeasure();
  extern const uint8_t trebleNotes[];
  extern const uint8_t bassNotes[];
  extern const uint8_t durations[];
  extern const uint8_t numDurations;
  extern const uint8_t maxDuration;
  extern const Chord chords[];
  extern const uint8_t numChords;
  extern const GeneratorParameters parameters[16];
  extern const uint32_t tempos[4];
  extern uint8_t currentNote;
  extern uint8_t parameter;
  extern uint8_t lastNote;
  extern Measure measure;
  //extern Audio::FMSynthSource *sources[4];
  extern FMSynth::Patch piano;
};

#endif //MUSIC_H
