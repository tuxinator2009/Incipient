#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/error/error.h"

struct Point
{
  int16_t x;
  int16_t y;
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
  std::vector<int16_t> values;
};

struct Model
{
  char name[32];
  std::vector<Shape> shapes;
  Point link;
  Circle weakspot;
};

using namespace rapidjson;

const char *modelsPath = "../models.json";
const char *headerPath = "../models.h";

int getLine(const char *file, size_t offset)
{
  FILE *f = fopen(file, "r");
  int line = 1;
  while (offset > 0)
  {
    if (fgetc(f) == '\n')
      ++line;
    --offset;
  }
  fclose(f);
  return line;
}

int main(int argc, char *argv[])
{
  std::vector<Model*> objects;
  FILE *f = fopen(modelsPath, "r");
  size_t numShapes = 0;
  char buffer[65536];
  FileReadStream is(f, buffer, sizeof(buffer));
  Document doc;
  if (!f)
  {
    printf("Error: failed to open \"%s\"\n", modelsPath);
    perror("Reason");
    return -1;
  }
  ParseResult ok = doc.ParseStream(is);
  fclose(f);
  if (!ok)
  {
    printf("Error: failed to parse json \"%s\"\n", modelsPath);
    printf("Reason: %s (%d)\n", GetParseError_En(ok.Code()), getLine(modelsPath, ok.Offset()));
    return -1;
  }
  for (auto& model : doc.GetArray())
  {
    Model *object = new Model;
    auto &link = model["link"];
    auto &weakspot = model["weakspot"];
    auto &shapes = model["shapes"];
    strcpy(object->name, model["name"].GetString());
    object->link.x = link[0].GetInt();
    object->link.y = link[1].GetInt();
    object->weakspot.center.x = weakspot[0].GetInt();
    object->weakspot.center.y = weakspot[1].GetInt();
    object->weakspot.radius = weakspot[2].GetInt();
    for (auto& shape : shapes.GetArray())
    {
      Shape s;
      for (auto& value : shape.GetArray())
        s.values.push_back(value.GetInt());
      object->shapes.push_back(s);
    }
    objects.push_back(object);
  }
  f = fopen(headerPath, "w");
  fprintf(f, "#ifndef MODELS_H\n");
  fprintf(f, "#define MODELS_H\n\n");
  fprintf(f, "#include \"Types.h\"\n\n");
  fprintf(f, "namespace Models\n{\n");
  //Output shape array
  fprintf(f, "  static const Shape shapes[] =\n");
  fprintf(f, "  {\n");
  for (size_t i = 0; i < objects.size(); ++i)
  {
    for (size_t j = 0; j < objects[i]->shapes.size(); ++j)
    {
      Shape *shape = &objects[i]->shapes[j];
      if (shape->values.size() == 2)
        fprintf(f, "    {Shape::Point,    .point=   {%2d, %2d}},                ", shape->values[0], shape->values[1]);
      else if (shape->values.size() == 3)
        fprintf(f, "    {Shape::Circle,   .circle=  {%2d, %2d, %2d}},            ", shape->values[0], shape->values[1], shape->values[2]);
      else if (shape->values.size() == 4)
        fprintf(f, "    {Shape::Line,     .line=    {%2d, %2d, %2d, %2d}},        ", shape->values[0], shape->values[1], shape->values[2], shape->values[3]);
      else if (shape->values.size() == 6)
        fprintf(f, "    {Shape::Triangle, .triangle={%2d, %2d, %2d, %2d, %2d, %2d}},", shape->values[0], shape->values[1], shape->values[2], shape->values[3], shape->values[4], shape->values[5]);
      if (j == 0)
        fprintf(f, " //%s", objects[i]->name);
      fprintf(f, "\n");
    }
  }
  fprintf(f, "  };\n");
  //Output model variables
  for (size_t i = 0; i < objects.size(); ++i)
  {
    fprintf(f, "  static const Model %s = {%lu, {%d, %d}, {%d, %d, %d}, shapes + %lu};\n", objects[i]->name, objects[i]->shapes.size(), objects[i]->link.x, objects[i]->link.y, objects[i]->weakspot.center.x, objects[i]->weakspot.center.y, objects[i]->weakspot.radius, numShapes);
    numShapes += objects[i]->shapes.size();
  }
  //Output model array
  fprintf(f, "  static const Model *models[] =\n");
  fprintf(f, "  {\n");
  for (size_t i = 0; i < objects.size(); ++i)
    fprintf(f, "    &%s,\n", objects[i]->name);
  fprintf(f, "  };\n");
  fprintf(f, "};\n\n");
  fprintf(f, "\n#endif //MODELS_H\n");
  fclose(f);
  for (size_t i = 0; i < objects.size(); ++i)
    delete objects[i];
  objects.clear();
}
