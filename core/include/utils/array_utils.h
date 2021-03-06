#pragma once

#ifndef JADE_CORE_INCLUDE_UTILS_ARRAYUTILS_H_
#define JADE_CORE_INCLUDE_UTILS_ARRAYUTILS_H_

#include <string>

namespace JadeCore
{
	static class ArrayUtils
	{
	public:
		
		/**
		 * \brief Convert an array of strings to a single string
		 * \param array 
		 * \param delimiter 
		 * \return 
		 */
		static std::string to_string(std::string array[], const std::string& delimiter = ",");
	};
}

#endif