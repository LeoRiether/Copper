#include "Colors.h"

#include "util.h"

#define MODULE "Colors"

SDL_Color colorFromHex(const char *hex) {
  if (hex[0] == '#')
    hex++;

  // h for... hexadecimal?
  auto h = [hex](char x) {
    if (x >= '0' && x <= '9')
      return x - '0';
    if (x >= 'a' && x <= 'f')
      return x - 'a' + 10;
    if (x >= 'A' && x <= 'F')
      return x - 'A' + 10;
    fail2("colorFromHex received weird rgb: %s", hex);
  };

  uint8_t r = h(hex[0]) * 16 + h(hex[1]);
  uint8_t g = h(hex[2]) * 16 + h(hex[3]);
  uint8_t b = h(hex[4]) * 16 + h(hex[5]);
  return SDL_Color{r, g, b, 255};
}
