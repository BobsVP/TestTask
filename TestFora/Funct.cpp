#include<iostream>
#include<fstream>
#include<iomanip>
#include<codecvt>
#include"Header.h"

bool operator<(const timer& arg1, const timer& arg2) {	//Сортировка результатов
	if (arg1.hour == 0 && arg1.minutes == 0 && (int)arg1.seconds == 0) return false;
	if (arg2.hour == 0 && arg2.minutes == 0 && (int)arg2.seconds == 0) return true;
	if (arg1.minutes == arg2.minutes)
	{
		if (arg1.seconds < arg2.seconds) {
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (arg1.minutes > arg2.minutes)
	{
		return false;
	}
	return true;
}


void parserFileSportsmens(const std::string& File, Sptmn& Sportmens, std::vector<int>& Keys) {  //Парсим файл с данными спортсменов
	std::ifstream in(File);
	if (in.is_open()) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		unsigned char B, O, M; //Проверка BOM
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
			else if (poziciya == 4) {	// Name, Surname
				if (line.find("Name") != std::string::npos) {
					name = line.substr(13, line.rfind("\"") - 13); // 13 Позиция начала поля Name 
				}
				if (line.find("Surname") != std::string::npos) {
					surname = line.substr(16, line.rfind("\"") - 16);   // 16 Позиция начала поля Surname
					flag = true;
				}
			}
			else if (poziciya == 2) {	//Sign
				for (auto simb : line) {
					if (isdigit(simb)) sign += simb;
				}
			}
			if (flag)
			{
				int key = std::stoi(sign);
				Keys.push_back(key);
				Sportmens[key] = 
					Sportsmen{ converter.from_bytes(name.c_str()), converter.from_bytes(surname.c_str()) };
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

void parserFileResults(const std::string& File, Sptmn& Sportmens) { //Парсим файл с результатами
	std::ifstream in(File);
	if (in.is_open()) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		unsigned char B, O, M; //Проверка BOM
		B = in.get();
		O = in.get();
		M = in.get();
		if (B != 0xEF || O != 0xBB || M != 0xBF) {
			in.seekg(0, std::ios_base::beg);
		}
		int sign = 0, hour = 0, minutes = 0;
		const int sec_per_min = 60;
		std::string startorend, seconds;
		while (!in.eof()) {
			in >> sign >> startorend >> hour >> B >> minutes >> B >> seconds;
			if (in.fail()) break;
			if (Sportmens.count(sign)) {
				if (startorend == "start") {
					Sportmens[sign].result.hour = hour;
					Sportmens[sign].result.minutes = minutes;
					Sportmens[sign].result.seconds = std::stof(seconds);
				}
				if (startorend == "finish") {
					Sportmens[sign].result.hour = hour - Sportmens[sign].result.hour;
					Sportmens[sign].result.minutes = minutes - Sportmens[sign].result.minutes;
					float varszpt = std::stof(seconds);
					std::replace(seconds.begin(), seconds.end(), ',', '.');
					float varstchk = std::stof(seconds);
					if (varszpt > varstchk)
						Sportmens[sign].result.seconds = varszpt - Sportmens[sign].result.seconds;
					else
						Sportmens[sign].result.seconds = varstchk - Sportmens[sign].result.seconds;
					if (Sportmens[sign].result.seconds < 0) {
						--Sportmens[sign].result.minutes;
						Sportmens[sign].result.seconds += sec_per_min;
					}
					if (Sportmens[sign].result.minutes < 0) {
						--Sportmens[sign].result.hour;
						Sportmens[sign].result.minutes += sec_per_min;
					}
					if ((int)Sportmens[sign].result.minutes < 10)
						Sportmens[sign].Resultat = L'0' + std::to_wstring(Sportmens[sign].result.minutes) + L":";
					else
						Sportmens[sign].Resultat = std::to_wstring(Sportmens[sign].result.minutes) + L":";
					if ((int)Sportmens[sign].result.seconds < 10)
						Sportmens[sign].Resultat += (L'0' + std::to_wstring(Sportmens[sign].result.seconds));
					else
						Sportmens[sign].Resultat += std::to_wstring(Sportmens[sign].result.seconds);
					Sportmens[sign].Resultat = Sportmens[sign].Resultat.substr(0, 8);
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

void outputResultsScr(Sptmn& Sportmens, const std::vector<int>& Keys) {
	Shapka();
	PromegStr();
	for (size_t i = 0; i < Keys.size(); ++i) {
		int sign = Keys[i];
		std::wcout << L"\x2502";
		std::wcout << std::setw(13) << std::left << i + 1;
		std::wcout << L"\x2502";
		std::wcout << std::setw(15) << std::left << sign;
		std::wcout << L"\x2502";
		std::wcout << std::setw(11) << std::left << Sportmens[sign].Surname;
		std::wcout << L"\x2502";
		std::wcout << std::setw(11) << std::left << Sportmens[sign].Name << "\t";
		std::wcout << L"\x2502";
		std::wcout << std::setw(8) << std::left << Sportmens[sign].Resultat;
		std::wcout << L" \x2502" << std::endl;
		if (i < Keys.size() - 1) PromegStr();
	}
	ZaverTab();
}

void outputResultsFile(const std::string& File, Sptmn& Sportmens, const std::vector<int>& Keys) {
	std::wofstream out(File);
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	if (out.is_open()) {
		out.imbue(utf8_locale);
		wchar_t BOM = { 0xFEFF };
		out << BOM;  //Добавляем BOM
		out << "{\n";
		for (int i = 0; i < Keys.size(); ++i) {
			int sign = Keys[i];
			out << "    \"" << i + 1 << "\": {\n";
			out << "        \"";
			out << L"Нагрудный номер\": \"" << sign << "\",\n";
			out << "        \"";
			out << L"Имя\": \"" << Sportmens[sign].Surname << "\",\n";
			out << "        \"";
			out << L"Фамилия\": \"" << Sportmens[sign].Name << "\",\n";
			out << "        \"";
			std::string msek = std::to_string(Sportmens[sign].result.seconds);
			if ((int)Sportmens[sign].result.seconds < 10) msek = '0' + msek;
			msek = msek.substr(0, 5);
			out << L"Результат\": \"";
			out << Sportmens[sign].Resultat << "\"\n";
			if (i < Keys.size() - 1) out << "    },\n";
			else out << "    }\n";
		}
		out << "}";
	}
	out.close();
}