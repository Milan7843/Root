#include "TileSet.h"

TileSet::~TileSet()
{
}

void TileSet::create(const std::string& path, const std::string& name)
{
	// Reading in the file
	std::ifstream file(path);
	if (file.is_open())
	{
		// Holds the next full line
		std::string line;

		// Keeps track of what 'phase' we are at
		std::string phase{ "tag" };

		// Stop at the end or on an error
		while (file.good())
		{
			// Retrieving the next full line
			std::getline(file, line);
			
			switch (phase)
		}
	}
	else {
		Logger::logError("Cannot read tile set from '" + path + "'; File cannot be opened.");
	}
}

TileSet::TileSet()
{
}
