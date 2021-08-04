#include <cstdint>
#include <Pokitto.h>
#include <miloslav.h>
#include <File>
#include <LibAudio>
#include <LibSchedule>
#include "FMSynthSource.h"
#include "music.h"

#define NOTE_A 12
#define NOTE_B 14
#define NOTE_C 3
#define NOTE_D 5
#define NOTE_E 7
#define NOTE_F 8
#define NOTE_G 10

#define NOTE(n, o) (n+o*12)-14

const uint8_t ProceduralMusic::trebleNotes[] = 
{
  NOTE(NOTE_C, 4), NOTE(NOTE_D, 4), NOTE(NOTE_E, 4), NOTE(NOTE_F, 4),
  NOTE(NOTE_G, 4), NOTE(NOTE_A, 4), NOTE(NOTE_B, 4), NOTE(NOTE_C, 5),
  NOTE(NOTE_D, 5), NOTE(NOTE_E, 5), NOTE(NOTE_F, 5), NOTE(NOTE_G, 5)
};
const uint8_t ProceduralMusic::bassNotes[] =
{
  NOTE(NOTE_E, 2), NOTE(NOTE_F, 2), NOTE(NOTE_G, 2), NOTE(NOTE_A, 2),
  NOTE(NOTE_B, 2), NOTE(NOTE_C, 3), NOTE(NOTE_D, 3), NOTE(NOTE_E, 3),
  NOTE(NOTE_F, 3), NOTE(NOTE_G, 3), NOTE(NOTE_A, 3), NOTE(NOTE_B, 3)
};
const uint8_t ProceduralMusic::durations[] = {8, 4, 2, 1};
const uint8_t ProceduralMusic::numDurations = sizeof(durations);
const uint8_t ProceduralMusic::maxDuration = durations[0];

const Chord ProceduralMusic::chords[] =
{
  {0, 2, 4},
  {0, 2, 5},
  {0, 3, 5},
  {1, 3, 5},
  {1, 3, 6},
  {1, 4, 6},
  {2, 4, 6},
  {2, 4, 7},
  {2, 5, 7},
  {3, 5, 7},
  {3, 5, 8},
  {3, 6, 8},
  {4, 6, 8},
  {4, 6, 9},
  {4, 7, 9},
  {5, 7, 9},
  {5, 7, 10},
  {5, 8, 10},
  {6, 8, 10},
  {6, 8, 11},
  {6, 9, 11},
  {7, 9, 11}
};
const uint8_t ProceduralMusic::numChords = sizeof(ProceduralMusic::chords) / 3;

const ProceduralMusic::GeneratorParameters ProceduralMusic::parameters[16]
{//treble  1/1  1/2  1/4  1/8    Stp  Skp  Jmp
  { true, { 10,  25, 100, 100}, { 70,  90, 100}}, //slowest methodical
  { true, {  5,  15, 100, 100}, { 70,  90, 100}}, //slow methodical
  { true, {  0,  10,  80, 100}, { 70,  85, 100}}, //fast methodical
  { true, {  0,   0,  25, 100}, { 70,  85, 100}}, //fastest methodical
  
  { true, { 10,  25, 100, 100}, { 50,  85, 100}}, //slowest semi-methodical
  { true, {  5,  15,  85, 100}, { 50,  85, 100}}, //slow semi-methodical
  { true, {  0,   5,  70, 100}, { 50,  90, 100}}, //fast semi-methodical
  {false, {  0,   5,  65, 100}, { 50,  90, 100}}, //fastest semi-methodical
  
  { true, { 10,  25, 100, 100}, { 30,  80, 100}}, //slowest semi-chaotic
  { true, {  5,  15,  80, 100}, { 30,  80, 100}}, //slow semi-chaotic
  {false, {  0,   5,  60, 100}, { 30,  80, 100}}, //fast semi-chaotic
  {false, {  0,   0,  45, 100}, { 30,  80, 100}}, //fastest semi-chaotic
  
  { true, { 10,  25, 100, 100}, { 10,  75, 100}}, //slowest chaotic
  {false, {  5,  15,  75, 100}, { 10,  75, 100}}, //slow chaotic
  {false, {  0,   5,  50, 100}, { 10,  75, 100}}, //fast chaotic
  {false, {  0,   0,  25, 100}, { 10,  75, 100}}  //fastest chaotic
};
const uint32_t ProceduralMusic::tempos[4] = {(4 * 60000) / 120, (4 * 60000) / 160, (4 * 60000) / 200, (4 * 60000) / 240};

uint8_t ProceduralMusic::currentNote = 0;
uint8_t ProceduralMusic::parameter = 0;
uint8_t ProceduralMusic::lastNote = 6;
ProceduralMusic::Measure ProceduralMusic::measure = {0, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0}};
//Audio::FMSynthSource *ProceduralMusic::sources[4] = {nullptr, nullptr, nullptr, nullptr};
FMSynth::Patch ProceduralMusic::piano =
{
  .algorithm=3, .volume=80, .feedback=50, .glide=0, .attack=0, .decay=75, .sustain=100, .release=60,
  .lfo={.speed=0, .attack=0, .pmd=0},
  .op=
  {
    {.level=71, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=70, .sustain=0, .loop=false}, 
    {.level=23, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=57, .attack=0, .decay=70, .sustain=0, .loop=false}, 
    {.level=20, .pitch={.fixed=false, .coarse=5, .fine=0}, .detune=51, .attack=0, .decay=70, .sustain=0, .loop=false}, 
    {.level=40, .pitch={.fixed=false, .coarse=1, .fine=0}, .detune=50, .attack=0, .decay=50, .sustain=0, .loop=false}
  }
};

