#include "ST7036i.h"

namespace text_lcd {

namespace {

constexpr char kCommandByteWrite = 0x80;
constexpr char kCommandStreamWrite = 0x00;
constexpr char kDataStreamWrite = 0x40;

}  // namespace

void ST7036i_20x2::initialize() {
  // Initialization sequence from datasheet.
  bus_.start();
  bus_.write(address_);
  bus_.write(kCommandStreamWrite);
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
  int result = bus_.write(address_, buf, sizeof(buf));
  if (result != 0) {
    printf("lcd cmd failed\n");
    MBED_WARNING1(
        MBED_MAKE_ERROR(MBED_MODULE_UNKNOWN, MBED_ERROR_FAILED_OPERATION),
        "Command write failed", result);
  }
}

void ST7036i_20x2::writeData(Span<const uint8_t> data) {
  bus_.start();
  bus_.write(address_);
  bus_.write(kDataStreamWrite);

  for (auto b : data) {
    int result = bus_.write(b);
    if (result != 1) {
      MBED_WARNING1(
          MBED_MAKE_ERROR(MBED_MODULE_UNKNOWN, MBED_ERROR_FAILED_OPERATION),
          "Data byte write failed", result);
    }
  }

  bus_.stop();
}

}  // namespace text_lcd
