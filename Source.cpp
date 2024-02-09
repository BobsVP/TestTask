#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>

const std::string FileSportsmens = "competitors2.json";
const std::string FileResultsRun = "results_RUN.txt";

struct timer
{
	int hour = 0;
	int minutes = 0;
	int seconds = 0;
	int miliseconds = 0;
};

struct Sportsmen
{
	std::string Name;
	std::string Surname;
	timer result;
};

using Sptmn = std::map<int, Sportsmen>;

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens); //Парсим файл с данными спортсменов
void parserFileResults(const std::string& File, Sptmn& Sportmens); //Парсим файл с результатами

int main() {
	setlocale(LC_ALL, ".UTF8");
	Sptmn Sportmens;
	parserFileSportsmens(FileSportsmens, Sportmens);
	parserFileResults(FileResultsRun, Sportmens);
	int count = 0;
	for (auto Z : Sportmens) {
		std::cout << Z.first << "\t" << Z.second.Name << "\t" << Z.second.Surname << std::endl;
		std::cout << Z.second.result.hour << ":" << Z.second.result.minutes << ":" << Z.second.result.seconds << "," << Z.second.result.miliseconds << std::endl;
		++count;
		if (count > 9)
		{
			count = 0;
			system("pause");
		}
	}
	return 0;
}

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens) {
	std::ifstream in(File);
	if (in.is_open()) {

		unsigned char B, O, M; //Проверка на BOM
		B = in.get();
		O = in.get();
		M = in.get();
		if (B != 0xEF || O != 0xBB || M != 0xBF) {
			in.seekg(0, std::ios_base::beg);
		}

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
		std::cerr << "Failed to open file " << File << std::endl;;
	in.close();
}

void parserFileResults(const std::string& File, Sptmn& Sportmens)
{
	std::ifstream in(File);
	if (in.is_open()) {
		unsigned char B, O, M; //Проверка на BOM
		B = in.get();
		O = in.get();
		M = in.get();
		if (B != 0xEF || O != 0xBB || M != 0xBF) {
			in.seekg(0, std::ios_base::beg);
		}
		int sign = 0, hour = 0, minutes = 0, seconds = 0, miliseconds = 0;
		const int sec_per_min = 60;
		std::string startorend;
		while (!in.eof()) {
			in >> sign >> startorend >> hour >> B >> minutes >> B >> seconds >> B >> miliseconds;
			if (in.fail()) break;
			if (Sportmens.count(sign)) {
				if (startorend == "start") {
					Sportmens[sign].result.hour = hour;
					Sportmens[sign].result.minutes = minutes;
					Sportmens[sign].result.seconds = seconds;
					Sportmens[sign].result.miliseconds = miliseconds;
				}
				if (startorend == "finish") {
					Sportmens[sign].result.hour = hour - Sportmens[sign].result.hour;
					Sportmens[sign].result.minutes = minutes - Sportmens[sign].result.minutes;
					Sportmens[sign].result.seconds = seconds - Sportmens[sign].result.seconds;
					Sportmens[sign].result.miliseconds = miliseconds - Sportmens[sign].result.miliseconds;
					if (Sportmens[sign].result.seconds < 0) {
						--Sportmens[sign].result.minutes;
						Sportmens[sign].result.seconds += sec_per_min;
					}
					if (Sportmens[sign].result.minutes < 0) {
						--Sportmens[sign].result.hour;
						Sportmens[sign].result.minutes += sec_per_min;
					}
				}
			}
			else
				std::cerr << "Sportsman " << sign << " not registered" << std::endl;
		}
	}
	else
		std::cerr << "Failed to open file " << File << std::endl;;
	in.close();
}
