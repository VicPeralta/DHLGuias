#include "preCompiled.h"
#include "XMLParser.h"
#include "RequestException.h"

bool XMLParser::initparser(std::string& data)
{
	auto d = (unsigned char*)data.data();
	m_ptrStream = SHCreateMemStream(d, static_cast<UINT>(data.size()));
	if (!m_ptrStream) return false;

	if (S_OK != CreateXmlReader(__uuidof(IXmlReader),
		reinterpret_cast<void**>(m_ptrReader.GetAddressOf()), nullptr)) {
		return false;;
	}
	if (S_OK != m_ptrReader->SetInput(m_ptrStream.Get())) {
		return false;
	}
	return true;
}

std::string XMLParser::getAirBillNumber()
{
	XmlNodeType nodeType;
	const wchar_t* name;
	UINT c{};
	std::string s{};
	while (m_ptrReader->Read(&nodeType) != S_FALSE) {
		if (nodeType == XmlNodeType_Element) {
			m_ptrReader->GetLocalName(&name, &c);
			s = createSafeString(name, c);
			if (s == "AirwayBillNumber") {
				const wchar_t* value;
				m_ptrReader->Read(&nodeType);
				m_ptrReader->GetValue(&value, &c);
				s = createSafeString(value, c);
				return s;
			}
		}
	}
	return std::string{};
}

std::string XMLParser::getOutputFormat()
{
	XmlNodeType nodeType;
	const wchar_t* name;
	while (m_ptrReader->Read(&nodeType) != S_FALSE) {
		if (nodeType == XmlNodeType_Element) {
			UINT c{};
			m_ptrReader->GetLocalName(&name, &c);
			std::string s = createSafeString(name, c);
			if (s == "OutputFormat") {
				const wchar_t* value;
				m_ptrReader->Read(&nodeType);
				m_ptrReader->GetValue(&value, &c);
				s = createSafeString(value, c);
				return s;
			}
		}
	}
	return std::string();
}

std::string XMLParser::getOutputImage()
{
	XmlNodeType nodeType;
	const wchar_t* name;
	while (m_ptrReader->Read(&nodeType) != S_FALSE) {
		if (nodeType == XmlNodeType_Element) {
			UINT c{};
			m_ptrReader->GetLocalName(&name, &c);
			std::string s = createSafeString(name, c);
			if (s == "OutputImage") {
				const wchar_t* value;
				m_ptrReader->Read(&nodeType);
				m_ptrReader->GetValue(&value, &c);
				s = createSafeString(value, c);
				return s;
			}
		}
	}
	return std::string{};
}

bool XMLParser::isShipmentResponse()
{
	XmlNodeType nodeType{};
	const wchar_t* name;
	UINT c{};
	std::string s;
	auto result = m_ptrReader->Read(&nodeType);
	while (result == S_OK) {
		if (nodeType == XmlNodeType_Element) {
			m_ptrReader->GetLocalName(&name, &c);
			s = createSafeString(name, c);
			if (s == "ShipmentValidateErrorResponse") {
				do {
					result = m_ptrReader->Read(&nodeType);
					m_ptrReader->GetLocalName(&name, &c);
					s = createSafeString(name, c);
					if (s == "ConditionData") {
						m_ptrReader->Read(&nodeType);
						m_ptrReader->GetValue(&name, &c);
						s = createSafeString(name, c);
						throw RequestException(s);
					}
				} while (result == S_OK);
			}
			else if (s == "ErrorResponse") {
				do {
					result = m_ptrReader->Read(&nodeType);
					m_ptrReader->GetLocalName(&name, &c);
					s = createSafeString(name, c);
					if (s == "ConditionData") {
						m_ptrReader->Read(&nodeType);
						m_ptrReader->GetValue(&name, &c);
						s = createSafeString(name, c);
						throw RequestException(s);
					}
				} while (result == S_OK);
			}
			// El primer elemento debe ser ShipmentResponse si la solicitud fue valida
			else if (s == "ShipmentResponse") {
				return true;
			}
		}
		result = m_ptrReader->Read(&nodeType);
	}
	return false;
}

std::string XMLParser::createSafeString(const wchar_t* p, int count)
{
	std::string safe(count, '\0');
	for (int i = 0; i < count; i++)
		safe[i] = static_cast<char>(*(p + i));
	return safe;
}
