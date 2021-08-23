#include <cstdint>
#include <Pokitto.h>
#include "FixedPoints/FixedPoints.h"
#include "Display.h"
#include "background.h"

//#define DEBUG_WEAKSPOTS

void copy4BPPto16BPPfast(uint16_t *scanline, const uint8_t *src, const uint32_t *palette)
{
  uint32_t *pixels = reinterpret_cast<uint32_t*>(scanline);
  for (int i = 0; i < 110; ++i)
    *pixels++ = palette[*src++];
}

#ifdef POK_SIM
void flushLine16(uint16_t *line)
{
  Pokitto::pumpDRAMdata(line, 220);
}
#else
extern "C" void flushLine16(uint16_t *line);
#endif

Display::InternalShape Display::shapes[Display::MAX_SHAPES];
const uint32_t Display::plasmaDelay[4] = {100, 75, 50, 25};
const uint16_t Display::foodColors[4] = {0xffff, 0xffff, 0xc618, 0xc618};
const uint8_t Display::foodAlphas[4] = {16, 16, 8, 8};
uint8_t Display::maxShapesPerLine[176]{0};
uint32_t Display::lastPaletteRotation = 0;
int Display::numShapes = 0;
uint8_t Display::tone = 0;
uint8_t Display::pacing = 0;

void Display::update()
{
  if (Pokitto::Core::getTime() - lastPaletteRotation > plasmaDelay[pacing])
  {
    rotateBGPalette();
    lastPaletteRotation = Pokitto::Core::getTime();
  }
  Pokitto::lcdPrepareRefresh();
  for (int y = 0; y < 176; ++y)
  {
    uint16_t scanline[220];
    copy4BPPto16BPPfast(scanline, bgImage + 110 * y, bgPalette);
    drawShapes(scanline, y, maxShapesPerLine[y]);
    flushLine16(scanline);
  }
  numShapes = 0;
}

void Display::drawCreature(const Creature &creature, bool showIndicator)
{
  BodyPart *body = creature.head->next;
  if (creature.mouthClosed == nullptr)
    drawModel(creature.head->model, creature.head->transform, creature.head->scale, creature.head->colors, creature.head->alphas);
  else
    interpolateModels(creature.head->model, creature.mouthClosed, creature.head->transform, creature.head->scale, creature.head->colors, creature.head->alphas, creature.t);
  while (body != nullptr)
  {
    drawModel(body->model, body->transform, body->scale, body->colors, body->alphas);
    body = body->next;
  }
  if (!showIndicator)
    return;
  if (creature.head->transform.offset.x < 0 || creature.head->transform.offset.x >= 220 || creature.head->transform.offset.y < 0 || creature.head->transform.offset.y >= 176)
    drawIndicator(creature.head->transform.offset, creature.head->colors[Shape::Line]);
}

void Display::drawParticle(const Particle &particle)
{
  if (numShapes == MAX_SHAPES)
    return;
  shapes[numShapes].point.x = int16_t(particle.offset.x + particle.vector.x * particle.distance);
  shapes[numShapes].point.y = int16_t(particle.offset.y + particle.vector.y * particle.distance);
  shapes[numShapes].firstLine = shapes[numShapes].point.y;
  shapes[numShapes].lastLine = shapes[numShapes].point.y;
  shapes[numShapes].color = particle.color;
  shapes[numShapes].alpha = ShapePoint + std::min((uint8_t)32, particle.life);
  if (shapes[numShapes].point.x < 0)
    return;
  if (shapes[numShapes].point.x >= 220)
    return;
  if (shapes[numShapes].point.y < 0)
    return;
  if (shapes[numShapes].point.y >= 176)
    return;
  if (shapes[numShapes].alpha == 0)
    return;
  ++maxShapesPerLine[shapes[numShapes].firstLine];
  ++numShapes;
}

void Display::setTonePacing(uint8_t t, uint8_t p)
{
  tone = t;
  pacing = p;
}

