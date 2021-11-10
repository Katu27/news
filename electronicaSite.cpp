#include "electronicaSite.h"
#include <SFML/Network.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>

std::string url = "http://www.electronica.pub.ro";

sf::Http http;
sf::Http::Request request;
sf::Http::Response response;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int electronicaSiteTypeDef::start(){
	//setup:
	http.setHost(url);
	request.setHttpVersion(1, 1);
	request.setMethod(sf::Http::Request::Get);
	
	//request:
	request.setUri("/index.php/anunturi");

	response = http.sendRequest(request);

	//response:
	std::ofstream file("response.html");

	file << "status: " << response.getStatus() << std::endl;
	file << "HTTP version: " << response.getMajorHttpVersion() << "." << response.getMinorHttpVersion() << std::endl; 
	file << "Content-Type header: " << response.getField("Content-Type") << std::endl;
	file << "body: " << response.getBody() << std::endl;
	
	file.close();

	//Processing:
	processResponse();

	return 0; //everything worked!
}

int electronicaSiteTypeDef::processResponse(){
	std::ifstream file("response.html");
	std::string content, url, baseUrl = "http://www.electronica.pub.ro";
	int ok = 0;

	while(file >> content){
		if(content == "class=\"item" && ok < 2){
			ok++;
		}
		else if(content == "class=\"item" && ok == 2){
			while(file >> content){
				if(content.find("href=") != std::string::npos){
					url = content;
					while(file >> content){
						if(content.find("href=") != std::string::npos){
							break;
						}
					}
				}
				else if(content.find("class=\"modified\"") != std::string::npos){
					file >> content; //Actualizat

					SetConsoleTextAttribute(hConsole, 10);

					file >> content; //Ziua
					std::cout << content << " ";
					
					file >> content; //Data
					std::cout << content << " ";

					file >> content; //Luna
					std::cout << content << " ";

					file >> content; //An
					std::cout << content << " ";

					file >> content; //Ora
					std::cout << content << " ";


					url = url.substr(6, url.size() - 6);					
					url.pop_back();
					url.pop_back();
					
					SetConsoleTextAttribute(hConsole, 15);
					std::cout <<  std::endl << baseUrl  << url << std::endl << std::endl;
					
					break;
				}
			}
		}
	}

	file.close();
	

	return 0; //everything worked!	
}