void ProceduralMusic::begin()
{
  generateMeasure();
  nextNote();
}

void ProceduralMusic::nextNote()
{
  int8_t chordVelocity = (parameters[parameter].bassChords) ? 127:64;
  if (currentNote == measure.numNotes)
    generateMeasure();
  /*sources[0] = &*/Audio::FMSynthSource& melodySource = Audio::play<0>(piano, measure.melody[currentNote], 127);
  if (currentNote == 0) //play chords
  {
    /*sources[1] = &*/Audio::FMSynthSource& chord1Source = Audio::play<1>(piano, measure.chord[0], chordVelocity);
    /*sources[2] = &*/Audio::FMSynthSource& chord2Source = Audio::play<2>(piano, measure.chord[1], chordVelocity);
    /*sources[3] = &*/Audio::FMSynthSource& chord3Source = Audio::play<3>(piano, measure.chord[2], chordVelocity);
  }
  Schedule::after<100>(tempos[parameter % 4] / measure.durations[currentNote], &ProceduralMusic::nextNote);
  ++currentNote;
}

void ProceduralMusic::generateMeasure()
{
  uint8_t availableNotes = 8;
  uint8_t availableChords = 0;
  uint8_t chance;
  currentNote = 0;
  measure.numNotes = 0;
  while (availableNotes > 0)
  {
    //pick a random note
    chance = rand() % 100;
    if (chance < parameters[parameter].movementChances[0]) //single step
    {
      if (lastNote == 0)
        measure.melody[measure.numNotes] = lastNote + 1;
      else if (lastNote == 11)
        measure.melody[measure.numNotes] = lastNote - 1;
      else if ((rand() & 128) == 0)
        measure.melody[measure.numNotes] = lastNote - 1;
      else
        measure.melody[measure.numNotes] = lastNote + 1;
    }
    else if (chance < parameters[parameter].movementChances[1]) //skip
    {
      if (lastNote <= 1)
        measure.melody[measure.numNotes] = lastNote + 2;
      else if (lastNote >= 10)
        measure.melody[measure.numNotes] = lastNote - 2;
      else if ((rand() & 128) == 0)
        measure.melody[measure.numNotes] = lastNote - 2;
      else
        measure.melody[measure.numNotes] = lastNote + 2;
    }
    else
    {
      chance = ((rand() >> 3) & 3) + 3; //jump 3, 4, 5, or 6 notes
      if (lastNote < chance)
        measure.melody[measure.numNotes] = lastNote + chance;
      else if (lastNote + chance > 11)
        measure.melody[measure.numNotes] = lastNote - chance;
      else if ((rand() & 128) == 0)
        measure.melody[measure.numNotes] = lastNote - chance;
      else
        measure.melody[measure.numNotes] = lastNote + chance;
    }
    lastNote = measure.melody[measure.numNotes];
    if (parameters[parameter].bassChords)
      measure.melody[measure.numNotes] = trebleNotes[measure.melody[measure.numNotes]];
    else
      measure.melody[measure.numNotes] = bassNotes[measure.melody[measure.numNotes]];
    //pick a random duration
    chance = rand() % 100;
    if (chance < parameters[parameter].durationChances[0] && availableNotes == 8)
    {
      measure.durations[measure.numNotes++] = 1;
      availableNotes = 0;
    }
    else if (chance < parameters[parameter].durationChances[1] && availableNotes >= 4)
    {
      measure.durations[measure.numNotes++] = 2;
      availableNotes -= 4;
    }
    else if (chance < parameters[parameter].durationChances[2] && availableNotes >= 2)
    {
      measure.durations[measure.numNotes++] = 4;
      availableNotes -= 2;
    }
    else
    {
      measure.durations[measure.numNotes++] = 8;
      availableNotes -= 1;
    }
  }
  //Pick a chord
  uint8_t firstNote = measure.melody[0];
  if (parameters[parameter].bassChords)
    firstNote += 2;
  else
    firstNote += 5;
  firstNote %= 12;
  for (uint8_t i = 0; i < numChords; ++i)
  {
    if (chords[i][0] == firstNote || chords[i][1] == firstNote || chords[i][2] == firstNote)
      ++availableChords;
  }
  chance = rand() % availableChords + 1;
  for (uint8_t i = 0; i < numChords; ++i)
  {
    if (chords[i][0] == firstNote || chords[i][1] == firstNote || chords[i][2] == firstNote)
      --chance;
    if (chance == 0)
    {
      measure.chord[0] = chords[i][0];
      measure.chord[1] = chords[i][1];
      measure.chord[2] = chords[i][2];
      break;
    }
  }
  if (parameters[parameter].bassChords)
  {
    measure.chord[0] = bassNotes[measure.chord[0]];
    measure.chord[1] = bassNotes[measure.chord[1]];
    measure.chord[2] = bassNotes[measure.chord[2]];
  }
  else
  {
    measure.chord[0] = trebleNotes[measure.chord[0]];
    measure.chord[1] = trebleNotes[measure.chord[1]];
    measure.chord[2] = trebleNotes[measure.chord[2]];
  }
}
