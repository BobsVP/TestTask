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
	if (input.empty()) { // Проверка на пустую строку
		return L"";
	}
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
		std::cerr << "Failed to open file " << File << std::endl;
		return;
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

void Fillers(const std::vector<size_t>& columns, const wchar_t symb, const wchar_t symb1) {
	for (const auto X : columns) {
		for (size_t i = 0; i < X; ++i) std::wcout << symb;
		std::wcout << symb1;
	}
	std::wcout << '\b';
}

template<typename T>
void Fillers_data(const size_t colm, T str) {
	std::wcout << L'\x2502' << std::setw(colm) << std::left << str;
}

void outputResultsScr(std::vector<Sportsmen>& Participants) {
	std::wstring o_place = L"Занятое место";							// Определяем ширину колонок
	size_t o_place_legth = o_place.size();
	std::wstring b_number = L"Нагрудный номер";
	size_t b_number_legth = b_number.size();
	std::wstring Surname = L"Имя";
	size_t Surname_legth = Surname.size();
	std::wstring Name = L"Фамилия";
	size_t Name_legth = Name.size();
	std::wstring Result = L"Результат";
	size_t Result_legth = Result.size();
	for (const auto X : Participants) {
		Surname_legth = std::max(X.Surname.size(), Surname_legth);
		Name_legth = std::max(X.Name.size(), Name_legth);
		Result_legth = std::max(X.Resultat.size(), Result_legth);
	}
	std::vector<size_t> columns{ o_place_legth, b_number_legth, Surname_legth, Name_legth, Result_legth };

	std::wcout << L'\x250C';											// Печатаем шапку таблицы
	Fillers(columns, L'\x2500', L'\x252C');
	std::wcout << L'\x2510' << std::endl;
	Fillers_data(o_place_legth, o_place);
	Fillers_data(b_number_legth, b_number);
	Fillers_data(Surname_legth, Surname);
	Fillers_data(Name_legth, Name);
	Fillers_data(Result_legth, Result);
	std::wcout << L'\x2502' << std::endl;
	std::wcout << L'\x251C';
	Fillers(columns, L'\x2500', L'\x253C');
	std::wcout << L'\x2524' << std::endl;

	for (size_t i = 0; i < Participants.size(); ++i) {					// Печатаем таблицу
		Fillers_data(o_place_legth, i + 1);
		Fillers_data(b_number_legth, Participants[i].Sign);
		Fillers_data(Surname_legth, Participants[i].Surname);
		Fillers_data(Name_legth, Participants[i].Name);
		Fillers_data(Result_legth, Participants[i].Resultat);
		std::wcout << L'\x2502' << std::endl;
		if (i < Participants.size() - 1) {
			std::wcout << L'\x251C';
			Fillers(columns, L'\x2500', L'\x253C');
			std::wcout << L'\x2524' << std::endl;
		}
	}
	std::wcout << L'\x2514';
	Fillers(columns, L'\x2500', L'\x2534');
	std::wcout << L'\x2518' << std::endl;
}

void outputResultsFile(const std::string& File, std::vector<Sportsmen>& Participants) {
	std::wofstream out(File);
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	if (!out.is_open()) {
		std::cerr << "Failed to open file " << File << std::endl;
		return; 
	}
	out.imbue(utf8_locale);
	out << "{\n";
	for (size_t i = 0; i < Participants.size(); ++i) {
		out << L"    \"" << i + 1 << "\": {\n";
		out << L"        \"Нагрудный номер\": \"" << Participants[i].Sign << "\",\n";
		out << L"        \"Имя\": \"" << Participants[i].Surname << "\",\n";
		out << L"        \"Фамилия\": \"" << Participants[i].Name << "\",\n";
		out << L"        \"Результат\": \"" << Participants[i].Resultat << "\"\n";
		out << L"    }" << (i + 1 < Participants.size() ? L",\n" : L"\n");
	}
	out << "}";
	out.close();
}