#include <cstdint>
#include <Pokitto.h>
#include "FixedPoints/FixedPoints.h"
#include "Display.h"
#include "background.h"

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

Display::InternalTriangle Display::triangles[128]{0};
Display::InternalCircle Display::circles[32]{0};
Display::InternalParticle Display::particles[32]{0};
uint32_t Display::lastPaletteRotation = 0;
uint8_t Display::maxTrianglesPerLine[176]{0};
uint8_t Display::maxCirclesPerLine[176]{0};
uint8_t Display::numTriangles = 0;
uint8_t Display::numCircles = 0;
uint8_t Display::numParticles = 0;
uint8_t Display::tone = 0;
uint8_t Display::pacing = 0;
const uint8_t Display::maxTriangles = 128;
const uint8_t Display::maxCircles = 32;
const uint8_t Display::maxParticles = 32;
const uint32_t Display::plasmaDelay[4] = {100, 75, 50, 25};

const SQ15x16 Display::sinTable[] =
{
  /*(fix16_t)0x00000000,(fix16_t)0x0000064F,(fix16_t)0x00000C9C,(fix16_t)0x000012E8,(fix16_t)0x00001931,(fix16_t)0x00001F76,(fix16_t)0x000025B6,(fix16_t)0x00002BF0,(fix16_t)0x00003223,(fix16_t)0x0000384E,(fix16_t)0x00003E71,(fix16_t)0x0000448A,(fix16_t)0x00004A99,(fix16_t)0x0000509B,(fix16_t)0x00005692,(fix16_t)0x00005C7A,
  (fix16_t)0x00006255,(fix16_t)0x00006820,(fix16_t)0x00006DDB,(fix16_t)0x00007385,(fix16_t)0x0000791D,(fix16_t)0x00007EA2,(fix16_t)0x00008413,(fix16_t)0x00008970,(fix16_t)0x00008EB8,(fix16_t)0x000093E9,(fix16_t)0x00009903,(fix16_t)0x00009E06,(fix16_t)0x0000A2F0,(fix16_t)0x0000A7C1,(fix16_t)0x0000AC77,(fix16_t)0x0000B113,
  (fix16_t)0x0000B593,(fix16_t)0x0000B9F8,(fix16_t)0x0000BE3F,(fix16_t)0x0000C268,(fix16_t)0x0000C674,(fix16_t)0x0000CA60,(fix16_t)0x0000CE2D,(fix16_t)0x0000D1DA,(fix16_t)0x0000D567,(fix16_t)0x0000D8D2,(fix16_t)0x0000DC1C,(fix16_t)0x0000DF43,(fix16_t)0x0000E248,(fix16_t)0x0000E529,(fix16_t)0x0000E7E7,(fix16_t)0x0000EA81,
  (fix16_t)0x0000ECF7,(fix16_t)0x0000EF47,(fix16_t)0x0000F173,(fix16_t)0x0000F378,(fix16_t)0x0000F558,(fix16_t)0x0000F712,(fix16_t)0x0000F8A6,(fix16_t)0x0000FA13,(fix16_t)0x0000FB59,(fix16_t)0x0000FC78,(fix16_t)0x0000FD6F,(fix16_t)0x0000FE40,(fix16_t)0x0000FEE8,(fix16_t)0x0000FF6A,(fix16_t)0x0000FFC3,(fix16_t)0x0000FFF5,
  (fix16_t)0x0000FFFF,(fix16_t)0x0000FFE1,(fix16_t)0x0000FF9B,(fix16_t)0x0000FF2E,(fix16_t)0x0000FE99,(fix16_t)0x0000FDDC,(fix16_t)0x0000FCF8,(fix16_t)0x0000FBED,(fix16_t)0x0000FABB,(fix16_t)0x0000F961,(fix16_t)0x0000F7E1,(fix16_t)0x0000F63A,(fix16_t)0x0000F46D,(fix16_t)0x0000F27A,(fix16_t)0x0000F062,(fix16_t)0x0000EE23,
  (fix16_t)0x0000EBC0,(fix16_t)0x0000E939,(fix16_t)0x0000E68D,(fix16_t)0x0000E3BD,(fix16_t)0x0000E0CA,(fix16_t)0x0000DDB4,(fix16_t)0x0000DA7B,(fix16_t)0x0000D721,(fix16_t)0x0000D3A5,(fix16_t)0x0000D008,(fix16_t)0x0000CC4B,(fix16_t)0x0000C86E,(fix16_t)0x0000C472,(fix16_t)0x0000C057,(fix16_t)0x0000BC1F,(fix16_t)0x0000B7C9,
  (fix16_t)0x0000B357,(fix16_t)0x0000AEC9,(fix16_t)0x0000AA1F,(fix16_t)0x0000A55C,(fix16_t)0x0000A07E,(fix16_t)0x00009B88,(fix16_t)0x00009679,(fix16_t)0x00009153,(fix16_t)0x00008C17,(fix16_t)0x000086C4,(fix16_t)0x0000815D,(fix16_t)0x00007BE2,(fix16_t)0x00007653,(fix16_t)0x000070B2,(fix16_t)0x00006AFF,(fix16_t)0x0000653C,
  (fix16_t)0x00005F69,(fix16_t)0x00005988,(fix16_t)0x00005398,(fix16_t)0x00004D9B,(fix16_t)0x00004793,(fix16_t)0x0000417F,(fix16_t)0x00003B61,(fix16_t)0x0000353A,(fix16_t)0x00002F0A,(fix16_t)0x000028D3,(fix16_t)0x00002296,(fix16_t)0x00001C54,(fix16_t)0x0000160D,(fix16_t)0x00000FC2,(fix16_t)0x00000976,(fix16_t)0x00000327,
  (fix16_t)0xFFFFFCD9,(fix16_t)0xFFFFF68A,(fix16_t)0xFFFFF03E,(fix16_t)0xFFFFE9F3,(fix16_t)0xFFFFE3AC,(fix16_t)0xFFFFDD6A,(fix16_t)0xFFFFD72D,(fix16_t)0xFFFFD0F6,(fix16_t)0xFFFFCAC6,(fix16_t)0xFFFFC49F,(fix16_t)0xFFFFBE81,(fix16_t)0xFFFFB86D,(fix16_t)0xFFFFB265,(fix16_t)0xFFFFAC68,(fix16_t)0xFFFFA678,(fix16_t)0xFFFFA097,
  (fix16_t)0xFFFF9AC4,(fix16_t)0xFFFF9501,(fix16_t)0xFFFF8F4E,(fix16_t)0xFFFF89AD,(fix16_t)0xFFFF841E,(fix16_t)0xFFFF7EA3,(fix16_t)0xFFFF793C,(fix16_t)0xFFFF73E9,(fix16_t)0xFFFF6EAD,(fix16_t)0xFFFF6987,(fix16_t)0xFFFF6478,(fix16_t)0xFFFF5F82,(fix16_t)0xFFFF5AA4,(fix16_t)0xFFFF55E1,(fix16_t)0xFFFF5137,(fix16_t)0xFFFF4CA9,
  (fix16_t)0xFFFF4837,(fix16_t)0xFFFF43E1,(fix16_t)0xFFFF3FA9,(fix16_t)0xFFFF3B8E,(fix16_t)0xFFFF3792,(fix16_t)0xFFFF33B5,(fix16_t)0xFFFF2FF8,(fix16_t)0xFFFF2C5B,(fix16_t)0xFFFF28DF,(fix16_t)0xFFFF2585,(fix16_t)0xFFFF224C,(fix16_t)0xFFFF1F36,(fix16_t)0xFFFF1C43,(fix16_t)0xFFFF1973,(fix16_t)0xFFFF16C7,(fix16_t)0xFFFF1440,
  (fix16_t)0xFFFF11DD,(fix16_t)0xFFFF0F9E,(fix16_t)0xFFFF0D86,(fix16_t)0xFFFF0B93,(fix16_t)0xFFFF09C6,(fix16_t)0xFFFF081F,(fix16_t)0xFFFF069F,(fix16_t)0xFFFF0545,(fix16_t)0xFFFF0413,(fix16_t)0xFFFF0308,(fix16_t)0xFFFF0224,(fix16_t)0xFFFF0167,(fix16_t)0xFFFF00D2,(fix16_t)0xFFFF0065,(fix16_t)0xFFFF001F,(fix16_t)0xFFFF0001,
  (fix16_t)0xFFFF000B,(fix16_t)0xFFFF003D,(fix16_t)0xFFFF0096,(fix16_t)0xFFFF0118,(fix16_t)0xFFFF01C0,(fix16_t)0xFFFF0291,(fix16_t)0xFFFF0388,(fix16_t)0xFFFF04A7,(fix16_t)0xFFFF05ED,(fix16_t)0xFFFF075A,(fix16_t)0xFFFF08EE,(fix16_t)0xFFFF0AA8,(fix16_t)0xFFFF0C88,(fix16_t)0xFFFF0E8D,(fix16_t)0xFFFF10B9,(fix16_t)0xFFFF1309,
  (fix16_t)0xFFFF157F,(fix16_t)0xFFFF1819,(fix16_t)0xFFFF1AD7,(fix16_t)0xFFFF1DB8,(fix16_t)0xFFFF20BD,(fix16_t)0xFFFF23E4,(fix16_t)0xFFFF272E,(fix16_t)0xFFFF2A99,(fix16_t)0xFFFF2E26,(fix16_t)0xFFFF31D3,(fix16_t)0xFFFF35A0,(fix16_t)0xFFFF398C,(fix16_t)0xFFFF3D98,(fix16_t)0xFFFF41C1,(fix16_t)0xFFFF4608,(fix16_t)0xFFFF4A6D,
  (fix16_t)0xFFFF4EED,(fix16_t)0xFFFF5389,(fix16_t)0xFFFF583F,(fix16_t)0xFFFF5D10,(fix16_t)0xFFFF61FA,(fix16_t)0xFFFF66FD,(fix16_t)0xFFFF6C17,(fix16_t)0xFFFF7148,(fix16_t)0xFFFF7690,(fix16_t)0xFFFF7BED,(fix16_t)0xFFFF815E,(fix16_t)0xFFFF86E3,(fix16_t)0xFFFF8C7B,(fix16_t)0xFFFF9225,(fix16_t)0xFFFF97E0,(fix16_t)0xFFFF9DAB,
  (fix16_t)0xFFFFA386,(fix16_t)0xFFFFA96E,(fix16_t)0xFFFFAF65,(fix16_t)0xFFFFB567,(fix16_t)0xFFFFBB76,(fix16_t)0xFFFFC18F,(fix16_t)0xFFFFC7B2,(fix16_t)0xFFFFCDDD,(fix16_t)0xFFFFD410,(fix16_t)0xFFFFDA4A,(fix16_t)0xFFFFE08A,(fix16_t)0xFFFFE6CF,(fix16_t)0xFFFFED18,(fix16_t)0xFFFFF364,(fix16_t)0xFFFFF9B1,(fix16_t)0x00000000*/
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

const SQ15x16 Display::cosTable[] =
{
  /*(fix16_t)0x00010000,(fix16_t)0x0000FFEC,(fix16_t)0x0000FFB0,(fix16_t)0x0000FF4D,(fix16_t)0x0000FEC2,(fix16_t)0x0000FE0F,(fix16_t)0x0000FD35,(fix16_t)0x0000FC34,(fix16_t)0x0000FB0B,(fix16_t)0x0000F9BB,(fix16_t)0x0000F845,(fix16_t)0x0000F6A7,(fix16_t)0x0000F4E4,(fix16_t)0x0000F2FB,(fix16_t)0x0000F0EB,(fix16_t)0x0000EEB6,
  (fix16_t)0x0000EC5D,(fix16_t)0x0000E9DE,(fix16_t)0x0000E73B,(fix16_t)0x0000E474,(fix16_t)0x0000E18A,(fix16_t)0x0000DE7D,(fix16_t)0x0000DB4D,(fix16_t)0x0000D7FB,(fix16_t)0x0000D487,(fix16_t)0x0000D0F2,(fix16_t)0x0000CD3D,(fix16_t)0x0000C968,(fix16_t)0x0000C574,(fix16_t)0x0000C161,(fix16_t)0x0000BD30,(fix16_t)0x0000B8E1,
  (fix16_t)0x0000B476,(fix16_t)0x0000AFEF,(fix16_t)0x0000AB4C,(fix16_t)0x0000A68F,(fix16_t)0x0000A1B8,(fix16_t)0x00009CC8,(fix16_t)0x000097BF,(fix16_t)0x0000929F,(fix16_t)0x00008D68,(fix16_t)0x0000881B,(fix16_t)0x000082B9,(fix16_t)0x00007D42,(fix16_t)0x000077B8,(fix16_t)0x0000721C,(fix16_t)0x00006C6E,(fix16_t)0x000066AF,
  (fix16_t)0x000060E0,(fix16_t)0x00005B01,(fix16_t)0x00005515,(fix16_t)0x00004F1C,(fix16_t)0x00004916,(fix16_t)0x00004305,(fix16_t)0x00003CE9,(fix16_t)0x000036C4,(fix16_t)0x00003097,(fix16_t)0x00002A62,(fix16_t)0x00002426,(fix16_t)0x00001DE5,(fix16_t)0x0000179F,(fix16_t)0x00001155,(fix16_t)0x00000B09,(fix16_t)0x000004BB,
  (fix16_t)0xFFFFFE6C,(fix16_t)0xFFFFF81E,(fix16_t)0xFFFFF1D1,(fix16_t)0xFFFFEB86,(fix16_t)0xFFFFE53E,(fix16_t)0xFFFFDEFA,(fix16_t)0xFFFFD8BB,(fix16_t)0xFFFFD283,(fix16_t)0xFFFFCC51,(fix16_t)0xFFFFC628,(fix16_t)0xFFFFC008,(fix16_t)0xFFFFB9F1,(fix16_t)0xFFFFB3E6,(fix16_t)0xFFFFADE6,(fix16_t)0xFFFFA7F3,(fix16_t)0xFFFFA20E,
  (fix16_t)0xFFFF9C37,(fix16_t)0xFFFF9670,(fix16_t)0xFFFF90B9,(fix16_t)0xFFFF8B14,(fix16_t)0xFFFF8580,(fix16_t)0xFFFF8000,(fix16_t)0xFFFF7A94,(fix16_t)0xFFFF753C,(fix16_t)0xFFFF6FFA,(fix16_t)0xFFFF6ACE,(fix16_t)0xFFFF65BA,(fix16_t)0xFFFF60BD,(fix16_t)0xFFFF5BD9,(fix16_t)0xFFFF570F,(fix16_t)0xFFFF525F,(fix16_t)0xFFFF4DCA,
  (fix16_t)0xFFFF4951,(fix16_t)0xFFFF44F4,(fix16_t)0xFFFF40B4,(fix16_t)0xFFFF3C92,(fix16_t)0xFFFF388E,(fix16_t)0xFFFF34A9,(fix16_t)0xFFFF30E4,(fix16_t)0xFFFF2D3F,(fix16_t)0xFFFF29BB,(fix16_t)0xFFFF2658,(fix16_t)0xFFFF2317,(fix16_t)0xFFFF1FF8,(fix16_t)0xFFFF1CFC,(fix16_t)0xFFFF1A24,(fix16_t)0xFFFF176F,(fix16_t)0xFFFF14DE,
  (fix16_t)0xFFFF1272,(fix16_t)0xFFFF102A,(fix16_t)0xFFFF0E08,(fix16_t)0xFFFF0C0C,(fix16_t)0xFFFF0A35,(fix16_t)0xFFFF0885,(fix16_t)0xFFFF06FB,(fix16_t)0xFFFF0598,(fix16_t)0xFFFF045C,(fix16_t)0xFFFF0347,(fix16_t)0xFFFF0259,(fix16_t)0xFFFF0192,(fix16_t)0xFFFF00F4,(fix16_t)0xFFFF007C,(fix16_t)0xFFFF002D,(fix16_t)0xFFFF0005,
  (fix16_t)0xFFFF0005,(fix16_t)0xFFFF002D,(fix16_t)0xFFFF007C,(fix16_t)0xFFFF00F4,(fix16_t)0xFFFF0192,(fix16_t)0xFFFF0259,(fix16_t)0xFFFF0347,(fix16_t)0xFFFF045C,(fix16_t)0xFFFF0598,(fix16_t)0xFFFF06FB,(fix16_t)0xFFFF0885,(fix16_t)0xFFFF0A35,(fix16_t)0xFFFF0C0C,(fix16_t)0xFFFF0E08,(fix16_t)0xFFFF102A,(fix16_t)0xFFFF1272,
  (fix16_t)0xFFFF14DE,(fix16_t)0xFFFF176F,(fix16_t)0xFFFF1A24,(fix16_t)0xFFFF1CFC,(fix16_t)0xFFFF1FF8,(fix16_t)0xFFFF2317,(fix16_t)0xFFFF2658,(fix16_t)0xFFFF29BB,(fix16_t)0xFFFF2D3F,(fix16_t)0xFFFF30E4,(fix16_t)0xFFFF34A9,(fix16_t)0xFFFF388E,(fix16_t)0xFFFF3C92,(fix16_t)0xFFFF40B4,(fix16_t)0xFFFF44F4,(fix16_t)0xFFFF4951,
  (fix16_t)0xFFFF4DCA,(fix16_t)0xFFFF525F,(fix16_t)0xFFFF570F,(fix16_t)0xFFFF5BD9,(fix16_t)0xFFFF60BD,(fix16_t)0xFFFF65BA,(fix16_t)0xFFFF6ACE,(fix16_t)0xFFFF6FFA,(fix16_t)0xFFFF753C,(fix16_t)0xFFFF7A94,(fix16_t)0xFFFF8000,(fix16_t)0xFFFF8580,(fix16_t)0xFFFF8B14,(fix16_t)0xFFFF90B9,(fix16_t)0xFFFF9670,(fix16_t)0xFFFF9C37,
  (fix16_t)0xFFFFA20E,(fix16_t)0xFFFFA7F3,(fix16_t)0xFFFFADE6,(fix16_t)0xFFFFB3E6,(fix16_t)0xFFFFB9F1,(fix16_t)0xFFFFC008,(fix16_t)0xFFFFC628,(fix16_t)0xFFFFCC51,(fix16_t)0xFFFFD283,(fix16_t)0xFFFFD8BB,(fix16_t)0xFFFFDEFA,(fix16_t)0xFFFFE53E,(fix16_t)0xFFFFEB86,(fix16_t)0xFFFFF1D1,(fix16_t)0xFFFFF81E,(fix16_t)0xFFFFFE6C,
  (fix16_t)0x000004BB,(fix16_t)0x00000B09,(fix16_t)0x00001155,(fix16_t)0x0000179F,(fix16_t)0x00001DE5,(fix16_t)0x00002426,(fix16_t)0x00002A62,(fix16_t)0x00003097,(fix16_t)0x000036C4,(fix16_t)0x00003CE9,(fix16_t)0x00004305,(fix16_t)0x00004916,(fix16_t)0x00004F1C,(fix16_t)0x00005515,(fix16_t)0x00005B01,(fix16_t)0x000060E0,
  (fix16_t)0x000066AF,(fix16_t)0x00006C6E,(fix16_t)0x0000721C,(fix16_t)0x000077B8,(fix16_t)0x00007D42,(fix16_t)0x000082B9,(fix16_t)0x0000881B,(fix16_t)0x00008D68,(fix16_t)0x0000929F,(fix16_t)0x000097BF,(fix16_t)0x00009CC8,(fix16_t)0x0000A1B8,(fix16_t)0x0000A68F,(fix16_t)0x0000AB4C,(fix16_t)0x0000AFEF,(fix16_t)0x0000B476,
  (fix16_t)0x0000B8E1,(fix16_t)0x0000BD30,(fix16_t)0x0000C161,(fix16_t)0x0000C574,(fix16_t)0x0000C968,(fix16_t)0x0000CD3D,(fix16_t)0x0000D0F2,(fix16_t)0x0000D487,(fix16_t)0x0000D7FB,(fix16_t)0x0000DB4D,(fix16_t)0x0000DE7D,(fix16_t)0x0000E18A,(fix16_t)0x0000E474,(fix16_t)0x0000E73B,(fix16_t)0x0000E9DE,(fix16_t)0x0000EC5D,
  (fix16_t)0x0000EEB6,(fix16_t)0x0000F0EB,(fix16_t)0x0000F2FB,(fix16_t)0x0000F4E4,(fix16_t)0x0000F6A7,(fix16_t)0x0000F845,(fix16_t)0x0000F9BB,(fix16_t)0x0000FB0B,(fix16_t)0x0000FC34,(fix16_t)0x0000FD35,(fix16_t)0x0000FE0F,(fix16_t)0x0000FEC2,(fix16_t)0x0000FF4D,(fix16_t)0x0000FFB0,(fix16_t)0x0000FFEC,(fix16_t)0x00010000*/
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
    drawTriangles(scanline, y, maxTrianglesPerLine[y]);
    drawCircles(scanline, y, maxCirclesPerLine[y]);
    drawParticles(scanline, y);
    flushLine16(scanline);
  }
  numTriangles = 0;
  numCircles = 0;
  numParticles = 0;
}

