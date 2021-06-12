#include "preCompiled.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "RequestException.h"
#include "XMLParser.h"
#include "Requester.h"
#include "HttpRequester.h"

std::string Requester::getDataFromFile(std::string const& filename) {
	std::filesystem::path p{ filename };
	if (!p.is_absolute())
		p = std::filesystem::absolute(p);
	if (std::filesystem::exists(p) == false) {
		return std::string{};
	}
	std::ifstream is(p.c_str());
	if (!is) return std::string{};
	is.seekg(0, std::ios::end);
	auto size = is.tellg();
	is.seekg(0);
	std::string data(size, '\0');
	is.read(&data[0], size);
	is.close();
	return data;
}

bool Requester::writeFormatToFile( std::string const& filename,	 std::string const& data)
{
	std::ofstream os(filename, std::ios::binary);
	if (!os) return false;
	std::stringstream ss{ data };
	Poco::Base64Decoder decoder(ss);
	Poco::StreamCopier::copyStream(decoder, os);
	os.close();
	return true;
}
std::string Requester::getOutputFileName(std::string const& directory, std::string const& filename,
	std::string const& airbill, std::string const& format) {
	std::filesystem::path p{ directory };
	std::string f = std::filesystem::path(filename).stem().string();
	f += "-" + airbill + "." + format;
	p += f;
	if (!p.is_absolute())
		p = std::filesystem::absolute(p);
	return p.string();
}

AirbillInfo Requester::requestAirBill(std::string const& URL,
	std::string const& inputFileName,
	std::string const& outputDirectory) {

	AirbillInfo info;
	auto requestBody = getDataFromFile(inputFileName);
	if (requestBody.empty()) {
		throw RequestException("Invalid data file");
	}
	HttpRequester httpRequester{URL};
	if (httpRequester.makeRequest(requestBody))
	{
		XMLParser parser{};
		auto bodyResponse = httpRequester.getBodyResponse();
		if (parser.initparser(bodyResponse)) {
			if (!parser.isShipmentResponse())throw RequestException{ "No shipmentResponse" };

			info.number = parser.getAirBillNumber();
			info.format = parser.getOutputFormat();
			std::string outputFileName = getOutputFileName(outputDirectory, inputFileName, info.number, info.format);
			if (!writeFormatToFile(outputFileName, parser.getOutputImage())) {
				throw RequestException{ "Error writing label file" };
			}
			info.filename = outputFileName;
		}
		else {
			throw RequestException{ "Error reading XML file" };
		}
	}
	else {
		throw RequestException{ "Http error: " + httpRequester.getReasonForError()};
	}
	return info;
}
