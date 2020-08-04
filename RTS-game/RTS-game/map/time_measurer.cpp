#include "time_measurer.h"

#include <iostream>

TimeMeasurer::TimeMeasurer() : start_time(std::chrono::system_clock::now()) {}

void TimeMeasurer::PrintTime(const std::string& message) const {
	std::cout << message << (message.empty() ? "" : ": ") <<
		static_cast<double>((std::chrono::system_clock::now() - start_time).count()) / 10000000
		<< " s" << std::endl;
}