void Display::drawCreature(const Creature &creature)
{
  BodyPart *body = creature.head->next;
  interpolateModels(creature.head->model, creature.mouthClosed, creature.head->transform, creature.head->scale, creature.head->color, creature.head->alpha, creature.t);
  while (body != nullptr)
  {
    drawModel(body->model, body->transform, body->scale, body->color, body->alpha);
    body = body->next;
  }
}

void Display::interpolateModels(const Model *model1, const Model *model2, const Transform &transform, const SQ15x16 &scale, uint16_t color, uint8_t alpha, int16_t t)
{
  InternalTriangle *triangle = &triangles[numTriangles];
  InternalCircle *circle = &circles[numCircles];
  for (int i = 0; i < model1->numTriangles; ++i)
  {
    interpolateTriangle(model1->triangles[i], model2->triangles[i], transform, triangle, t);
    triangle->firstLine = std::max((int16_t)0, std::min(triangle->p1[1], std::min(triangle->p2[1], triangle->p3[1])));
    triangle->lastLine = std::min((int16_t)175, std::max(triangle->p1[1], std::max(triangle->p2[1], triangle->p3[1])));
    if (triangle->firstLine >= 176 || triangle->lastLine < 0) //triangle off screen
      continue;
    triangle->color = color;
    triangle->alpha = alpha;
    for (int y = triangle->firstLine; y <= triangle->lastLine; ++y)
      ++maxTrianglesPerLine[y];
    ++numTriangles;
    ++triangle;
  }
  for (int i = 0; i < model1->numCircles; ++i)
  {
    interpolateCircle(model1->circles[i], model2->circles[i], transform, scale, circle, t);
    circle->color = color;
    circle->alpha = alpha;
    circle->firstLine = std::max(0, circle->center[1] - circle->radius);
    circle->lastLine = std::min(175, circle->center[1] + circle->radius);
    if (circle->firstLine >= 176 || circle->lastLine < 0) //circle off screen
      continue;
    for (int y = circles[numCircles].firstLine; y <= circles[numCircles].lastLine; ++y)
      ++maxCirclesPerLine[y];
    ++numCircles;
    ++circle;
  }
}

