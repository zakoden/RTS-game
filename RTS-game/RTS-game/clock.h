#pragma once

class Clock {
private:
	unsigned int time_ = 0, frequency_;

public:
	Clock(unsigned int frequency);

	bool Ding() const;
	void Tick();
};