#pragma once

#include <Root/tilegrids/TileSet.h>

#include <memory>
#include <string>
#include <map>

namespace TileGridEngine
{
	void addTileSet(std::shared_ptr<TileSet> tileSet, const std::string& name);

	TileSet* getTileSet(const std::string& name);

	bool isNumber(char c);

	unsigned int readNumber(std::ifstream* file, char first);
};

