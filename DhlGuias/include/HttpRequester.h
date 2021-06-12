#pragma once
#include <string>
class HttpRequester
{
private:
	std::string m_url;
	int port{ 80 };
	Poco::Net::HTTPResponse::HTTPStatus m_httpStatus{};
	std::string m_responseBody{};
	std::string m_reasonForError{};
public:
	HttpRequester() = delete;
	HttpRequester(std::string uri) :m_url{uri} {};
	bool makeRequest(std::string const& body);
	std::string getReasonForError();
	std::string& getBodyResponse();
};