void Display::drawModel(const Model *model, const Transform &transform, const SQ15x16 &scale, uint16_t color, uint8_t alpha)
{
  InternalTriangle *triangle = &triangles[numTriangles];
  InternalCircle *circle = &circles[numCircles];
  for (int i = 0; i < model->numTriangles; ++i)
  {
    transformTriangle(model->triangles[i], transform, triangle);
    triangle->firstLine = std::max((int16_t)0, std::min(triangle->p1[1], std::min(triangle->p2[1], triangle->p3[1])));
    triangle->lastLine = std::min((int16_t)175, std::max(triangle->p1[1], std::max(triangle->p2[1], triangle->p3[1])));
    if (triangle->firstLine >= 176 || triangle->lastLine < 0) //triangle off screen
      continue;
    triangle->color = color;
    triangle->alpha = alpha;
    for (int y = triangle->firstLine; y <= triangle->lastLine; ++y)
      ++maxTrianglesPerLine[y];
    ++numTriangles;
    ++triangle;
  }
  for (int i = 0; i < model->numCircles; ++i)
  {
    transformCircle(model->circles[i], transform, scale, circle);
    circle->color = color;
    circle->alpha = alpha;
    circle->firstLine = std::max(0, circle->center[1] - circle->radius);
    circle->lastLine = std::min(175, circle->center[1] + circle->radius);
    if (circle->firstLine >= 176 || circle->lastLine < 0) //circle off screen
      continue;
    for (int y = circles[numCircles].firstLine; y <= circles[numCircles].lastLine; ++y)
      ++maxCirclesPerLine[y];
    ++numCircles;
    ++circle;
  }
}

