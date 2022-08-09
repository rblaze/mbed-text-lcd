#include "ST7036i.h"

namespace text_lcd {

namespace {

constexpr char kCommandByteWrite = 0x80;
constexpr char kCommandStreamWrite = 0x00;
constexpr char kDataStreamWrite = 0x40;

}  // namespace

ST7036i_20x2::ST7036i_20x2(I2C& bus, int address)
    : bus_{bus}, address_{address} {
  // Initialization sequence from datasheet.
  bus_.write(address_, &kCommandStreamWrite, sizeof(kCommandStreamWrite), true);
  // Function set: 8 bit, 2 lines, single height, IS 0b00
  bus_.write(0x38);
  ThisThread::sleep_for(10ms);
  // Function set: 8 bit, 2 lines, single height, IS 0b01
  bus_.write(0x39);
  ThisThread::sleep_for(10ms);
  // Bias set: 1/5 bias
  // Contrast set: 0b1000
  // PICC set: ICON display on, booster on, contast 0b10
  // Follower control: follower on, ratio 0b101
  // Display on/off: display on, cursor off, blink off
  // Clear screen
  // Entry mode set: increment AC, shift off.
  for (auto b : {0x14, 0x78, 0x5e, 0x6d, 0x0c, 0x01, 0x06}) {
    bus_.write(b);
  }
  ThisThread::sleep_for(10ms);
  bus_.stop();
}

void ST7036i_20x2::writeCommand(uint8_t value) {
  char buf[2] = {kCommandByteWrite, value};
  int result = bus_.write(address_, buf, sizeof(buf), false);
  if (result != 0) {
    MBED_WARNING1(
        MBED_MAKE_ERROR(MBED_MODULE_UNKNOWN, MBED_ERROR_FAILED_OPERATION),
        "Command write failed", result);
  }
}

void ST7036i_20x2::writeData(Span<const uint8_t> data) {
  int result =
      bus_.write(address_, &kDataStreamWrite, sizeof(kDataStreamWrite), true);
  if (result != 0) {
    MBED_WARNING1(
        MBED_MAKE_ERROR(MBED_MODULE_UNKNOWN, MBED_ERROR_FAILED_OPERATION),
        "Control byte write failed", result);
  }

  for (auto b : data) {
    result = bus_.write(b);
    if (result != 1) {
      MBED_WARNING1(
          MBED_MAKE_ERROR(MBED_MODULE_UNKNOWN, MBED_ERROR_FAILED_OPERATION),
          "Data byte write failed", result);
    }
  }

  bus_.stop();
}

}  // namespace text_lcd