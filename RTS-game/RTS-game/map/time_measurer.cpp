#include "time_measurer.h"

#include <iostream>

TimeMeasurer::TimeMeasurer() : start_time_(std::chrono::system_clock::now()) {}

void TimeMeasurer::PrintTime(const std::string& message) {
	auto time = std::chrono::system_clock::now();
	std::cout << message << (message.empty() ? "" : ": ") <<
		static_cast<double>((time - start_time_).count()) / 10000000
		<< " s" << std::endl;
	start_time_ = time;
}

