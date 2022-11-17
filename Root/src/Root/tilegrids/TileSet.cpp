#include "TileSet.h"

TileSet::~TileSet()
{
}

bool isNumber(char c)
{
	const std::string numbers{ "0123456789" };
	return numbers.find(c) != std::string::npos;
}

unsigned int readNumber(std::ifstream* file, char first)
{
	std::string number{ "" };

	// While we keep encountering numbers
	while (isNumber(file->peek()))
	{
		// Add the numbers to the string
		number.push_back(file->get());
	}

	int result{ 0 };

	try
	{
		result = std::stoi(number);
	}
	catch (std::invalid_argument const& ex)
	{
		Logger::log(std::string("Could not read number: ") + number + ", " + ex.what());
	}
	return result;
}

void TileSet::create(const std::string& path, const std::string& name)
{
	bool enableDebugLogging{ true };

	// Reading in the file
	std::ifstream file(path);
	if (file.is_open())
	{
		const std::string allowedTags{ "abcdefghijklmnopqrstuvwxyz" };

		// Holds a full line
		std::string line;

		// Keeps track of what phase we are at
		Phase phase{ Phase::TAG_READING };

		// Stop at the end or on an error
		while (file.good())
		{
			// Find the next char
			char c = file.get();

			// Just go the the next line
			if (c == '\n') {
				continue;
			}
			
			// Skip entire line after #
			if (c == '#') {
				std::getline(file, line);
				continue;
			}
			
			// Actual data: read it
			switch (phase)
			{
				case Phase::TAG_READING:
					// Check whether the character is a valid tag
					if (allowedTags.find(c) != std::string::npos)
					{
						if (enableDebugLogging)
							Logger::log(std::string("Found tag: ") + c);

						// Move onto the next phase
						phase = Phase::TEXTURE_INDEX_READING;
					}
					break;

				case Phase::TEXTURE_INDEX_READING:
					while (file.good())
					{
						// Ignore spaces
						if (c == ' ')
							break;

						// Just go the the next line
						if (c == '\n')
						{
							continue;
						}

						// Skip entire line after #
						if (c == '#')
						{
							std::getline(file, line);
							continue;
						}

						unsigned int coordinate1
					}

					break;

				case Phase::TILE_RULE_READING:
					break;
			}
		}
	}
	else {
		Logger::logError("Cannot read tile set from '" + path + "'; File cannot be opened.");
	}
}

TileSet::TileSet()
{
}
