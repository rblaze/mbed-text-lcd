#pragma once

#include <mbed.h>

#include <array>

namespace text_lcd {

// Physical layer for I2C connected displays.
class ST7036i_20x2 {
 public:
  ST7036i_20x2(I2C& bus, int address) : bus_{bus}, address_{address} {}

 protected:
  void initialize();
  void writeCommand(uint8_t value);
  void writeData(Span<const uint8_t> data);

  static constexpr int kNumColumns{20};
  static constexpr int kNumLines{2};
  static constexpr std::array<int, kNumLines> kLineOffsets = {0x00, 0x40};

 private:
  I2C& bus_;
  const int address_;
};

}  // namespace text_lcd