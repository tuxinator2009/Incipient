#ifndef DISPLAY_H
#define DISPLAY_H

#include <cstdint>
#include <Pokitto.h>
#include "FixedPoints/FixedPoints.h"
#include "Types.h"

class Display
{
  public:
    static void update();
    static void drawCreature(const Creature &creature, bool showIndicator=true);
    static void drawParticle(const Particle &particle);
    static void setTonePacing(uint8_t t, uint8_t p);
    static int getNumShapes() {return numShapes;}
    static bool isOnScreen(const Point &point)
    {
      if (point.x < 0)
        return false;
      if (point.x >= 220)
        return false;
      if (point.y < 0)
        return false;
      if (point.y >= 176)
        return false;
      return true;
    }
  private:
    struct InternalPoint
    {
      int16_t x;
      int16_t y;
    };
    struct InternalLine
    {
      InternalPoint p;
      InternalPoint end;
      InternalPoint d;
      int16_t sx;
      int16_t err;
    };
    struct InternalCircle
    {
      InternalPoint center;
      int16_t radius;
    };
    struct InternalTriangle
    {
      InternalPoint p1;
      InternalPoint p2;
      InternalPoint p3;
    };
    struct InternalShape
    {
      union
      {
        InternalLine line;
        InternalTriangle triangle;
        InternalCircle circle;
        InternalPoint point;
      };
      int16_t firstLine;
      int16_t lastLine;
      uint16_t color;
      uint8_t alpha;
    };
    static void interpolateModels(const Model *model1, const Model *model2, const Transform &transform, const fixed_t &scale, uint16_t colors[4], uint8_t alphas[4], int16_t t);
    static void drawModel(const Model *model, const Transform &transform, const fixed_t &scale, uint16_t colors[4], uint8_t alphas[4]);
    static void drawIndicator(const VectorF &offset, uint16_t color);
    static void rotateBGPalette();
    static void drawShapes(uint16_t *scanline, int y, uint8_t &max);
    static uint16_t alphaBlend(uint32_t fg, uint32_t bg, uint8_t alpha);
    static bool getIntersect(int16_t &x /*OUT*/, int16_t y, const InternalPoint &p1, const InternalPoint &p2);
    static void getTriangleStartEnd(int16_t &start /*OUT*/, int16_t &end /*OUT*/, int16_t y, InternalTriangle &triangle);
    static void drawTriangle(uint16_t *scanline, int y, InternalTriangle &triangle, uint16_t color, uint8_t alpha);
    static void drawCircle(uint16_t *scanline, int y, InternalCircle &circle, uint16_t color, uint8_t alpha);
    static void drawLine(uint16_t *scanline, int y, InternalLine &line, uint16_t color, uint8_t alpha);
    static void drawPoint(uint16_t *scanline, int y, InternalPoint &point, uint16_t color, uint8_t alpha);
    static void interpolateTriangle(const Triangle &in1, const Triangle &in2, const Transform &transform, InternalTriangle &out, int16_t t, int16_t &firstLine, int16_t &lastLine);
    static void transformTriangle(const Triangle &in, const Transform &transform, const Point &origin, InternalTriangle &out, int16_t &firstLine, int16_t &lastLine);
    static void transformTriangle(const Triangle &in, const Transform &transform, InternalTriangle &out, int16_t &firstLine, int16_t &lastLine);
    static void interpolateCircle(const Circle &in1, const Circle &in2, const Transform &transform, InternalCircle &out, int16_t t, int16_t &firstLine, int16_t &lastLine);
    static void transformCircle(const Circle &in, const Transform &transform, const Point &origin, InternalCircle &out, int16_t &firstLine, int16_t &lastLine);
    static void transformCircle(const Circle &in, const Transform &transform, InternalCircle &out, int16_t &firstLine, int16_t &lastLine);
    static void interpolateLine(const Line &in1, const Line &in2, const Transform &transform, InternalLine &out, int16_t t, int16_t &firstLine, int16_t &lastLine);
    static void transformLine(const Line &in, const Transform &transform, const Point &origin, InternalLine &out, int16_t &firstLine, int16_t &lastLine);
    static void transformLine(const Line &in, const Transform &transform, InternalLine &out, int16_t &firstLine, int16_t &lastLine);
    static void interpolatePoint(const Point &in1, const Point &in2, const Transform &transform, InternalPoint &out, int16_t t, int16_t &firstLine, int16_t &lastLine);
    static void transformPoint(const Point &in, const Transform &transform, const Point &origin, InternalPoint &out, int16_t &firstLine, int16_t &lastLine);
    static void transformPoint(const Point &in, const Transform &transform, InternalPoint &out, int16_t &firstLine, int16_t &lastLine);
    static constexpr int MAX_SHAPES = 512;
    static InternalShape shapes[MAX_SHAPES];
    static const uint32_t plasmaDelay[4];
    static const uint16_t foodColors[4];
    static const uint8_t foodAlphas[4];
    static uint8_t maxShapesPerLine[176];
    static uint32_t lastPaletteRotation;
    static int numShapes;
    static uint8_t tone;
    static uint8_t pacing;
    static constexpr uint8_t ShapeType = 192;
    static constexpr uint8_t ShapePoint = 0;
    static constexpr uint8_t ShapeLine = 64;
    static constexpr uint8_t ShapeCircle = 128;
    static constexpr uint8_t ShapeTriangle = 192;
};

#endif //DISPLAY_H
