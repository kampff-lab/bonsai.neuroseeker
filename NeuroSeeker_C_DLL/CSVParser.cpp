
#include <istream>
#include <string>
#include <vector>
#include <iostream>  
#include <fstream> 
#include<map>
#include <string>

#include "CSVParser.h"
#include "ChannelConfiguration.h"


std::vector<int> ChannelConfigReference;
std::vector<int> ChannelConfigGain;
std::vector<bool> ChannelConfigMode;
std::vector<int> ChannelConfigBW;

CSVParser::CSVParser(const char* filename)
{
	std::filebuf confFileBuffer;
	if (confFileBuffer.open(filename, std::ios::in))
	{
		std::istream confFileIS(&confFileBuffer);
		std::vector<std::vector<std::string>> ParsedTable = readCSV(confFileIS);
		ParsedStringDataToChannelConfigInts(ParsedTable);
	}
	else {
		std::cout << "Reading of Channel Configuration CSV file failed.\n";
	}
}


CSVParser::~CSVParser()
{
}


enum class CSVState {
	UnquotedField,
	QuotedField,
	QuotedQuote
};

std::vector<std::string> CSVParser::readCSVRow(const std::string & row)
{
	CSVState state = CSVState::UnquotedField;
	std::vector<std::string> fields{ "" };
	size_t i = 0; // index of the current field
	for (char c : row) {
		switch (state) {
		case CSVState::UnquotedField:
			switch (c) {
			case ',': // end of field
				fields.push_back(""); i++;
				break;
			case '"': state = CSVState::QuotedField;
				break;
			default:  fields[i].push_back(c);
				break;
			}
			break;
		case CSVState::QuotedField:
			switch (c) {
			case '"': state = CSVState::QuotedQuote;
				break;
			default:  fields[i].push_back(c);
				break;
			}
			break;
		case CSVState::QuotedQuote:
			switch (c) {
			case ',': // , after closing quote
				fields.push_back(""); i++;
				state = CSVState::UnquotedField;
				break;
			case '"': // "" -> "
				fields[i].push_back('"');
				state = CSVState::QuotedField;
				break;
			default:  // end of quote
				state = CSVState::UnquotedField;
				break;
			}
			break;
		}
	}
	return fields;
}


/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::string>> CSVParser::readCSV(std::istream & in)
{
	std::vector<std::vector<std::string>> table;
	std::string row;
	while (true) {
		std::getline(in, row);
		if (in.bad() || in.eof()) {
			break;
		}
		auto fields = readCSVRow(row);
		table.push_back(fields);
	}
	return table;
}



// Structures to map from the strings of the CSV file to the ints and enum values of the API for the different channel configuration parameters
struct RefSelMap : public std::map<std::string, RefSel>
{
	RefSelMap()
	{
		this->operator[]("REF_SEL_NONE") = REF_SEL_NONE;
		this->operator[]("REF_SEL_EXT") = REF_SEL_EXT;
		this->operator[]("REF_SEL_L1") = REF_SEL_L1;
		this->operator[]("REF_SEL_L2") = REF_SEL_L2;
	};
	~RefSelMap() {}
};

struct GainMap : public std::map<std::string, int>
{
	GainMap()
	{
		this->operator[]("50") = 0;
		this->operator[]("125") = 1;
		this->operator[]("250") = 2;
		this->operator[]("500") = 3;
		this->operator[]("1000") = 4;
		this->operator[]("1500") = 5;
		this->operator[]("2000") = 6;
		this->operator[]("2500") = 7;
	};
	~GainMap() {}
};

struct ModeMap : public std::map < std::string, bool>
{
	ModeMap()
	{
		this->operator[]("AP") = true;
		this->operator[]("LFP") = false;
	};
	~ModeMap() {}
};

struct BWMap : public std::map<std::string, int>
{
	BWMap()
	{
		this->operator[]("300") = 0;
		this->operator[]("500") = 1;
		this->operator[]("NA") = 2;
		this->operator[]("1000") = 3;
	};
	~BWMap() {}
};


//Transform the String data from the CSV file into ChannelConfiguration.h enumeration ints
void CSVParser::ParsedStringDataToChannelConfigInts(std::vector<std::vector<std::string>> table)
{
	RefSelMap ReferenceSelectionMap;
	GainMap GainParameterMap;
	ModeMap ModeBandMap;
	BWMap BandwidthMap;


	for (int channel = 0; channel < table.size(); ++channel)
	{
		ChannelConfigReference.insert(ChannelConfigReference.end(), ReferenceSelectionMap[table[channel][0]]);
		ChannelConfigGain.insert(ChannelConfigGain.end(), GainParameterMap[table[channel][1]]);
		ChannelConfigMode.insert(ChannelConfigMode.end(), ModeBandMap[table[channel][2]]);
		ChannelConfigBW.insert(ChannelConfigBW.end(), BandwidthMap[table[channel][3]]);
	}
}

