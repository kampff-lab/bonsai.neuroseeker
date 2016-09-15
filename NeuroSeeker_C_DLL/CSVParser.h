#pragma once

#include <istream>
#include <string>
#include <vector>


class CSVParser
{
public:
	std::vector<int> ChannelConfigReference;
	std::vector<int> ChannelConfigGain;
	std::vector<bool> ChannelConfigMode;
	std::vector<int> ChannelConfigBW;

	CSVParser(const char* filename);
	~CSVParser();

private:
	std::vector<std::string> readCSVRow(const std::string & row);
	std::vector<std::vector<std::string>> readCSV(std::istream & in);
	void ParsedStringDataToChannelConfigInts(std::vector<std::vector<std::string>> table);
};

