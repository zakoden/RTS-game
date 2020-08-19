#pragma once

class Clock {
private:
	int time_ = 0, frequency_;

public:
	Clock(unsigned int frequency);

	operator int() const;

	bool Ding() const;
	void Tick();
};