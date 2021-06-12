#include "preCompiled.h"
#include "HttpRequester.h"


bool HttpRequester::makeRequest(std::string const& body)
{
	Poco::Net::HTTPResponse response{};
	Poco::URI uri{ m_url };
	std::string path(uri.getPathAndQuery());
	if (path.empty()) path = "/";
	m_responseBody.clear();
	m_httpStatus = Poco::Net::HTTPResponse::HTTPStatus::HTTP_GONE;

	Poco::Net::HTTPRequest request{ Poco::Net::HTTPRequest::HTTP_GET, path,Poco::Net::HTTPRequest::HTTPMessage::HTTP_1_1 };
	Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
	request.setContentType("text/xml");
	request.setContentLength(body.size());
	try {
		auto& ss = session.sendRequest(request);
		ss.write(body.c_str(), body.size());
		auto& res = session.receiveResponse(response);
		m_httpStatus = response.getStatus();
		if (m_httpStatus == Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK) {
			std::string responseData{};
			while (res) {
				char buffer[1024];
				res.read(buffer, 1023);
				buffer[res.gcount()] = '\0';
				responseData.append(buffer);
			}
			m_responseBody = std::move(responseData);
			return true;
		}
		else {
			m_reasonForError = response.getReasonForStatus(m_httpStatus);
			return false;
		}
	}
	catch (const std::exception& ex) {
		m_reasonForError = ex.what();
	}
	return false;
}
std::string HttpRequester::getReasonForError()
{
	return m_reasonForError;
}
std::string& HttpRequester::getBodyResponse()
{
	return m_responseBody;
}

