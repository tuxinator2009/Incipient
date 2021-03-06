#ifndef TYPES_H
#define TYPES_H

#include <Pokitto.h>
#include "FixedPoints/FixedPoints.h"

#define USE_FLOAT

#ifdef USE_FLOAT
using fixed_t = float;
#else
using fixed_t = SFixed<15, 16>;
#endif

const fixed_t sinTable[] =
{
  0.000000, 0.024637, 0.049260, 0.073853, 0.098400, 0.122888, 0.147302, 0.171626, 0.195845, 0.219946, 0.243914, 0.267733, 0.291390, 0.314870, 0.338158, 0.361242,
  0.384106, 0.406737, 0.429121, 0.451244, 0.473094, 0.494656, 0.515918, 0.536867, 0.557489, 0.577774, 0.597707, 0.617278, 0.636474, 0.655284, 0.673696, 0.691698,
  0.709281, 0.726434, 0.743145, 0.759405, 0.775204, 0.790532, 0.805381, 0.819740, 0.833602, 0.846958, 0.859800, 0.872120, 0.883910, 0.895163, 0.905873, 0.916034,
  0.925638, 0.934680, 0.943154, 0.951057, 0.958381, 0.965124, 0.971281, 0.976848, 0.981823, 0.986201, 0.989980, 0.993159, 0.995734, 0.997705, 0.999070, 0.999829,
  0.999981, 0.999526, 0.998464, 0.996795, 0.994522, 0.991645, 0.988165, 0.984086, 0.979410, 0.974139, 0.968276, 0.961826, 0.954791, 0.947177, 0.938988, 0.930229,
  0.920906, 0.911023, 0.900587, 0.889604, 0.878081, 0.866025, 0.853444, 0.840344, 0.826734, 0.812622, 0.798017, 0.782928, 0.767363, 0.751332, 0.734845, 0.717912,
  0.700543, 0.682749, 0.664540, 0.645928, 0.626924, 0.607539, 0.587785, 0.567675, 0.547220, 0.526432, 0.505325, 0.483911, 0.462204, 0.440216, 0.417960, 0.395451,
  0.372702, 0.349727, 0.326539, 0.303153, 0.279583, 0.255843, 0.231948, 0.207912, 0.183750, 0.159476, 0.135105, 0.110653, 0.086133, 0.061561, 0.036951, 0.012320,
  -0.012320,-0.036951,-0.061561,-0.086133,-0.110653,-0.135105,-0.159476,-0.183750,-0.207912,-0.231948,-0.255843,-0.279583,-0.303153,-0.326539,-0.349727,-0.372702,
  -0.395451,-0.417960,-0.440216,-0.462204,-0.483911,-0.505325,-0.526432,-0.547220,-0.567675,-0.587785,-0.607539,-0.626924,-0.645928,-0.664540,-0.682749,-0.700543,
  -0.717912,-0.734845,-0.751332,-0.767363,-0.782928,-0.798017,-0.812622,-0.826734,-0.840344,-0.853444,-0.866025,-0.878081,-0.889604,-0.900587,-0.911023,-0.920906,
  -0.930229,-0.938988,-0.947177,-0.954791,-0.961826,-0.968276,-0.974139,-0.979410,-0.984086,-0.988165,-0.991645,-0.994522,-0.996795,-0.998464,-0.999526,-0.999981,
  -0.999829,-0.999070,-0.997705,-0.995734,-0.993159,-0.989980,-0.986201,-0.981823,-0.976848,-0.971281,-0.965124,-0.958381,-0.951057,-0.943154,-0.934680,-0.925638,
  -0.916034,-0.905873,-0.895163,-0.883910,-0.872120,-0.859800,-0.846958,-0.833602,-0.819740,-0.805381,-0.790532,-0.775204,-0.759405,-0.743145,-0.726434,-0.709281,
  -0.691698,-0.673696,-0.655284,-0.636474,-0.617278,-0.597707,-0.577774,-0.557489,-0.536867,-0.515918,-0.494656,-0.473094,-0.451244,-0.429121,-0.406737,-0.384106,
  -0.361242,-0.338158,-0.314870,-0.291390,-0.267733,-0.243914,-0.219946,-0.195845,-0.171626,-0.147302,-0.122888,-0.098400,-0.073853,-0.049260,-0.024637,-0.000000
};

