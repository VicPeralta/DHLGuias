// dhl.cpp
// Copyright 2021 Victor Peralta G.
// victor.peralta.gomez@gmail.com

/*
	Este programa realiza procesa un archivo XML que contiene la información
	completa para solicitar una guía de embarque a DHL, realiza la consulta al
	servidor de DHL y si los datos están correctos recibe una respuesta XML que contiene
	la información del embarque, guía, imágenes de los códigos de barras y el formato de
	la etiqueta, ya sea en ZPL o PDF
*/
#include "preCompiled.h"
#include "Requester.h"
#include "RequestException.h"
#include "Logger.h"
#include "AirbillInfo.h"
#include <filesystem>
int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cout << "Argumentos incorrectos\n";
		std::cout << "Uso:\n\tdhl [URL Servidor] [Archivo XML] [Carpeta destino]\n";
		std::cout << "Ejemplo:\n\tdhl http://xmlpitest-ea.dhl.com/XMLShippingServlet requestPDF.xml c:\\ \n";
		return 1;
	}
	Requester request{};
	try {
		auto start = std::chrono::high_resolution_clock::now();
		auto airbill = request.requestAirBill(argv[1], argv[2], argv[3]);
		std::cout << "Airbill number: " << airbill.number << "\n";
		std::cout << "File label: " << airbill.filename << "\n";
		std::cout << "Format: " << airbill.format << "\n";
		Logger::LogAirbill(argv[3], argv[2], airbill.number);
		std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - start).count();
		std::cout << " ms\n";

	}
	catch (RequestException& ex) {
		Logger::LogError(argv[2], ex.what());
		std::cout << ex.what() << "\n";
	}
	catch (...) {
		std::cout << "Error inesperado..\n";
	}
	return 0;
}