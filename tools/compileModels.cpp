#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/error/error.h"

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

struct Model
{
  char name[32];
  std::vector<Triangle*> triangles;
  std::vector<Circle*> circles;
  int16_t link[2];
};

using namespace rapidjson;

const char *modelsPath = "../models.json";
const char *headerPath = "../models.h";

bool isCCW(Triangle *triangle)
{
  int16_t cx = (triangle->p1[0] + triangle->p2[0] + triangle->p3[0]) / 3;
  int16_t cy = (triangle->p1[1] + triangle->p2[1] + triangle->p3[1]) / 3;
  if (((triangle->p2[1] - triangle->p1[1])*(cx - triangle->p1[0]) - (triangle->p2[0] - triangle->p1[0])*(cy - triangle->p1[1])) < 0)
    return true;
  if (((triangle->p3[1] - triangle->p2[1])*(cx - triangle->p2[0]) - (triangle->p3[0] - triangle->p2[0])*(cy - triangle->p2[1])) < 0)
    return true;
  if (((triangle->p1[1] - triangle->p3[1])*(cx - triangle->p3[0]) - (triangle->p1[0] - triangle->p3[0])*(cy - triangle->p3[1])) < 0)
    return true;
  return false;
}

void flipNormal(Triangle *triangle)
{
  int16_t temp = triangle->p2[0];
  triangle->p2[0] = triangle->p3[0];
  triangle->p3[0] = temp;
  temp = triangle->p2[1];
  triangle->p2[1] = triangle->p3[1];
  triangle->p3[1] = temp;
}

int main(int argc, char *argv[])
{
  std::vector<Model*> objects;
  FILE *f = fopen(modelsPath, "r");
  size_t numTriangles = 0;
  size_t numCircles = 0;
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
    printf("Reason: %s (%lu)\n", GetParseError_En(ok.Code()), ok.Offset());
    return -1;
  }
  for (auto& model : doc.GetArray())
  {
    Model *object = new Model;
    auto &link = model["link"];
    auto &triangles = model["triangles"];
    auto &circles = model["circles"];
    strcpy(object->name, model["name"].GetString());
    object->link[0] = link[0].GetInt();
    object->link[1] = link[1].GetInt();
    for (auto& triangle : triangles.GetArray())
    {
      Triangle *tri = new Triangle;
      tri->p1[0] = triangle[0].GetInt();
      tri->p1[1] = triangle[1].GetInt();
      tri->p2[0] = triangle[2].GetInt();
      tri->p2[1] = triangle[3].GetInt();
      tri->p3[0] = triangle[4].GetInt();
      tri->p3[1] = triangle[5].GetInt();
      if (isCCW(tri))
        flipNormal(tri);
      object->triangles.push_back(tri);
    }
    for (auto& circle : circles.GetArray())
    {
      Circle *c = new Circle;
      c->center[0] = circle[0].GetInt();
      c->center[1] = circle[1].GetInt();
      c->radius = circle[2].GetInt();
      object->circles.push_back(c);
    }
    objects.push_back(object);
  }
  f = fopen(headerPath, "w");
  fprintf(f, "#ifndef MODELS_H\n");
  fprintf(f, "#define MODELS_H\n\n");
  fprintf(f, "#include \"Display.h\"\n\n");
  fprintf(f, "namespace Models\n{\n");
  fprintf(f, "  const Display::Triangle triangles[] =\n");
  fprintf(f, "  {\n");
  //Output triangle array
  for (size_t i = 0; i < objects.size(); ++i)
  {
    for (size_t j = 0; j < objects[i]->triangles.size(); ++j)
    {
      Triangle *tri = objects[i]->triangles[j];
      fprintf(f, "    {%2d, %2d, %2d, %2d, %2d, %2d},", tri->p1[0], tri->p1[1], tri->p2[0], tri->p2[1], tri->p3[0], tri->p3[1]);
      if (j == 0)
        fprintf(f, " //%s", objects[i]->name);
      fprintf(f, "\n");
    }
  }
  fprintf(f, "  };\n");
  fprintf(f, "  const Display::Circle circles[] =\n");
  fprintf(f, "  {\n");
  //Output circle array
  for (size_t i = 0; i < objects.size(); ++i)
  {
    for (size_t j = 0; j < objects[i]->circles.size(); ++j)
    {
      Circle *c = objects[i]->circles[j];
      fprintf(f, "    {%2d, %2d, %2d},", c->center[0], c->center[1], c->radius);
      if (j == 0)
        fprintf(f, " //%s", objects[i]->name);
      fprintf(f, "\n");
    }
  }
  fprintf(f, "  };\n");
  //Output model variables
  for (size_t i = 0; i < objects.size(); ++i)
  {
    fprintf(f, "  const Display::Model %s = {%lu, %lu, %d, %d, triangles + %lu, circles + %lu};\n", objects[i]->name, objects[i]->triangles.size(), objects[i]->circles.size(), objects[i]->link[0], objects[i]->link[1], numTriangles, numCircles);
    numTriangles += objects[i]->triangles.size();
    numCircles += objects[i]->circles.size();
  }
  //Output models array
  fprintf(f, "  const Display::Model *models[] =\n");
  fprintf(f, "  {\n");
  for (size_t i = 0; i < objects.size(); ++i)
    fprintf(f, "    &%s,\n", objects[i]->name);
  fprintf(f, "  };\n");
  fprintf(f, "};\n\n");
  fprintf(f, "\n#endif //MODELS_H\n");
  fclose(f);
  for (size_t i = 0; i < objects.size(); ++i)
  {
    for (size_t j = 0; j < objects[i]->triangles.size(); ++j)
      delete objects[i]->triangles[j];
    for (size_t j = 0; j < objects[i]->circles.size(); ++j)
      delete objects[i]->circles[j];
    delete objects[i];
  }
  objects.clear();
}
