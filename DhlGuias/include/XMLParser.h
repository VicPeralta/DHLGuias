#pragma once
#include <string>
#include <wrl.h>
#include <xmllite.h>
#include <shlwapi.h>
class XMLParser
{
public:
	XMLParser() = default;
	bool initparser(std::string& data);
	std::string getAirBillNumber();
	std::string getOutputFormat();
	std::string getOutputImage();
	bool isShipmentResponse();
private:
	std::string m_outputFormat;
	std::string m_airBillNumber;
	std::string m_inputFileName;
	Microsoft::WRL::ComPtr<IStream> m_ptrStream;
	Microsoft::WRL::ComPtr<IXmlReader> m_ptrReader;
	std::string createSafeString(const wchar_t* p, int count);
};

