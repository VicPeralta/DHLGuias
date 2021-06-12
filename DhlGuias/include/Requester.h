
#pragma once
#include <string>
#include "AirbillInfo.h"

class Requester
{
public:
	Requester() = default;
	~Requester() = default;
	AirbillInfo requestAirBill(std::string const& URL, std::string const& inputFileName, std::string const& outputDirectory);
private:
	std::string getDataFromFile(std::string const& filename);
	bool writeFormatToFile(std::string const& filename, std::string const& data);
	std::string getOutputFileName(std::string const& directory, std::string const& filename,
		std::string const& airbill, std::string const& format);
};

