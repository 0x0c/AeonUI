#pragma once

#include <StandardCplusplus.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <sstream>
#include <Arduino.h>

namespace AeonUI
{
	class Util
	{
	public:
		static std::vector<std::string> split(const std::string &str, char delim)
		{
			std::vector<std::string> res;
			size_t current = 0, found;
			while((found = str.find_first_of(delim, current)) != std::string::npos){
				res.push_back(std::string(str, current, found - current));
				current = found + 1;
			}
			res.push_back(std::string(str, current, str.size() - current));

			return res;
		}
	};
};