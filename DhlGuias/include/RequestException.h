#pragma once
#include <string>
#include <exception>
struct RequestException: public std::exception{
	std::string message;
	RequestException(std::string const& errorMessage) : message{errorMessage} {}
	const char* what()const throw() {
		return message.c_str();
	}
};
