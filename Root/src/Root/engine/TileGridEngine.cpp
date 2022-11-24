#include "TileGridEngine.h"

namespace TileGridEngine
{
    namespace
    {
        std::map<std::string, std::shared_ptr<TileSet>> tileSets;
    }

	void update()
	{
		// Create a map iterator and point to beginning of map
		std::map<std::string, std::shared_ptr<TileSet>>::iterator it = tileSets.begin();

		// Iterate over the map using Iterator till end
		while (it != tileSets.end())
		{
			it->second->update();
			// Increment the Iterator to point to next entry
			it++;
		}

		for (std::shared_ptr<TileSet> tileSet : tileSets.) {

		}
	}

    void addTileSet(std::shared_ptr<TileSet> tileSet, const std::string& name)
    {
        tileSets.emplace(name, tileSet);
    }

	TileSet* getTileSet(const std::string& name)
	{
		std::map<std::string, std::shared_ptr<TileSet>>::iterator it = tileSets.find(name);

		if (it != tileSets.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	bool isNumber(char c)
	{
		const std::string numbers{ "0123456789" };
		return numbers.find(c) != std::string::npos;
	}

	unsigned int readNumber(std::ifstream* file, char first)
	{
		std::string number{ "" };

		number.push_back(first);

		// While we keep encountering numbers
		while (file->good() && isNumber(file->peek()))
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
			Logger::logError(std::string("Could not read number: ") + number + ", " + ex.what());
		}
		return result;
	}
};
