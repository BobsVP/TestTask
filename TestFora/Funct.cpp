#include <iostream>
#include <fstream>
#include <iomanip>
#include <codecvt>
#include <regex>
#include "Header.h"

bool operator<(const timer& arg1, const timer& arg2) {	//Сортировка результатов
	if (arg1.seconds == 0 && arg1.miliseconds == 0) return false;
	if (arg2.seconds == 0 && arg2.miliseconds == 0) return true;
	if (arg1.seconds == arg2.seconds)
	{
		if (arg1.miliseconds < arg2.miliseconds) {
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (arg1.seconds > arg2.seconds)
	{
		return false;
	}
	return true;
}

std::wstring extractData(const std::wstring& input) { // Извлекаем содержимое между кавычками
	size_t end = input.rfind(L'"');
	if (end == std::wstring::npos) {
		return L"";
	}

	size_t start = input.rfind(L'"', end - 1);
	if (start == std::wstring::npos) {
		return L"";
	}

	return input.substr(start + 1, end - start - 1);
}

void parserFileSportsmens(const std::string& File, std::vector<Sportsmen>& Participants) {  //Парсим файл с данными спортсменов
	
	std::wifstream in(File);

	if (!in.is_open()) {
		std::cerr << "Failed to open file " << File << std::endl;;
	}

	in.imbue(std::locale("ru_RU.UTF-8"));

	int sign;
	std::wstring name, surname, line;

	std::getline(in, line);

	while (!in.eof()) {
		std::getline(in, line);
		std::wstring tmp;
		if (line.find(L"{") != std::string::npos) {
			std::copy_if(line.cbegin(), line.cend(), std::back_inserter(tmp), ::isdigit);
			sign = std::stoi(tmp);
		}
		if (line.find(L"Name") != std::string::npos) name = extractData(line);
		if (line.find(L"Surname") != std::string::npos) {
			surname = extractData(line);
			Participants.push_back(Sportsmen{ sign, name, surname });
		}
	}
	in.close();
}

void parserFileResults(const std::string& File, std::vector<Sportsmen>& Participants) { //Парсим файл с результатами
	std::map<int, int> Position;
	for (size_t i = 0; i < Participants.size(); ++i) {
		Position[Participants[i].Sign] = i;
	}

	std::ifstream in(File);
	if (!in.is_open()) {
		std::cerr << "Failed to open file " << File << std::endl;
		return;
	}

	const int sec_per_min = 60;
	const int sec_per_hour = sec_per_min * 60;
	std::string line;
	std::getline(in, line);

	const std::regex regular("(\\d+)\\s+(\\w+)\\s+(\\d+):(\\d+):(\\d+),(\\d+)"); //Регулярное выражение для разделения строки

	while (!in.eof()) {
		std::smatch result;
		if (line.find(":") != std::string::npos) {
			for (auto i = std::sregex_iterator(line.begin(), line.end(), regular); i != std::sregex_iterator(); ++i) { //Выделяю нужные части строки
				result = *i;
				int sign = std::stoi(result[1]);
				if (Position.count(sign)) sign = Position[sign];
				else {
					std::cerr << "Sportsman " << sign << " not registered" << std::endl;
					break;
				}
				if (result[2] == "start") {
					Participants[sign].result.seconds = -(std::stoi(result[3]) * sec_per_hour);
					Participants[sign].result.seconds -= (std::stoi(result[4]) * sec_per_min);
					Participants[sign].result.seconds -= std::stoi(result[5]);
					Participants[sign].result.miliseconds = -std::stoi(result[6]);
				}
				else {
					Participants[sign].result.seconds += (std::stoi(result[3]) * sec_per_hour);
					Participants[sign].result.seconds += (std::stoi(result[4]) * sec_per_min);
					Participants[sign].result.seconds += std::stoi(result[5]);
					Participants[sign].result.miliseconds += std::stoi(result[6]);
					int hour = Participants[sign].result.seconds / sec_per_hour;
					int minutes = (Participants[sign].result.seconds / sec_per_min) - (hour * sec_per_min);
					int seconds = Participants[sign].result.seconds - (minutes * sec_per_min) - (hour * sec_per_min);
					int milseconds = (Participants[sign].result.miliseconds + 5000) / 10000;
					if (minutes < 10)
						Participants[sign].Resultat = L'0' + std::to_wstring(minutes) + L":";
					else
						Participants[sign].Resultat = std::to_wstring(minutes) + L":";
					if (seconds < 10)
						Participants[sign].Resultat += (L'0' + std::to_wstring(seconds));
					else
						Participants[sign].Resultat += std::to_wstring(seconds);
					if (milseconds < 10)
						Participants[sign].Resultat += (L",0" + std::to_wstring(milseconds));
					else
						Participants[sign].Resultat += (L',' + std::to_wstring(milseconds));
				}
			}
		}
		std::getline(in, line);
	}
	in.close();
}

void Shapka() {
	std::wcout << L"\x250C";
	for (int i = 0; i < 13; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x252C";
	for (int i = 0; i < 15; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x252C";
	for (int i = 0; i < 11; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x252C";
	for (int i = 0; i < 13; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x252C";
	for (int i = 0; i < 9; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2510" << std::endl;
	std::wcout << std::setw(14) << std::left << L"\x2502Занятое место";
	std::wcout << std::setw(15) << std::left << L"\x2502Нагрудный номер";
	std::wcout << std::setw(12) << std::left << L"\x2502Имя";
	std::wcout << std::setw(14) << std::left << L"\x2502Фамилия";
	std::wcout << std::setw(15) << std::left << L"\x2502Результат\x2502" << std::endl;
}

void PromegStr() {
	std::wcout << L"\x251C";
	for (int i = 0; i < 13; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x253C";
	for (int i = 0; i < 15; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x253C";
	for (int i = 0; i < 11; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x253C";
	for (int i = 0; i < 13; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x253C";
	for (int i = 0; i < 9; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2524" << std::endl;
}

void ZaverTab() {
	std::wcout << L"\x2514";
	for (int i = 0; i < 13; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2534";
	for (int i = 0; i < 15; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2534";
	for (int i = 0; i < 11; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2534";
	for (int i = 0; i < 13; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2534";
	for (int i = 0; i < 9; ++i)std::wcout << L"\x2500";
	std::wcout << L"\x2518" << std::endl;
}

void outputResultsScr(std::vector<Sportsmen>& Participants) {
	Shapka();
	PromegStr();
	for (size_t i = 0; i < Participants.size(); ++i) {
		std::wcout << L"\x2502";
		std::wcout << std::setw(13) << std::left << i + 1;
		std::wcout << L"\x2502";
		std::wcout << std::setw(15) << std::left << Participants[i].Sign;
		std::wcout << L"\x2502";
		std::wcout << std::setw(11) << std::left << Participants[i].Surname;
		std::wcout << L"\x2502";
		std::wcout << std::setw(11) << std::left << Participants[i].Name << "\t";
		std::wcout << L"\x2502";
		std::wcout << std::setw(8) << std::left << Participants[i].Resultat;
		std::wcout << L" \x2502" << std::endl;
		if (i < Participants.size() - 1) PromegStr();
	}
	ZaverTab();
}

void outputResultsFile(const std::string& File, std::vector<Sportsmen>& Participants) {
	std::wofstream out(File);
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	if (out.is_open()) {
		out.imbue(utf8_locale);
		wchar_t BOM = { 0xFEFF };
		out << BOM;  //Добавляем BOM
		out << "{\n";
		for (int i = 0; i < Participants.size(); ++i) {
			out << "    \"" << i + 1 << "\": {\n";
			out << "        \"";
			out << L"Нагрудный номер\": \"" << Participants[i].Sign << "\",\n";
			out << "        \"";
			out << L"Имя\": \"" << Participants[i].Surname << "\",\n";
			out << "        \"";
			out << L"Фамилия\": \"" << Participants[i].Name << "\",\n";
			out << "        \"";
			out << L"Результат\": \"";
			out << Participants[i].Resultat << "\"\n";
			if (i < Participants.size() - 1) out << "    },\n";
			else out << "    }\n";
		}
		out << "}";
	}
	out.close();
}