void Display::interpolateModels(const Model *model1, const Model *model2, const Transform &transform, const fixed_t &scale, uint16_t colors[4], uint8_t alphas[4], int16_t t)
{
  if (numShapes == MAX_SHAPES)
    return;
#ifdef DEBUG_WEAKSPOTS
  if (model1->weakspot.radius != 0)
  {
    shapes[numShapes].color = colors[Shape::Circle];
    shapes[numShapes].alpha = 32 + ShapeCircle;
    transformCircle(model1->weakspot, transform, shapes[numShapes].circle, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    if (shapes[numShapes].firstLine < 0)
      shapes[numShapes].firstLine = 0;
    if (shapes[numShapes].lastLine > 175)
      shapes[numShapes].lastLine = 175;
    if (shapes[numShapes].firstLine <= 175 && shapes[numShapes].lastLine >= 0)
    {
      for (int16_t y = shapes[numShapes].firstLine; y <= shapes[numShapes].lastLine; ++y)
        ++maxShapesPerLine[y];
      ++numShapes;
    }
  }
  if (numShapes == MAX_SHAPES)
    return;
#endif
  for (int i = 0; i < model1->numShapes && numShapes < MAX_SHAPES; ++i)
  {
    if (alphas[model1->shapes[i].type] == 0)
      continue;
    shapes[numShapes].color = colors[model1->shapes[i].type];
    shapes[numShapes].alpha = std::min((uint8_t)32, alphas[model1->shapes[i].type]) + (model1->shapes[i].type << 6);
    if (model1->shapes[i].type == Shape::Point)
      interpolatePoint(model1->shapes[i].point, model2->shapes[i].point, transform, shapes[numShapes].point, t, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    else if (model1->shapes[i].type == Shape::Line)
      interpolateLine(model1->shapes[i].line, model2->shapes[i].line, transform, shapes[numShapes].line, t, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    else if (model1->shapes[i].type == Shape::Circle)
      interpolateCircle(model1->shapes[i].circle, model2->shapes[i].circle, transform, shapes[numShapes].circle, t, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    else if (model1->shapes[i].type == Shape::Triangle)
      interpolateTriangle(model1->shapes[i].triangle, model2->shapes[i].triangle, transform, shapes[numShapes].triangle, t, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    if (shapes[numShapes].firstLine < 0)
      shapes[numShapes].firstLine = 0;
    if (shapes[numShapes].lastLine > 175)
      shapes[numShapes].lastLine = 175;
    if (shapes[numShapes].firstLine <= 175 && shapes[numShapes].lastLine >= 0)
    {
      for (int16_t y = shapes[numShapes].firstLine; y <= shapes[numShapes].lastLine; ++y)
        ++maxShapesPerLine[y];
      ++numShapes;
    }
  }
}

void Display::drawModel(const Model *model, const Transform &transform, const fixed_t &scale, uint16_t colors[4], uint8_t alphas[4])
{
  if (numShapes == MAX_SHAPES)
    return;
#ifdef DEBUG_WEAKSPOTS
  if (model->weakspot.radius != 0)
  {
    shapes[numShapes].color = colors[Shape::Circle];
    shapes[numShapes].alpha = 32 + ShapeCircle;
    transformCircle(model->weakspot, transform, shapes[numShapes].circle, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    if (shapes[numShapes].firstLine < 0)
      shapes[numShapes].firstLine = 0;
    if (shapes[numShapes].lastLine > 175)
      shapes[numShapes].lastLine = 175;
    if (shapes[numShapes].firstLine <= 175 && shapes[numShapes].lastLine >= 0)
    {
      for (int16_t y = shapes[numShapes].firstLine; y <= shapes[numShapes].lastLine; ++y)
        ++maxShapesPerLine[y];
      ++numShapes;
    }
  }
  if (numShapes == MAX_SHAPES)
    return;
#endif
  for (int i = 0; i < model->numShapes && numShapes < MAX_SHAPES; ++i)
  {
    if (alphas[model->shapes[i].type] == 0)
      continue;
    shapes[numShapes].color = colors[model->shapes[i].type];
    shapes[numShapes].alpha = std::min((uint8_t)32, alphas[model->shapes[i].type]) + (model->shapes[i].type << 6);
    if (model->shapes[i].type == Shape::Point)
      transformPoint(model->shapes[i].point, transform, shapes[numShapes].point, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    else if (model->shapes[i].type == Shape::Line)
      transformLine(model->shapes[i].line, transform, shapes[numShapes].line, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    else if (model->shapes[i].type == Shape::Circle)
      transformCircle(model->shapes[i].circle, transform, shapes[numShapes].circle, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    else if (model->shapes[i].type == Shape::Triangle)
      transformTriangle(model->shapes[i].triangle, transform, shapes[numShapes].triangle, shapes[numShapes].firstLine, shapes[numShapes].lastLine);
    if (shapes[numShapes].firstLine < 0)
      shapes[numShapes].firstLine = 0;
    if (shapes[numShapes].lastLine > 175)
      shapes[numShapes].lastLine = 175;
    if (shapes[numShapes].firstLine <= 175 && shapes[numShapes].lastLine >= 0)
    {
      for (int16_t y = shapes[numShapes].firstLine; y <= shapes[numShapes].lastLine; ++y)
        ++maxShapesPerLine[y];
      ++numShapes;
    }
  }
}

void Display::drawIndicator(const VectorF &offset, uint16_t color)
{
  if (numShapes == MAX_SHAPES)
    return;
  shapes[numShapes].circle.center.x = std::max((int16_t)0, std::min((int16_t)219, (int16_t)offset.x));
  shapes[numShapes].circle.center.y = std::max((int16_t)0, std::min((int16_t)175, (int16_t)offset.y));
  shapes[numShapes].circle.radius = 4;
  shapes[numShapes].firstLine = shapes[numShapes].circle.center.y - 4;
  shapes[numShapes].lastLine = shapes[numShapes].circle.center.y + 4;
  shapes[numShapes].color = color;
  shapes[numShapes].alpha = ShapeCircle + 32;
  if (shapes[numShapes].firstLine < 0)
    shapes[numShapes].firstLine = 0;
  if (shapes[numShapes].lastLine > 175)
    shapes[numShapes].lastLine = 175;
  if (shapes[numShapes].firstLine <= 175 && shapes[numShapes].lastLine >= 0)
  {
    for (int16_t y = shapes[numShapes].firstLine; y <= shapes[numShapes].lastLine; ++y)
      ++maxShapesPerLine[y];
    ++numShapes;
  }
}

void Display::rotateBGPalette()
{
  for (int i = 0; i < 4; ++i)
  {
    uint16_t first = bgPalettes[i*16];
    for (int j = 0; j < 15; ++j)
      bgPalettes[i*16+j] = bgPalettes[i*16+j+1];
    bgPalettes[i*16+15] = first;
  }
  for (int i = 0; i < 16; ++i)
  {
    for (int j = 0; j < 16; ++j)
      bgPalette[i*16+j] = ((uint32_t)bgPalettes[tone*16+i]) + ((uint32_t)bgPalettes[tone*16+j] << 16);
  }
}

void Display::drawShapes(uint16_t *scanline, int y, uint8_t &max)
{
  InternalShape *shape = shapes;
  for (int i = 0; i < numShapes && max > 0; ++i, ++shape)
  {
    if (y < shape->firstLine || y > shape->lastLine)
      continue;
    if ((shape->alpha & ShapeType) == ShapeTriangle)
      drawTriangle(scanline, y, shape->triangle, shape->color, shape->alpha & ~ShapeType);
    else if ((shape->alpha & ShapeType) == ShapeCircle)
      drawCircle(scanline, y, shape->circle, shape->color, shape->alpha & ~ShapeType);
    else if ((shape->alpha & ShapeType) == ShapeLine)
      drawLine(scanline, y, shape->line, shape->color, shape->alpha & ~ShapeType);
    else
      drawPoint(scanline, y, shape->point, shape->color, shape->alpha & ~ShapeType);
    --max;
  }
}

uint16_t Display::alphaBlend(uint32_t fg, uint32_t bg, uint8_t alpha)
{
  //Fully transparent so return background color
  if (alpha == 0)
    return (uint16_t)bg;
  //Fully opaque so return foreground color
  if (alpha == 32)
    return (uint16_t)fg;
  fg = (fg | fg << 16) & 0x07e0f81f;
  bg = (bg | bg << 16) & 0x07e0f81f;
  bg += (fg - bg) * alpha >> 5;
  bg &= 0x07e0f81f;
  return (uint16_t)(bg | bg >> 16);
}

bool Display::getIntersect(int16_t &x /*OUT*/, int16_t y, const InternalPoint &p1, const InternalPoint &p2)
{
  if (p1.y < y && p2.y < y)
    return false;
  if (p1.y > y && p2.y > y)
    return false;
  if (p1.y == p2.y) //shouldn't receive this value
    return false;
  x = p1.x + (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y);
  return true;
}

void Display::getTriangleStartEnd(int16_t &start /*OUT*/, int16_t &end /*OUT*/, int16_t y, InternalTriangle &triangle)
{
  if (triangle.p1.y == y)
  {
    if (triangle.p2.y == y)
    {
      start = triangle.p1.x;
      end = triangle.p2.x;
    }
    else if (triangle.p3.y == y)
    {
      start = triangle.p1.x;
      end = triangle.p3.x;
    }
    else
    {
      start = triangle.p1.x;
      end = triangle.p1.x; //in case p2->p3 doesn't intersect scanline
      getIntersect(end, y, triangle.p2, triangle.p3);
    }
  }
  else if (triangle.p2.y == y)
  {
    if (triangle.p3.y == y)
    {
      start = triangle.p2.x;
      end = triangle.p3.x;
    }
    else
    {
      start = triangle.p2.x;
      end = triangle.p2.x; //in case p3->p1 doesn't intersect scanline
      getIntersect(end, y, triangle.p3, triangle.p1);
    }
  }
  else if (triangle.p3.y == y)
  {
    start = triangle.p3.x;
    end = triangle.p3.x; //in case p1->p2 doesn't intersect scanline
    getIntersect(end, y, triangle.p1, triangle.p2);
  }
  else
  {
    if (getIntersect(start, y, triangle.p1, triangle.p2))
    {
      if (!getIntersect(end, y, triangle.p2, triangle.p3))
        getIntersect(end, y, triangle.p3, triangle.p1);
    }
    else
    {
      getIntersect(start, y, triangle.p2, triangle.p3);
      getIntersect(end, y, triangle.p3, triangle.p1);
    }
  }
  if (start > end)
    swapWT(int16_t,start,end);
  if (start < 0)
    start = 0;
  if (end > 219)
    end = 219;
}

void Display::drawTriangle(uint16_t *scanline, int y, InternalTriangle &triangle, uint16_t color, uint8_t alpha)
{
  int16_t start, end;
  uint16_t *pixel;
  getTriangleStartEnd(start, end, y, triangle);
  pixel = scanline + start;
  for (int16_t x = start; x <= end; ++x, ++pixel) //TODO enable alpha blending
    *pixel = alphaBlend(color, *pixel, alpha);
}

void Display::drawCircle(uint16_t *scanline, int y, InternalCircle &circle, uint16_t color, uint8_t alpha)
{
  int16_t start = circle.center.x - circle.radius;
  int16_t end = circle.center.x + circle.radius;
  int16_t dist2 = (circle.center.y - y) * (circle.center.y - y);
  int16_t r2 = circle.radius * circle.radius;
  uint16_t *pixel;
  while ((circle.center.x - start) * (circle.center.x - start) + dist2 > r2)
  {
    ++start;
    --end;
  }
  if (start < 0)
    start = 0;
  if (end > 219)
    end = 219;
  pixel = scanline + start;
  for (int16_t x = start; x <= end; ++x, ++pixel)
    *pixel = alphaBlend(color, *pixel, alpha);
}

void Display::drawLine(uint16_t *scanline, int y, InternalLine &line, uint16_t color, uint8_t alpha)
{
  uint16_t *pixel = scanline + line.p.x;
  while (line.p.y == y)
  {
    if (line.p.x >= 0 && line.p.x < 220)
      *pixel = alphaBlend(color, *pixel, alpha);
    if (line.p.x == line.end.x && line.p.y == line.end.y) break;
    if (line.err >= line.d.y)
    {
      line.err += line.d.y * 2;
      line.p.x += line.sx;
      pixel += line.sx;
    }
    if (line.err <= line.d.x && line.p.y != line.end.y)
    {
      line.err += line.d.x * 2;
      ++line.p.y;
    }
  }
}

void Display::drawPoint(uint16_t *scanline, int y, InternalPoint &point, uint16_t color, uint8_t alpha)
{
  if (y == point.y)
    scanline[point.x] = alphaBlend(color, scanline[point.x], alpha);
}

void Display::interpolateTriangle(const Triangle &in1, const Triangle &in2, const Transform &transform, InternalTriangle &out, int16_t t, int16_t &firstLine, int16_t &lastLine)
{
  interpolatePoint(in1.p1, in2.p1, transform, out.p1, t, firstLine, lastLine);
  interpolatePoint(in1.p2, in2.p2, transform, out.p2, t, firstLine, lastLine);
  interpolatePoint(in1.p3, in2.p3, transform, out.p3, t, firstLine, lastLine);
  firstLine = std::max((int16_t)0, std::min(out.p1.y, std::min(out.p2.y, out.p3.y)));
  lastLine = std::min((int16_t)175, std::max(out.p1.y, std::max(out.p2.y, out.p3.y)));
}

void Display::transformTriangle(const Triangle &in, const Transform &transform, const Point &origin, InternalTriangle &out, int16_t &firstLine, int16_t &lastLine)
{
  transformPoint(in.p1, transform, origin, out.p1, firstLine, lastLine);
  transformPoint(in.p2, transform, origin, out.p2, firstLine, lastLine);
  transformPoint(in.p3, transform, origin, out.p3, firstLine, lastLine);
  firstLine = std::max((int16_t)0, std::min(out.p1.y, std::min(out.p2.y, out.p3.y)));
  lastLine = std::min((int16_t)175, std::max(out.p1.y, std::max(out.p2.y, out.p3.y)));
}

void Display::transformTriangle(const Triangle &in, const Transform &transform, InternalTriangle &out, int16_t &firstLine, int16_t &lastLine)
{
  transformPoint(in.p1, transform, out.p1, firstLine, lastLine);
  transformPoint(in.p2, transform, out.p2, firstLine, lastLine);
  transformPoint(in.p3, transform, out.p3, firstLine, lastLine);
  firstLine = std::max((int16_t)0, std::min(out.p1.y, std::min(out.p2.y, out.p3.y)));
  lastLine = std::min((int16_t)175, std::max(out.p1.y, std::max(out.p2.y, out.p3.y)));
}

void Display::interpolateCircle(const Circle &in1, const Circle &in2, const Transform &transform, InternalCircle &out, int16_t t, int16_t &firstLine, int16_t &lastLine)
{
  interpolatePoint(in1.center, in2.center, transform, out.center, t, firstLine, lastLine);
  if (t == 0)
    out.radius = in1.radius;
  else if (t == 128)
    out.radius = in2.radius;
  else
    out.radius = (t * (in2.radius - in1.radius) / 128 + in1.radius);
  out.radius = round(out.radius * transform.xAxis.length());
  firstLine = out.center.y - out.radius;
  lastLine = out.center.y + out.radius;
}

void Display::transformCircle(const Circle &in, const Transform &transform, const Point &origin, InternalCircle &out, int16_t &firstLine, int16_t &lastLine)
{
  transformPoint(in.center, transform, origin, out.center, firstLine, lastLine);
  out.radius = round(in.radius * transform.xAxis.length());
  firstLine = out.center.y - out.radius;
  lastLine = out.center.y + out.radius;
}

void Display::transformCircle(const Circle &in, const Transform &transform, InternalCircle &out, int16_t &firstLine, int16_t &lastLine)
{
  transformPoint(in.center, transform, out.center, firstLine, lastLine);
  out.radius = round(in.radius * transform.xAxis.length());
  firstLine = out.center.y - out.radius;
  lastLine = out.center.y + out.radius;
}

void Display::interpolateLine(const Line &in1, const Line &in2, const Transform &transform, InternalLine &out, int16_t t, int16_t &firstLine, int16_t &lastLine)
{
  interpolatePoint(in1.p1, in2.p1, transform, out.p, t, firstLine, lastLine);
  interpolatePoint(in1.p2, in2.p2, transform, out.end, t, firstLine, lastLine);
  if (out.p.y > out.end.y)
  {
    swapWT(int16_t,out.p.x,out.end.x);
    swapWT(int16_t,out.p.y,out.end.y);
  }
  out.d.x = std::abs(out.p.x - out.end.x);
  out.d.y = out.p.y - out.end.y;
  out.sx = (out.p.x < out.end.x) ? 1:-1;
  out.err = 2 * (out.d.x + out.d.y);
  firstLine = out.p.y;
  lastLine = out.end.y;
}

void Display::transformLine(const Line &in, const Transform &transform, const Point &origin, InternalLine &out, int16_t &firstLine, int16_t &lastLine)
{
  transformPoint(in.p1, transform, origin, out.p, firstLine, lastLine);
  transformPoint(in.p2, transform, origin, out.end, firstLine, lastLine);
  if (out.p.y > out.end.y)
  {
    swapWT(int16_t,out.p.x,out.end.x);
    swapWT(int16_t,out.p.y,out.end.y);
  }
  out.d.x = std::abs(out.p.x - out.end.x);
  out.d.y = out.p.y - out.end.y;
  out.sx = (out.p.x < out.end.x) ? 1:-1;
  out.err = 2 * (out.d.x + out.d.y);
  firstLine = out.p.y;
  lastLine = out.end.y;
}

void Display::transformLine(const Line &in, const Transform &transform, InternalLine &out, int16_t &firstLine, int16_t &lastLine)
{
  transformPoint(in.p1, transform, out.p, firstLine, lastLine);
  transformPoint(in.p2, transform, out.end, firstLine, lastLine);
  if (out.p.y > out.end.y)
  {
    swapWT(int16_t,out.p.x,out.end.x);
    swapWT(int16_t,out.p.y,out.end.y);
  }
  out.d.x = std::abs(out.p.x - out.end.x);
  out.d.y = out.p.y - out.end.y;
  out.sx = (out.p.x < out.end.x) ? 1:-1;
  out.err = 2 * (out.d.x + out.d.y);
  firstLine = out.p.y;
  lastLine = out.end.y;
}

void Display::interpolatePoint(const Point &in1, const Point &in2, const Transform &transform, InternalPoint &out, int16_t t, int16_t &firstLine, int16_t &lastLine)
{
  if (t == 0)
    return transformPoint(in1, transform, out, firstLine, lastLine);
  else if (t == 128)
    return transformPoint(in2, transform, out, firstLine, lastLine);
  Point point = {t * (in2.x - in1.x) / 128 + in1.x, t * (in2.y - in1.y) / 128 + in1.y};
  transformPoint(point, transform, out, firstLine, lastLine);
}

void Display::transformPoint(const Point &in, const Transform &transform, const Point &origin, InternalPoint &out, int16_t &firstLine, int16_t &lastLine)
{
  out.x = (int16_t)round((in.x - origin.x) * transform.xAxis.x + (in.y - origin.y) * transform.yAxis.x + transform.offset.x) + origin.x;
  out.y = (int16_t)round((in.x - origin.x) * transform.xAxis.y + (in.y - origin.y) * transform.yAxis.y + transform.offset.y) + origin.y;
  firstLine = out.y;
  lastLine = out.y;
}

void Display::transformPoint(const Point &in, const Transform &transform, InternalPoint &out, int16_t &firstLine, int16_t &lastLine)
{
  out.x = (int16_t)round(in.x * transform.xAxis.x + in.y * transform.yAxis.x + transform.offset.x);
  out.y = (int16_t)round(in.x * transform.xAxis.y + in.y * transform.yAxis.y + transform.offset.y);
  firstLine = out.y;
  lastLine = out.y;
}
