#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#include <unordered_map>

using WordMap = std::unordered_map<std::string, int>;

class FileReader
{
public:
	bool ReadFile(const char* file)
	{
		if (!std::filesystem::is_regular_file(file)) return false;

		std::ifstream src(file, std::ios::binary);
		std::string content{ std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>() };

		std::stringstream ss(content);
		std::string word;
		while (ss >> word)
		{
			words_[word]++;
		}


		return true;
	}

	WordMap words_;
};