void Display::drawTriangle(const Triangle &triangle, uint16_t color, uint8_t alpha)
{
  if (numTriangles == maxTriangles)
    return;
  triangles[numTriangles].firstLine = std::max((int16_t)0, std::min(triangle.p1[1], std::min(triangle.p2[1], triangle.p3[1])));
  triangles[numTriangles].lastLine = std::min((int16_t)175, std::max(triangle.p1[1], std::max(triangle.p2[1], triangle.p3[1])));
  if (triangles[numTriangles].firstLine >= 176 || triangles[numTriangles].lastLine < 0) //triangle off screen
    return;
  triangles[numTriangles].p1[0] = triangle.p1[0];
  triangles[numTriangles].p1[1] = triangle.p1[1];
  triangles[numTriangles].p2[0] = triangle.p2[0];
  triangles[numTriangles].p2[1] = triangle.p2[1];
  triangles[numTriangles].p3[0] = triangle.p3[0];
  triangles[numTriangles].p3[1] = triangle.p3[1];
  triangles[numTriangles].color = color;
  triangles[numTriangles].alpha = alpha;
  for (int y = triangles[numTriangles].firstLine; y <= triangles[numTriangles].lastLine; ++y)
    ++maxTrianglesPerLine[y];
  ++numTriangles;
}

