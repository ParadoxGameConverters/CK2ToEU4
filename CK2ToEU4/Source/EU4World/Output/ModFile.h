#ifndef MOD_FILE_H
#define MOD_FILE_H

#include <ostream>

namespace EU4
{
class ModFile
{
  public:
	friend std::ostream& operator<<(std::ostream& output, const ModFile& modFile);
};
} // namespace EU4
#endif // MOD_FILE_H
