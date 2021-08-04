#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>
#include <Pokitto.h>
#include "FixedPoints/FixedPoints.h"

using SQ15x16 = SFixed<15, 16>;

struct Model;

class Display
{
  public:
    struct Triangle
    {
      int16_t p1[2];
      int16_t p2[2];
      int16_t p3[2];
    };
    struct Circle
    {
      int16_t center[2];
      int16_t radius;
    };
    struct Particle
    {
      SQ15x16 offset[2];
      SQ15x16 vector[2];
      int16_t velocity;
      int16_t distance;
      uint16_t color;
      uint8_t life;
    };
    struct Model
    {
      uint8_t numTriangles;
      uint8_t numCircles;
      int16_t link[2];
      const Triangle *triangles;
      const Circle *circles;
    };
    struct Transform
    {
      SQ15x16 offset[2];
      SQ15x16 xVector[2];
      SQ15x16 yVector[2];
    };
    struct BodyPart
    {
      Transform transform;
      const Model *model;
      BodyPart *previous;
      BodyPart *next;
      SQ15x16 scale;
      uint16_t color;
      uint8_t alpha;
    };
    struct Creature
    {
      const Model *mouthClosed;
      BodyPart *head;
      BodyPart *tail;
      Circle offscreen;
      uint8_t t;
    };
    struct InternalTriangle
    {
      int16_t p1[2];
      int16_t p2[2];
      int16_t p3[2];
      int16_t firstLine;
      int16_t lastLine;
      uint16_t color;
      uint8_t alpha;
    };
    struct InternalCircle
    {
      int16_t center[2];
      int16_t radius;
      uint16_t color;
      int16_t firstLine;
      int16_t lastLine;
      uint8_t alpha;
    };
    struct InternalParticle
    {
      int16_t x;
      int16_t y;
      uint16_t color;
      uint8_t alpha;
    };
    static void update();
    static void drawCreature(const Creature &creature);
    static void interpolateModels(const Model *model1, const Model *model2, const Transform &transform, const SQ15x16 &scale, uint16_t color, uint8_t alpha, int16_t t);
    static void drawModel(const Model *model, const Transform &transform, const SQ15x16 &scale, uint16_t color, uint8_t alpha);
    static void drawTriangle(const Triangle &triangle, uint16_t color, uint8_t alpha);
    static void drawCircle(const Circle &circle, uint16_t color, uint8_t alpha);
    static void drawParticle(const Particle &particle);
    static void drawPoint(int16_t x, int16_t y, uint16_t color, uint8_t alpha);
    static void setTonePacing(uint8_t t, uint8_t p);
    static void rotPoint(SQ15x16 &x, SQ15x16 &y, uint8_t amount);
    static void slerpVector(SQ15x16 &x1, SQ15x16 &y1, const SQ15x16 &x2, const SQ15x16 &y2, const SQ15x16 &amount, const SQ15x16 &scale);
    static bool floatsEqual(SQ15x16 f1, SQ15x16 f2);
    static bool isOffScreen(int16_t x, int16_t y);
    static void clipToScreen(int16_t &x, int16_t &y);
    static SQ15x16 fastInverseSquareRoot(float value);
    static InternalTriangle *getInternalTriangles() {return triangles;}
    static uint8_t getNumTriangles() {return numTriangles;}
    static inline constexpr SQ15x16 sqrt( SQ15x16 x ){
      uint32_t t = 0, q = 0, b = 0x40000000, r = x.getInternal();
      while( b > 0x4000 ){
        t = q;
        t += b;
        if( r >= t )
        {
          r -= t;
          q += b<<1;
        }
        r <<= 1;
        b >>= 1;
      }
      q >>= 8;
      x = SQ15x16::fromInternal(q);
      return x;
    }
    static const SQ15x16 sinTable[];
    static const SQ15x16 cosTable[];
  private:
    static void rotateBGPalette();
    static uint16_t alphaBlendRGB565(uint32_t fg, uint32_t bg, uint8_t alpha);
    static bool getIntersect(int16_t &x /*OUT*/, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    static void getTriangleStartEnd(int16_t &start /*OUT*/, int16_t &end /*OUT*/, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3);
    static void drawTriangles(uint16_t *scanline, int y, uint8_t &max);
    static void drawCircles(uint16_t *scanline, int y, uint8_t &max);
    static void drawParticles(uint16_t *scanline, int y);
    static void interpolateCircle(const Circle &in1, const Circle &in2, const Transform &transform, const SQ15x16 &scale, InternalCircle *out, int16_t t);
    static void transformCircle(const Circle &in, const Transform &transform, const SQ15x16 &scale, InternalCircle *out);
    static void interpolateTriangle(const Triangle &in1, const Triangle &in2, const Transform &transform, InternalTriangle *out, int16_t t);
    static void transformTriangle(const Triangle &in, const Transform &transform, InternalTriangle *out);
    static InternalTriangle triangles[128];
    static InternalCircle circles[32];
    static InternalParticle particles[32];
    static uint32_t lastPaletteRotation;
    static uint8_t maxTrianglesPerLine[176];
    static uint8_t maxCirclesPerLine[176];
    static uint8_t numTriangles;
    static uint8_t numCircles;
    static uint8_t numParticles;
    static uint8_t tone;
    static uint8_t pacing;
    static const uint8_t maxTriangles;
    static const uint8_t maxCircles;
    static const uint8_t maxParticles;
    static const uint32_t plasmaDelay[4];
};

#endif //DISPLAY_H
