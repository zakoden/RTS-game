#include "clock.h"

Clock::Clock(unsigned int frequency) : frequency_(frequency) {}

bool Clock::Ding() const { return time_ == 0; }

void Clock::Tick() { time_ = (++time_) % frequency_; }