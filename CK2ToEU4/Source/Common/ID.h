#ifndef ID_H
#define ID_H

#include "newParser.h"

class ID: commonItems::parser
{
public:
	ID() = default;
	explicit ID(std::istream& theStream);

	[[nodiscard]] auto getIDNum() const { return IDNum; }

private:
	int IDNum = 0;
	int IDType = 0;
};

#endif // ID_H