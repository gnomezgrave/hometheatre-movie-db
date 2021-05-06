#include "Utils.h"

#include <numeric>
#include <vector>

Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}

int Utils::GetStringDifference( std::string str1, std::string str2 )
{
	const std::size_t len1 = str1.size(), len2 = str2.size();
	std::vector<unsigned int> col(len2+1), prevCol(len2+1);

	for (unsigned int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++)
			col[j+1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1), prevCol[j] + (str1[i]==str2[j] ? 0 : 1) );
		col.swap(prevCol);
	}
	return prevCol[len2];
}