void Display::drawCircle(const Circle &circle, uint16_t color, uint8_t alpha)
{
  if (numCircles == maxCircles)
    return;
  circles[numCircles].center[0] = circle.center[0];
  circles[numCircles].center[1] = circle.center[1];
  circles[numCircles].radius = circle.radius;
  circles[numCircles].color = color;
  circles[numCircles].alpha = alpha;
  circles[numCircles].firstLine = std::max(0, circle.center[1] - circle.radius);
  circles[numCircles].lastLine = std::min(175, circle.center[1] + circle.radius);
  for (int y = circles[numCircles].firstLine; y <= circles[numCircles].lastLine; ++y)
    ++maxCirclesPerLine[y];
  ++numCircles;
}

void Display::drawParticle(const Particle &particle)
{
  drawPoint(int16_t(particle.offset[0] + SQ15x16(particle.distance) * particle.vector[0]), int16_t(particle.offset[1] + SQ15x16(particle.distance) * particle.vector[1]), particle.color, std::min((uint8_t)32, particle.life));
}

void Display::drawPoint(int16_t x, int16_t y, uint16_t color, uint8_t alpha)
{
  if (numParticles == maxParticles)
    return;
  particles[numParticles].x = x;
  particles[numParticles].y = y;
  particles[numParticles].color = color;
  particles[numParticles].alpha = alpha;
  if (particles[numParticles].x < 0)
    return;
  if (particles[numParticles].x >= 220)
    return;
  if (particles[numParticles].y < 0)
    return;
  if (particles[numParticles].y >= 176)
    return;
  ++numParticles;
}

