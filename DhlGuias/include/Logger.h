#pragma once
#include <string>
#include <fstream>
#include <filesystem>
struct Logger
{
	static void LogError(std::string const& filename, std::string const& error) {
		auto m{ std::filesystem::path(filename.c_str()).stem().string() };
		m += ".err";
		std::ofstream os(m);
		os << error << std::endl;
		os.close();
	}
	static void LogAirbill(std::string const& directory, std::string const& filename,
		std::string const& airbill) {

		std::filesystem::path p{ directory };
		p += "historial.air";
		std::ofstream os;
		os.open(p, std::ios::app);
		std::string f = std::filesystem::path(filename).stem().string();
		os << f << "-" << airbill << std::endl;
		os.close();
	}
};