const fixed_t cosTable[] =
{
  1.000000, 0.999696, 0.998786, 0.997269, 0.995147, 0.992421, 0.989092, 0.985162, 0.980635, 0.975512, 0.969797, 0.963493, 0.956604, 0.949135, 0.941089, 0.932472,
  0.923289, 0.913545, 0.903247, 0.892401, 0.881012, 0.869089, 0.856638, 0.843667, 0.830184, 0.816197, 0.801714, 0.786745, 0.771298, 0.755383, 0.739009, 0.722186,
  0.704926, 0.687237, 0.669131, 0.650618, 0.631711, 0.612420, 0.592758, 0.572735, 0.552365, 0.531659, 0.510631, 0.489293, 0.467658, 0.445738, 0.423549, 0.401102,
  0.378411, 0.355491, 0.332355, 0.309017, 0.285492, 0.261793, 0.237935, 0.213933, 0.189801, 0.165554, 0.141206, 0.116773, 0.092268, 0.067708, 0.043107, 0.018479,
  -0.006160,-0.030795,-0.055411,-0.079994,-0.104528,-0.128999,-0.153392,-0.177691,-0.201882,-0.225951,-0.249883,-0.273663,-0.297277,-0.320710,-0.343949,-0.366979,
  -0.389786,-0.412356,-0.434676,-0.456733,-0.478512,-0.500000,-0.521185,-0.542053,-0.562593,-0.582791,-0.602635,-0.622113,-0.641213,-0.659925,-0.678235,-0.696134,
  -0.713610,-0.730653,-0.747253,-0.763398,-0.779081,-0.794290,-0.809017,-0.823253,-0.836989,-0.850217,-0.862929,-0.875117,-0.886774,-0.897892,-0.908465,-0.918487,
  -0.927951,-0.936852,-0.945184,-0.952942,-0.960122,-0.966718,-0.972728,-0.978148,-0.982973,-0.987202,-0.990831,-0.993859,-0.996284,-0.998103,-0.999317,-0.999924,
  -0.999924,-0.999317,-0.998103,-0.996284,-0.993859,-0.990831,-0.987202,-0.982973,-0.978148,-0.972728,-0.966718,-0.960122,-0.952942,-0.945184,-0.936852,-0.927951,
  -0.918487,-0.908465,-0.897892,-0.886774,-0.875117,-0.862929,-0.850217,-0.836989,-0.823253,-0.809017,-0.794290,-0.779081,-0.763398,-0.747253,-0.730653,-0.713610,
  -0.696134,-0.678235,-0.659925,-0.641213,-0.622113,-0.602635,-0.582791,-0.562593,-0.542053,-0.521185,-0.500000,-0.478512,-0.456733,-0.434676,-0.412356,-0.389786,
  -0.366979,-0.343949,-0.320710,-0.297277,-0.273663,-0.249883,-0.225951,-0.201882,-0.177691,-0.153392,-0.128999,-0.104528,-0.079994,-0.055411,-0.030795,-0.006160,
  0.018479, 0.043107, 0.067708, 0.092268, 0.116773, 0.141206, 0.165554, 0.189801, 0.213933, 0.237935, 0.261793, 0.285492, 0.309017, 0.332355, 0.355491, 0.378411,
  0.401102, 0.423549, 0.445738, 0.467658, 0.489293, 0.510631, 0.531659, 0.552365, 0.572735, 0.592758, 0.612420, 0.631711, 0.650618, 0.669131, 0.687237, 0.704926,
  0.722186, 0.739009, 0.755383, 0.771298, 0.786745, 0.801714, 0.816197, 0.830184, 0.843667, 0.856638, 0.869089, 0.881012, 0.892401, 0.903247, 0.913545, 0.923289,
  0.932472, 0.941089, 0.949135, 0.956604, 0.963493, 0.969797, 0.975512, 0.980635, 0.985162, 0.989092, 0.992421, 0.995147, 0.997269, 0.998786, 0.999696, 1.000000
};

#ifdef USE_FLOAT
static inline float fastInverseSquareRoot(float value)
{
  const float x2 = value * 0.5F;
  const float threehalfs = 1.5F;
  
  union {
    float f;
    uint32_t i;
  } conv  = { .f = value };
  conv.i  = 0x5f3759df - ( conv.i >> 1 );
  conv.f  *= threehalfs - ( x2 * conv.f * conv.f );
  return conv.f;
}
#else
static inline constexpr SFixed<15, 16> sqrt( SFixed<15, 16> x ){
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
  x = SFixed<15, 16>::fromInternal(q);
  return x;
}
#endif

