#ifndef CHIP8_UTIL_H
#define CHIP8_UTIL_H

using byte = unsigned char;
using ushort = unsigned short;
namespace util {
byte inline get_nth_nibble(ushort val, ushort n) {
  return (val >> n * 4) & 0xF;
}
byte inline get_nth_byte(ushort val, ushort n) { return (val >> n * 4) & 0xFF; }
ushort inline get_addr(ushort val) { return val & 0xFFF; }
}; // namespace util

#endif