void Display::setTonePacing(uint8_t t, uint8_t p)
{
  tone = t;
  pacing = p;
}

void Display::rotPoint(SQ15x16 &x, SQ15x16 &y, uint8_t amount)
{
  SQ15x16 nx = x;
  SQ15x16 ny = y;
  SQ15x16 s = sinTable[amount];
  SQ15x16 c = cosTable[amount];
  x = nx * c - ny * s;
  y = nx * s + ny * c;
}

void Display::slerpVector(SQ15x16 &x1, SQ15x16 &y1, const SQ15x16 &x2, const SQ15x16 &y2, const SQ15x16 &amount, const SQ15x16 &scale)
{
  SQ15x16 len;
  SQ15x16 len2 = sqrt(x2 * x2 + y2 * y2) * scale;
  //SQ15x16 invsqrt;
  x1 += amount * (x2 - x1);
  y1 += amount * (y2 - y1);
  len = sqrt(x1 * x1 + y1 * y1);
  x1 = x1 * len2 / len;
  y1 = y1 * len2 / len;
  //invsqrt = fastInverseSquareRoot(float(x1 * x1 + y1 * y1));
  //x1 *= invsqrt * len;
  //y1 *= invsqrt * len;
}

bool Display::floatsEqual(SQ15x16 f1, SQ15x16 f2)
{
  return f1 == f2;
  //if (f1 < f2)
  //  return (f2 - f1).value < threshold;
  //return (f1 - f2).value < threshold;
}

