#include <cstdio>
#include <cstring>
#include <cmath>
#include <cinttypes>

#define HALF_MAX_VALUE 8
#define PI 3.1415926535

int mapValue(int value, int oldMin, int oldMax, int newMin, int newMax)
{
  return (value - oldMin) * (newMax - newMin) / (oldMax - oldMin) + newMin;
}

uint8_t mapValues[4*3*4] =
{
  16, 24, 32, 168, 204, 248,
  16, 24, 32, 126, 153, 186,
  
  16, 20, 32, 184, 168, 248,
  16, 20, 32, 138, 126, 186,
  
  24, 20, 32, 232, 168, 248,
  24, 20, 32, 174, 126, 186,
  
  32, 20, 24, 248, 168, 216,
  32, 20, 24, 186, 126, 174
};

uint8_t paletteRGB[16*3*4] = 
{
  //color1
  16, 24, 32,
  40, 48, 64,
  64, 76, 96,
  80, 100, 128,
  104, 128, 160,
  128, 152, 192,
  144, 176, 224,
  168, 204, 248,
  168, 204, 248,
  144, 176, 224,
  128, 152, 192,
  104, 128, 160,
  80, 100, 128,
  64, 76, 96,
  40, 48, 64,
  16, 24, 32,
  //color2
  16, 20, 32,
  40, 40, 64,
  64, 64, 96,
  88, 84, 128,
  112, 104, 160,
  136, 128, 192,
  160, 148, 224,
  184, 168, 248,
  184, 168, 248,
  160, 148, 224,
  136, 128, 192,
  112, 104, 160,
  88, 84, 128,
  64, 64, 96,
  40, 40, 64,
  16, 20, 32,
  //color3
  24, 20, 32,
  56, 40, 64,
  88, 64, 96,
  112, 84, 128,
  144, 104, 160,
  176, 128, 192,
  208, 148, 224,
  232, 168, 248,
  232, 168, 248,
  208, 148, 224,
  176, 128, 192,
  144, 104, 160,
  112, 84, 128,
  88, 64, 96,
  56, 40, 64,
  24, 20, 32,
  //color4
  32, 20, 24,
  64, 40, 48,
  96, 64, 80,
  128, 84, 104,
  160, 104, 136,
  192, 128, 160,
  224, 148, 192,
  248, 168, 216,
  248, 168, 216,
  224, 148, 192,
  192, 128, 160,
  160, 104, 136,
  128, 84, 104,
  96, 64, 80,
  64, 40, 48,
  32, 20, 24,
};

uint16_t RGBto565(uint8_t R,uint8_t G,uint8_t B)
{
  uint16_t color;
  color = B>>3;
  color |= ((G >> 2) << 5);
  color |= ((R >> 3) << 11);
  return color;
}

uint8_t mapSin(float value, uint8_t min, uint8_t max)
{
  return (uint8_t)((value + 1.0) * (max - min) / 2.0 + min);
}

int main(int argc, char *argv[])
{
  printf("#ifndef BACKGROUND_H\n");
  printf("#define BACKGROUND_H\n\n");
  /*printf("uint16_t bgPalette_original[16] = {");
  for (int i = 0; i < 16; ++i)
  {
    printf("0x%04X", RGBto565(paletteRGB[i*3]/6, paletteRGB[i*3+1]/6, paletteRGB[i*3+2]/6));
    if (i != 15)
      printf(",");
  }
  printf("};\n");*/
  printf("uint16_t bgPalettes[16*4] =\n");
  printf("{\n");
  for (int i = 0; i < 4; ++i)
  {
    printf("  ");
    for (int j = 0; j < 16; ++j)
    {
      uint8_t r = mapValue(paletteRGB[i*48+j*3], mapValues[i*12], mapValues[i*12+3], mapValues[i*12+6], mapValues[i*12+9]);
      uint8_t g = mapValue(paletteRGB[i*48+j*3+1], mapValues[i*12+1], mapValues[i*12+4], mapValues[i*12+7], mapValues[i*12+10]);
      uint8_t b = mapValue(paletteRGB[i*48+j*3+2], mapValues[i*12+2], mapValues[i*12+5], mapValues[i*12+8], mapValues[i*12+11]);
      printf("0x%04X", RGBto565(r/6, g/6, b/6));
      if (i != 3 || j != 15)
        printf(",");
    }
    printf("\n");
  }
  printf("};\n");
  printf("uint32_t bgPalette[16*16] =\n");
  printf("{\n");
  for (int i = 0; i < 16; ++i)
  {
    printf("  ");
    for (int j = 0; j < 16; ++j)
    {
      printf("0x");
      printf("%04X", RGBto565(paletteRGB[i*3]/6, paletteRGB[i*3+1]/6, paletteRGB[i*3+2]/6));
      printf("%04X", RGBto565(paletteRGB[j*3]/6, paletteRGB[j*3+1]/6, paletteRGB[j*3+2]/6));
      if (i != 15 || j != 15)
        printf(",");
    }
    printf("\n");
    //printf("0x%04X", RGBto565(paletteRGB[i*3]/6, paletteRGB[i*3+1]/6, paletteRGB[i*3+2]/6));
  }
  printf("};\n");
  printf("const uint8_t bgImage[] =\n");
  printf("{\n");
  for(int y = 0; y < 176; y++)
  {
    for(int x = 0; x < 220; x++)
    {
      //the plasma buffer is a sum of sines
      int color = int
      (
        HALF_MAX_VALUE + (HALF_MAX_VALUE * sin(x / 32.0))
        + HALF_MAX_VALUE + (HALF_MAX_VALUE * sin(y / 16.0))
        + HALF_MAX_VALUE + (HALF_MAX_VALUE * sin((x + y) / 24.0))
        + HALF_MAX_VALUE + (HALF_MAX_VALUE * sin(sqrt(double(x * x + y * y)) / 12.0))
      ) / 4;
      if (x == 0)
        printf("  ");
      if (x % 2 == 0)
        printf("0x%X", color);
      if (x % 2 == 1)
        printf("%X,", color);
    }
    printf("\n");
  }
  printf("};\n\n");
  printf("#endif //BACKGROUND_H\n");
  return 0;
};
