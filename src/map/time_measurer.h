#pragma once

#include <chrono>
#include <string>

class TimeMeasurer {
private:
	std::chrono::system_clock::time_point start_time_;
	
public:
	TimeMeasurer();

	void PrintTime(const std::string& message = "");
};