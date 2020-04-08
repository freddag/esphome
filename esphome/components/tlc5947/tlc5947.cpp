#include "tlc5947.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tlc5947 {

static const char *TAG = "tlc5947";

void TLC5947::setup() {
  ESP_LOGCONFIG(TAG, "Setting up TLC5947OutputComponent...");
  this->data_pin_->setup();
  this->data_pin_->digital_write(false);
  this->clock_pin_->setup();
  this->clock_pin_->digital_write(false);
  this->pwm_amounts_.resize(this->num_channels_, 0);

  //reset everything to off
for(uint8_t index = 0; index < this->num_chips_; index++){
  // send blank bits to reset chip 287 bits
  this->rest_reg();
}

}
void TLC5947::dump_config() {
  ESP_LOGCONFIG(TAG, "TLC5947:");
  LOG_PIN("  Data Pin: ", this->data_pin_);
  LOG_PIN("  Clock Pin: ", this->clock_pin_);
  LOG_PIN("  Latch Pin: ", this->latch_pin_);
  ESP_LOGCONFIG(TAG, "  Total number of channels: %u", this->num_channels_);
  ESP_LOGCONFIG(TAG, "  Number of chips: %u", this->num_chips_);
}
void TLC5947::loop() {
  if (!this->update_)
    return;
  

  //set latch pin low
  this->latch_pin_->digital_write(false);
  // send 12bit amount for each channel
  for (uint8_t index = 0; index < this->num_channels_; index++) {
    // send a start bit initially and after every 3 channels
    this->write_byte_(this->pwm_amounts_[index]);
  }
  //set clock_pin low
  this->clock_pin_->digital_write(false);

  //write data to latch reg from shift reg.
  this->latch_pin_->digital_write(true);
  this->latch_pin_->digital_write(false);

  this->update_ = false;
}

}  // namespace sm16716
}  // namespace esphome