struct Point
{
  int16_t x;
  int16_t y;
};

struct VectorF
{
  fixed_t x;
  fixed_t y;
  constexpr VectorF() : x(0.0), y(0.0) {}
  constexpr VectorF(const fixed_t &fx, const fixed_t &fy) : x(fx), y(fy) {}
  constexpr VectorF(const Point &point) : x(point.x), y(point.y) {}
  VectorF &rotate(const VectorF &origin, uint8_t amount)
  {
    fixed_t ox = x - origin.x;
    fixed_t s = sinTable[amount];
    fixed_t c = cosTable[amount];
    x = ox * c - y * s + origin.x;
    y = ox * s + y * c + origin.y;
    return *this;
  }
  fixed_t length() const
  {
    return sqrt(x * x + y * y);
  }
  VectorF normalize()
  {
#ifdef USE_FLOAT
    fixed_t invsqrt = fastInverseSquareRoot(x * x + y * y);
#else
    fixed_t invsqrt = fixed_t(1.0) / length();
#endif
    x *= invsqrt;
    y *= invsqrt;
    return *this;
  }
  VectorF normalized() const
  {
    VectorF vector = *this;
    vector.normalize();
    return vector;
  }
  VectorF lerp(const VectorF &other, const fixed_t &amount)
  {
    x += amount * (other.x - x);
    y += amount * (other.y - y);
    return *this;
  }
  VectorF slerp(const VectorF &other, const fixed_t &amount, const fixed_t &scale)
  {
    lerp(other, amount);
    normalize();
#ifdef USE_FLOAT
    fixed_t invsqrt = scale / fastInverseSquareRoot(other.x * other.x + other.y * other.y);
    x *= invsqrt;
    y *= invsqrt;
#else
    fixed_t len = other.length() * scale;
    x *= len;
    y *= len;
#endif
    return *this;
  }
  fixed_t cross(const VectorF &other) const
  {
    return x * other.y - y * other.x;
  }
  fixed_t cross(const Point &point) const
  {
    return x * point.y - y * point.x;
  }
  fixed_t dot(const VectorF &other) const
  {
    return x * other.x + y * other.y;
  }
  fixed_t dot(const Point &point) const
  {
    return x * point.x + y * point.y;
  }
  VectorF projected(const VectorF &xAxis, const VectorF &yAxis) const
  {
    return VectorF(x * xAxis.x + y * yAxis.x, x * xAxis.y + y * yAxis.y);
  }
  VectorF clipped(const VectorF &min, const VectorF &max) const
  {
    VectorF v(x, y);
    if (v.x < min.x)
      v.x = min.x;
    else if (v.x > max.x)
      v.x = max.x;
    if (v.y < min.y)
      v.y = min.y;
    else if (v.y > max.y)
      v.y = max.y;
    return v;
  }
  VectorF &operator=(const VectorF &other)
  {
    x = other.x;
    y = other.y;
    return *this;
  }
  VectorF &operator=(const Point &point)
  {
    x = point.x;
    y = point.y;
    return *this;
  }
  VectorF &operator+=(const VectorF &other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }
  VectorF &operator+=(const Point &point)
  {
    x += point.x;
    y += point.y;
    return *this;
  }
  VectorF &operator-=(const VectorF &other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  VectorF &operator-=(const Point &point)
  {
    x -= point.x;
    y -= point.y;
    return *this;
  }
  VectorF &operator*=(const VectorF &other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }
  VectorF &operator*=(const Point &point)
  {
    x *= point.x;
    y *= point.y;
    return *this;
  }
  VectorF &operator*=(const fixed_t &scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }
  VectorF &operator*=(const int16_t &scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }
  VectorF &operator/=(const VectorF &other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }
  VectorF &operator/=(const Point &point)
  {
    x /= point.x;
    y /= point.y;
    return *this;
  }
  VectorF &operator/=(const fixed_t &scale)
  {
    x /= scale;
    y /= scale;
    return *this;
  }
  VectorF &operator/=(const int16_t &scale)
  {
    x /= scale;
    y /= scale;
    return *this;
  }
  VectorF operator+(const VectorF &other) const
  {
    VectorF v(x, y);
    v += other;
    return v;
  }
  VectorF operator+(const Point &point) const
  {
    VectorF v(x, y);
    v += point;
    return v;
  }
  VectorF operator-(const VectorF &other) const
  {
    VectorF v(x, y);
    v -= other;
    return v;
  }
  VectorF operator-(const Point &point) const
  {
    VectorF v(x, y);
    v -= point;
    return v;
  }
  VectorF operator*(const VectorF &other) const
  {
    VectorF v(x, y);
    v *= other;
    return v;
  }
  VectorF operator*(const Point &point) const
  {
    VectorF v(x, y);
    v *= point;
    return v;
  }
  VectorF operator*(const fixed_t &scale) const
  {
    VectorF v(x, y);
    v *= scale;
    return v;
  }
  VectorF operator*(const int16_t &scale) const
  {
    VectorF v(x, y);
    v *= scale;
    return v;
  }
  VectorF operator/(const VectorF &other) const
  {
    VectorF v(x, y);
    v /= other;
    return v;
  }
  VectorF operator/(const Point &point) const
  {
    VectorF v(x, y);
    v /= point;
    return v;
  }
  VectorF operator/(const fixed_t &scale) const
  {
    VectorF v(x, y);
    v /= scale;
    return v;
  }
  VectorF operator/(const int16_t &scale) const
  {
    VectorF v(x, y);
    v /= scale;
    return v;
  }
  constexpr VectorF operator-() const
  {
    VectorF v(-x, -y);
    return v;
  }
  bool operator==(const VectorF &other) const
  {
    if (x != other.x)
      return false;
    if (y != other.y)
      return false;
    return true;
  }
  bool operator==(const Point &point) const
  {
    if ((int16_t)x != point.x)
      return false;
    if ((int16_t)y != point.y)
      return false;
    return true;
  }
  bool operator!=(const VectorF &other) const
  {
    if (x != other.x)
      return true;
    if (y != other.y)
      return true;
    return false;
  }
  operator Point() const
  {
    Point point;
    point.x = (int16_t)x;
    point.y = (int16_t)y;
    return point;
  }
};

struct Circle
{
  Point center;
  int16_t radius;
};

struct Line
{
  Point p1;
  Point p2;
};

struct Triangle
{
  Point p1;
  Point p2;
  Point p3;
};

struct Shape
{
  uint8_t type;
  union
  {
    Point point;
    Circle circle;
    Line line;
    Triangle triangle;
  };
  static constexpr uint8_t Point = 0;
  static constexpr uint8_t Line = 1;
  static constexpr uint8_t Circle = 2;
  static constexpr uint8_t Triangle = 3;
};

struct Particle
{
  VectorF offset;
  VectorF vector;
  fixed_t velocity;
  fixed_t distance;
  uint16_t color;
  uint8_t life;
};

struct Model
{
  uint8_t numShapes;
  Point link;
  Circle weakspot;
  const Shape *shapes;
};

struct Transform
{
  VectorF offset;
  VectorF xAxis;
  VectorF yAxis;
};

struct BodyPart
{
  static constexpr uint8_t State_Normal = 0;
  static constexpr uint8_t State_Digesting = 64;
  static constexpr uint8_t State_Breaking = 128;
  static constexpr uint8_t State_Growing = 192;
  static constexpr uint8_t State_Counter = 63;
  static constexpr uint8_t State_Flag = 192;
  Transform transform;
  const Model *model;
  BodyPart *previous;
  BodyPart *next;
  Circle weakspot;
  fixed_t scale;
  uint16_t colors[4];
  uint8_t alphas[4];
  uint8_t state;
};

struct Creature
{
  static constexpr uint8_t State_Stationary = 0;
  static constexpr uint8_t State_Wandering = 1;
  static constexpr uint8_t State_Fleeing = 2;
  static constexpr uint8_t State_Chasing = 3;
  const Model *mouthClosed;
  BodyPart *head;
  BodyPart *tail;
  fixed_t speed;
  fixed_t slerpAmount;
  uint8_t target;
  int8_t tDir;
  uint8_t t;
  uint8_t state;
};

struct Food
{
  const Model *model;
  uint16_t colors[4];
};

struct Score
{
  char name[4];
  uint32_t score;
};

struct Stage
{
  uint32_t nextStage;
  uint8_t maxEnemies;
  uint8_t minSize;
  uint8_t maxSize;
};

#endif //TYPES_H
