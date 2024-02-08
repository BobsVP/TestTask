#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>

const std::string FileSportsmens = "competitors2.json";
const std::string FileResultsRun = "results_RUN.txt";

struct Sportsmen
{
	std::string Name;
	std::string Surname;
};

using Sptmn = std::map<int, Sportsmen>;

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens); //Парсим файл с данными спортсменов

int main() {
	setlocale(LC_ALL, ".UTF8");
	Sptmn Sportmens;
	parserFileSportsmens(FileSportsmens, Sportmens);
	return 0;
}

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens) {
	std::ifstream in(FileSportsmens);
	if (in.is_open()) {
		bool flag = false;
		std::string sign, name, surname;
		while (!in.eof()) {
			std::string line;
			std::getline(in, line);
			long long poziciya = line.find("\"");
			if (poziciya == std::string::npos) continue;
			else if (poziciya == 4) {	//Позиция для Name, Surname
				if (line.find("Name") != std::string::npos) { 
					name = line.substr(13, line.rfind("\"") - 13);
				}
				if (line.find("Surname") != std::string::npos) { 
					surname = line.substr(16, line.rfind("\"") - 16);
					flag = true;
				}
			}
			else if (poziciya == 2) {	//Позиция для номера
				for (auto simb : line) {
					if (isdigit(simb)) sign += simb;
				}
				std::cout << std::endl;
			}
			if (flag)
			{
				Sportmens[std::stoi(sign)] = Sportsmen{ name, surname };
				flag = false;
				sign.clear();
				name.clear();
				surname.clear();
			}
		}
	}
	else
		std::cerr << "Failed to open file\n";
	in.close();

}
