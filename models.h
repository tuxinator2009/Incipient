#ifndef MODELS_H
#define MODELS_H

#include "Display.h"

namespace Models
{
  const Display::Triangle triangles[] =
  {
    {10,  0, 10, 15, 20, 30}, //fontA
    {10,  0,  0, 30, 10, 15},
    { 0, 30, 20, 30, 10, 20}, //fontB
    { 0,  0,  0, 30,  5, 15},
    { 0,  0, 10, 10, 20,  0},
    {20,  0,  5, 15, 12, 15},
    {12, 15,  5, 15, 20, 30},
    {16, 29, 19, 30, 19, 24}, //fontC
    { 0,  0,  0, 30,  5, 15},
    { 0,  0,  2,  5, 19,  0},
    { 0, 30, 19, 30,  2, 24},
    {16,  0, 20,  5, 19,  0},
    { 0, 30, 10, 30, 10, 25}, //fontD
    { 0,  0,  0, 30,  5, 15},
    {10,  0, 10, 30, 20, 15},
    { 0,  0, 10,  5, 10,  0},
    { 0, 30, 20, 30,  2, 24}, //fontE
    { 0,  0,  0, 30,  5, 15},
    { 0,  0,  2,  5, 20,  0},
    { 2, 21, 15, 15,  5, 15},
    { 2,  9,  5, 15, 15, 15},
    { 3, 21, 15, 15,  5, 15}, //fontF
    { 0,  0,  0, 30,  5, 15},
    { 0,  0,  2,  5, 20,  0},
    { 3,  9,  5, 15, 15, 15},
    {14, 18, 16, 21, 20, 18}, //fontG
    { 0,  0,  0, 30,  5, 15},
    {16,  1, 20,  6, 20,  0},
    { 0, 30, 20, 30,  1, 24},
    { 0,  0,  1,  6, 20,  0},
    {11, 27, 20, 30, 20, 24},
    {14, 26, 20, 24, 20, 18},
    {20,  0, 15, 15, 20, 30}, //fontH
    { 0,  0,  0, 30,  5, 15},
    { 5, 15, 15, 15,  4, 12},
    { 5, 15, 16, 18, 15, 15},
    {10, 30, 20, 30, 11, 26}, //fontI
    { 0,  0,  9,  3, 10,  0},
    {10,  0,  5, 15, 10, 30},
    {10,  0, 10, 30, 15, 15},
    { 0, 30, 10, 30,  9, 26},
    {10,  0, 11,  3, 20,  0},
    {20,  0, 15, 15, 20, 30}, //fontJ
    {20, 30, 18, 24,  0, 30},
    { 9, 27,  0, 24,  0, 30},
    { 6, 26,  0, 18,  0, 24},
    { 5, 15, 20,  0,  4, 12}, //fontK
    { 0,  0,  0, 30,  5, 15},
    { 5, 15, 20, 30,  8, 12},
    {20, 30,  2, 24,  0, 30}, //fontL
    { 0,  0,  0, 30,  5, 15},
    {20,  0, 17, 15, 20, 30}, //fontM
    { 0,  0,  0, 30,  3, 15},
    { 0,  0,  1,  5,  7,  9},
    {20,  0, 13,  9, 19,  5},
    {10, 10,  4,  5, 10, 13},
    {10, 10, 10, 13, 16,  6},
    { 0,  0,  0, 30,  3, 15}, //fontN
    {20,  0, 17, 15, 20, 30},
    { 0,  0,  1,  5, 19, 25},
    {19, 25,  1,  5, 20, 30},
    { 0, 15,  0, 27,  3, 27}, //fontO
    {20, 27,  0, 27, 11, 30},
    { 0,  2, 20,  2, 11,  0},
    {20, 15, 17, 27, 20, 27},
    { 0,  2,  0, 15,  3,  3},
    {20,  2, 17,  3, 20, 15},
    {16,  0, 18, 10, 20,  5}, //fontP
    { 0,  0,  0, 30, 10, 15},
    {20,  5, 17, 12, 20, 12},
    {20, 12,  8, 12, 10, 15},
    { 0,  0,  2,  3, 16,  0},
    {11,  1, 17,  5, 16,  0},
    {20, 30, 15, 27,  7, 30}, //fontQ
    {15, 27,  0, 27,  7, 30},
    { 0, 15,  0, 27,  3, 27},
    { 0,  2, 15,  3,  8,  0},
    {15, 15, 12, 27, 15, 27},
    { 0,  2,  0, 15,  3,  3},
    {15,  3, 12,  3, 15, 15},
    { 0,  0,  2,  3, 16,  0}, //fontR
    { 0,  0,  0, 30, 10, 15},
    {20,  5, 17, 12, 20, 12},
    {20, 12,  8, 12, 10, 15},
    {16,  0, 18, 10, 20,  5},
    {11,  1, 17,  5, 16,  0},
    {20, 30, 10, 15,  6, 21},
    { 0, 31, 16, 31,  2, 28}, //fontS
    { 0,  1,  2,  3, 16,  0},
    {11,  2, 20,  4, 16,  0},
    { 0,  1,  0, 11,  2,  3},
    { 0, 11, 18, 21,  1,  7},
    {11, 30, 16, 31, 17, 26},
    {16, 31, 20, 26, 18, 21},
    { 0, 11, 17, 26, 18, 21},
    {13,  3,  7,  3, 10, 30}, //fontT
    { 0,  3, 20,  3, 10,  0},
    {20,  0, 16, 28, 20, 28}, //fontU
    {20, 28,  0, 28, 10, 31},
    { 0,  0,  0, 28,  3, 28},
    {20,  0, 10, 21, 10, 31}, //fontV
    { 0,  0, 10, 31, 10, 21},
    {20,  0, 13, 27, 15, 31}, //fontW
    { 5, 31, 15, 31, 10, 21},
    { 0,  0,  5, 31,  7, 27},
    { 0,  0,  8, 18, 10, 14}, //fontX
    {20,  0, 10, 14, 12, 18},
    { 0, 31, 10, 20,  8, 18},
    {20, 31, 12, 18, 10, 20},
    {12, 18, 10, 14,  8, 18},
    {12, 18,  8, 18, 10, 20},
    {10, 11,  8, 13, 10, 31}, //fontY
    { 0,  0,  8, 13, 10, 11},
    {20,  0, 10, 11, 12, 13},
    {10, 11, 10, 31, 12, 13},
    { 0, 30, 20, 29,  4, 24}, //fontZ
    { 0,  0,  8,  4, 20,  0},
    { 0, 30, 20,  0,  8,  4},
    {-5, -5, -4, -1,  0,  0}, //mouthOpen
    { 5, -5,  0,  0,  4, -1},
    { 0, -7, -2, -4,  0,  0}, //mouthClosed
    { 0, -7,  0,  0,  2, -4},
    { 0,  0, -3,  3,  3,  3}, //body
    { 0, -2,  1, -4, -1, -4}, //exitIndicator
    { 0,  2, -1,  4,  1,  4},
    {-2,  0, -4, -1, -4,  1},
    { 2,  0,  4,  1,  4, -1},
  };
  const Display::Circle circles[] =
  {
    { 0,  0,  2}, //exit
  };
  const Display::Model fontA = {2, 0, 0, 0, triangles + 0, circles + 0};
  const Display::Model fontB = {5, 0, 0, 0, triangles + 2, circles + 0};
  const Display::Model fontC = {5, 0, 0, 0, triangles + 7, circles + 0};
  const Display::Model fontD = {4, 0, 0, 0, triangles + 12, circles + 0};
  const Display::Model fontE = {5, 0, 0, 0, triangles + 16, circles + 0};
  const Display::Model fontF = {4, 0, 0, 0, triangles + 21, circles + 0};
  const Display::Model fontG = {7, 0, 0, 0, triangles + 25, circles + 0};
  const Display::Model fontH = {4, 0, 0, 0, triangles + 32, circles + 0};
  const Display::Model fontI = {6, 0, 0, 0, triangles + 36, circles + 0};
  const Display::Model fontJ = {4, 0, 0, 0, triangles + 42, circles + 0};
  const Display::Model fontK = {3, 0, 0, 0, triangles + 46, circles + 0};
  const Display::Model fontL = {2, 0, 0, 0, triangles + 49, circles + 0};
  const Display::Model fontM = {6, 0, 0, 0, triangles + 51, circles + 0};
  const Display::Model fontN = {4, 0, 0, 0, triangles + 57, circles + 0};
  const Display::Model fontO = {6, 0, 0, 0, triangles + 61, circles + 0};
  const Display::Model fontP = {6, 0, 0, 0, triangles + 67, circles + 0};
  const Display::Model fontQ = {7, 0, 0, 0, triangles + 73, circles + 0};
  const Display::Model fontR = {7, 0, 0, 0, triangles + 80, circles + 0};
  const Display::Model fontS = {8, 0, 0, 0, triangles + 87, circles + 0};
  const Display::Model fontT = {2, 0, 0, 0, triangles + 95, circles + 0};
  const Display::Model fontU = {3, 0, 0, 0, triangles + 97, circles + 0};
  const Display::Model fontV = {2, 0, 0, 0, triangles + 100, circles + 0};
  const Display::Model fontW = {3, 0, 0, 0, triangles + 102, circles + 0};
  const Display::Model fontX = {6, 0, 0, 0, triangles + 105, circles + 0};
  const Display::Model fontY = {4, 0, 0, 0, triangles + 111, circles + 0};
  const Display::Model fontZ = {3, 0, 0, 0, triangles + 115, circles + 0};
  const Display::Model mouthOpen = {2, 0, 0, 0, triangles + 118, circles + 0};
  const Display::Model mouthClosed = {2, 0, 0, 0, triangles + 120, circles + 0};
  const Display::Model body = {1, 0, 0, 3, triangles + 122, circles + 0};
  const Display::Model exit = {0, 1, 0, 0, triangles + 123, circles + 0};
  const Display::Model exitIndicator = {4, 0, 0, 0, triangles + 123, circles + 1};
  const Display::Model *models[] =
  {
    &fontA,
    &fontB,
    &fontC,
    &fontD,
    &fontE,
    &fontF,
    &fontG,
    &fontH,
    &fontI,
    &fontJ,
    &fontK,
    &fontL,
    &fontM,
    &fontN,
    &fontO,
    &fontP,
    &fontQ,
    &fontR,
    &fontS,
    &fontT,
    &fontU,
    &fontV,
    &fontW,
    &fontX,
    &fontY,
    &fontZ,
    &mouthOpen,
    &mouthClosed,
    &body,
    &exit,
    &exitIndicator,
  };
};


#endif //MODELS_H
