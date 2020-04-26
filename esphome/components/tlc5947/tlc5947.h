#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/log.h"

namespace esphome {
namespace tlc5947 {

class TLC5947 : public Component {
 public:
  class Channel;

  void set_data_pin(GPIOPin *data_pin) { data_pin_ = data_pin; }
  void set_clock_pin(GPIOPin *clock_pin) { clock_pin_ = clock_pin; }
  void set_latch_pin(GPIOPin *latch_pin) {latch_pin_ = latch_pin; }
  void set_num_chips(uint16_t num_chips) { num_chips_ = num_chips; }
  void set_num_channels() {
    num_channels_ = num_chips_*24;
    ESP_LOGD("Debug:", "num channels: %i", num_channels_); }
  

  void setup() override;

  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void write();
  /// Send new values if they were updated.
  void loop() override;

  class Channel : public output::FloatOutput {
   public:
    void set_parent(TLC5947 *parent) { parent_ = parent; }
    void set_channel(uint16_t channel) { channel_ = channel; }

   protected:
    void write_state(float state) override {
      auto amount = uint16_t(state * 0xFFF);
      ESP_LOGD("Debug:", "channel: %i", channel_);
      ESP_LOGD("Debug:", "value: %i", amount);
      this->parent_->set_channel_value_(this->channel_, amount);
    }

    TLC5947 *parent_;
    uint16_t channel_;
  };

 protected:
  void set_channel_value_(uint16_t channel, uint16_t value) {
    uint16_t index =  channel - 1;
    if (this->pwm_amounts_[index] != value) {
      this->update_ = true;
    }
    this->pwm_amounts_[index] = value;
  }
  void write_bit_(bool value) {
    // ESP_LOGD("Debug:", "write bit: %i", value);
    this->clock_pin_->digital_write(false);
    this->data_pin_->digital_write(value);
    this->clock_pin_->digital_write(true);
  }
  //mask byte to 
  void write_byte_(uint16_t data) {
    ESP_LOGD("Debug:", "write byte: %i", data);
    //this->latch_pin_->digital_write(false);
    for (uint16_t mask = 0x800; mask; mask >>= 1) {
      this->write_bit_(data & mask);
    }
  }
  //rest method not used
  void rest_reg(){
    for (uint8_t i = 0; i < 287; i++) {
    this->write_bit_(false);
    }
  }

  GPIOPin *data_pin_;
  GPIOPin *clock_pin_;
  GPIOPin *latch_pin_;
  uint16_t num_channels_;
  uint16_t num_chips_ = 1;
  std::vector<uint16_t> pwm_amounts_;
  bool update_{true};
};

}  // namespace sm16716
}  // namespace esphome