bool Display::isOffScreen(int16_t x, int16_t y)
{
  if (x < 0)
    return true;
  if (x > 219)
    return true;
  if (y < 0)
    return true;
  if (y > 175)
    return true;
  return false;
}

void Display::clipToScreen(int16_t &x, int16_t &y)
{
  if (x < 0)
    x = 0;
  if (x > 219)
    x = 219;
  if (y < 0)
    y = 0;
  if (y > 175)
    y = 175;
}

SQ15x16 Display::fastInverseSquareRoot(float value)
{
  const float x2 = value * 0.5F;
  const float threehalfs = 1.5F;
  
  union {
    float f;
    uint32_t i;
  } conv  = { .f = value };
  conv.i  = 0x5f3759df - ( conv.i >> 1 );
  conv.f  *= threehalfs - ( x2 * conv.f * conv.f );
  return SQ15x16(conv.f);
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

uint16_t Display::alphaBlendRGB565(uint32_t fg, uint32_t bg, uint8_t alpha)
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

bool Display::getIntersect(int16_t &x /*OUT*/, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  if (y1 < y && y2 < y)
    return false;
  if (y1 > y && y2 > y)
    return false;
  if (y1 == y2) //shouldn't receive this value
    return false;
  x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
  return true;
}

void Display::getTriangleStartEnd(int16_t &start /*OUT*/, int16_t &end /*OUT*/, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3)
{
  if (y1 == y)
  {
    if (y2 == y)
    {
      start = x1;
      end = x2;
    }
    else if (y3 == y)
    {
      start = x1;
      end = x3;
    }
    else
    {
      start = x1;
      end = x1; //in case p2->p3 doesn't intersect scanline
      getIntersect(end, y, x2, y2, x3, y3);
    }
  }
  else if (y2 == y)
  {
    if (y3 == y)
    {
      start = x2;
      end = x3;
    }
    else
    {
      start = x2;
      end = x2; //in case p3->p1 doesn't intersect scanline
      getIntersect(end, y, x3, y3, x1, y1);
    }
  }
  else if (y3 == y)
  {
    start = x3;
    end = x3; //in case p1->p2 doesn't intersect scanline
    getIntersect(end, y, x1, y1, x2, y2);
  }
  else
  {
    if (getIntersect(start, y, x1, y1, x2, y2))
    {
      if (!getIntersect(end, y, x2, y2, x3, y3))
        getIntersect(end, y, x3, y3, x1, y1);
    }
    else
    {
      getIntersect(start, y, x2, y2, x3, y3);
      getIntersect(end, y, x3, y3, x1, y1);
    }
  }
  if (start > end)
    swapWT(int16_t,start,end);
  if (start < 0)
    start = 0;
  if (end > 219)
    end = 219;
}

void Display::drawTriangles(uint16_t *scanline, int y, uint8_t &max)
{
  InternalTriangle *triangle = triangles;
  int16_t start, end;
  uint16_t *pixel;
  for (int i = 0; i < numTriangles && max > 0; ++i, ++triangle)
  {
    if (y < triangle->firstLine || y > triangle->lastLine)
      continue;
    getTriangleStartEnd(start, end, y, triangle->p1[0], triangle->p1[1], triangle->p2[0], triangle->p2[1], triangle->p3[0], triangle->p3[1]);
    pixel = scanline + start;
    for (int16_t x = start; x <= end; ++x, ++pixel) //TODO enable alpha blending
      *pixel = alphaBlendRGB565(triangle->color, *pixel, triangle->alpha);
    --max;
  }
}

void Display::drawCircles(uint16_t *scanline, int y, uint8_t &max)
{
  InternalCircle *circle = circles;
  int16_t start, end;
  uint16_t *pixel;
  for (int i = 0; i < numTriangles && max > 0; ++i, ++circle)
  {
    if (y < circle->firstLine || y > circle->lastLine)
      continue;
    int16_t dist2 = (circle->center[1] - y) * (circle->center[1] - y);
    int16_t r2 = circle->radius * circle->radius;
    start = circle->center[0] - circle->radius;
    end = circle->center[0] + circle->radius;
    while ((circle->center[0] - start) * (circle->center[0] - start) + dist2 > r2)
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
      *pixel = alphaBlendRGB565(circle->color, *pixel, circle->alpha);
    --max;
  }
}

void Display::drawParticles(uint16_t *scanline, int y)
{
  InternalParticle *particle = particles;
  for (int i = 0; i < numParticles; ++i, ++particle)
  {
    if (y == particle->y)
      scanline[particle->x] = alphaBlendRGB565(particle->color, scanline[particle->x], particle->alpha);
  }
}

void Display::interpolateCircle(const Circle &in1, const Circle &in2, const Transform &transform, const SQ15x16 &scale, InternalCircle *out, int16_t t)
{
  if (t == 0)
    return transformCircle(in1, transform, scale, out);
  else if (t == 128)
    return transformCircle(in2, transform, scale, out);
  SQ15x16 x = (int16_t)(t * (in2.center[0] - in1.center[0]) / 128 + in1.center[0]);
  SQ15x16 y = (int16_t)(t * (in2.center[1] - in1.center[1]) / 128 + in1.center[1]);
  out->radius = int16_t(scale * (t * (in2.radius - in1.radius) / 128 + in1.radius));
  out->center[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->center[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
}

void Display::transformCircle(const Circle &in, const Transform &transform, const SQ15x16 &scale, InternalCircle *out)
{
  SQ15x16 x = in.center[0];
  SQ15x16 y = in.center[1];
  out->center[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->center[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
  out->radius = int16_t(scale * in.radius);
}

void Display::interpolateTriangle(const Triangle &in1, const Triangle &in2, const Transform &transform, InternalTriangle *out, int16_t t)
{
  if (t == 0)
    return transformTriangle(in1, transform, out);
  else if (t == 128)
    return transformTriangle(in2, transform, out);
  SQ15x16 x = t * (in2.p1[0] - in1.p1[0]) / 128 + in1.p1[0];
  SQ15x16 y = t * (in2.p1[1] - in1.p1[1]) / 128 + in1.p1[1];
  out->p1[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->p1[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
  x = t * (in2.p2[0] - in1.p2[0]) / 128 + in1.p2[0];
  y = t * (in2.p2[1] - in1.p2[1]) / 128 + in1.p2[1];
  out->p2[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->p2[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
  x = t * (in2.p3[0] - in1.p3[0]) / 128 + in1.p3[0];
  y = t * (in2.p3[1] - in1.p3[1]) / 128 + in1.p3[1];
  out->p3[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->p3[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
}

void Display::transformTriangle(const Triangle &in, const Transform &transform, InternalTriangle *out)
{
  SQ15x16 x = in.p1[0];
  SQ15x16 y = in.p1[1];
  out->p1[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->p1[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
  x = in.p2[0];
  y = in.p2[1];
  out->p2[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->p2[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
  x = in.p3[0];
  y = in.p3[1];
  out->p3[0] = int16_t(x * transform.xVector[0] + y * transform.yVector[0] + transform.offset[0]);
  out->p3[1] = int16_t(x * transform.xVector[1] + y * transform.yVector[1] + transform.offset[1]);
}
