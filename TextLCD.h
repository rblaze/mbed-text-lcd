#pragma once

#include <mbed.h>

template <typename Phys>
class TextLCD : public Phys {
  static_assert(Phys::kLineOffsets.size() == Phys::kNumLines);

 public:
  using Phys::Phys;

  constexpr int width() const { return Phys::kNumColumns; }
  constexpr int height() const { return Phys::kNumLines; }

  void clearScreen() { Phys::writeCommand(CMD_CLS); }

  void moveCursorHome() { Phys::writeCommand(CMD_HOME); }

  void moveCursorTo(int column, int line) {
    MBED_ASSERT(column >= 0 && column < Phys::kNumColumns);
    MBED_ASSERT(line >= 0 && line < Phys::kNumLines);

    int addr = Phys::kLineOffsets[line] + column;
    int cmd = CMD_DDRAM_ADDR | (addr & 0x7f);
    Phys::writeCommand(cmd);
  }

  void displayControl(bool display_on, bool cursor_on, bool cursor_blinking) {
    int cmd = CMD_DISPLAY_CONTROL;

    if (display_on) {
      cmd |= 0x04;
    }

    if (cursor_on) {
      cmd |= 0x02;
    }

    if (cursor_blinking) {
      cmd |= 0x01;
    }

    Phys::writeCommand(cmd);
  }

  void write(const char *str) {
    Phys::writeData(Span<const uint8_t>((uint8_t *)str, strlen(str)));
  }

  void write(const char *str, size_t len) {
    Phys::writeData(Span<const uint8_t>((uint8_t *)str, len));
  }

  void write(Span<const uint8_t> str) { Phys::writeData(str); }

 private:
  enum COMMAND : uint8_t {
    CMD_CLS = 0x01,
    CMD_HOME = 0x02,
    CMD_ENTRY_MODE_SET = 0x04,
    CMD_DISPLAY_CONTROL = 0x08,
    CMD_SHIFT = 0x10,
    CMD_FUNCTION = 0x20,
    CMD_CGRAM_ADDR = 0x40,
    CMD_DDRAM_ADDR = 0x80,
  };
};