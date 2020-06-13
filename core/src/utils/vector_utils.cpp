#include <string>

#include "../../include/utils/vector_utils.h"

namespace jade_core
{
	std::string vector_utils::to_string(std::vector<std::string> vector, const std::string delimiter)
	{
		std::string vector_string = "";

		for (int arrayIndex = 0; arrayIndex < vector.size(); ++arrayIndex)
		{
			vector_string += vector[arrayIndex];

			if(arrayIndex < vector.size() - 1)
			{
				vector_string += delimiter;
			}
		}

		return vector_string;
	}
}
