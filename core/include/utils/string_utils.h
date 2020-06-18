#pragma once

#ifndef JADE_CORE_INCLUDE_UTILS_STRINGUTILS_H_
#define JADE_CORE_INCLUDE_UTILS_STRINGUTILS_H_

#include <string>
#include <vector>

namespace JadeCore
{
	static class StringUtils
	{
	public:

		/**
		 * \brief Splits a provided string with the given delimiter
		 * \param string
		 * \param delimiter
		 * \param prefix
		 * \param suffix
		 * \return
		 */
		static std::vector<std::string> Split(std::string string, char delimiter, std::string prefix = "", std::string suffix = "");
	};

}
#endif
