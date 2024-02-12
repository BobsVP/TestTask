#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>
#include<iomanip>
#include<locale>
#include<codecvt>

const std::string FileSportsmens = "competitors2.json";
const std::string FileResultsRun = "results_RUN.txt";
const std::string FileResultsOutput = "final_results.json";

struct timer
{
	int hour = 0;
	int minutes = 0;
	float seconds = 0.0;
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
void outputResultsScr(Sptmn& Sportmens, const std::vector<int>& Keys); //Вывод результатов на консоль
void outputResultsFile(const std::string& File, Sptmn& Sportmens, const std::vector<int>& Keys); //Вывод результатов в файл
void Shapka(); // Печатаем шапку таблицы
void PromegStr(); // Печатаем промежуточные элементы таблицы
void ZaverTab(); // Печатаем завершение таблицы
bool operator<(const timer& arg1, const timer& arg2) {	//Для сортировки результатов
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

int main() {
	setlocale(LC_ALL, ".UTF8");
	Sptmn Sportmens;
	std::vector<int> Keys;
	parserFileSportsmens(FileSportsmens, Sportmens);
	parserFileResults(FileResultsRun, Sportmens);
	for (auto Z : Sportmens) {
		Keys.push_back(Z.first);
	}
	std::sort(Keys.begin(), Keys.end(), 
		[&Sportmens](int a, int b) {return Sportmens[a].result < Sportmens[b].result; });
	outputResultsScr(Sportmens, Keys);
	outputResultsFile(FileResultsOutput, Sportmens, Keys);
	return 0;
}

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens) {  //Парсим файл с данными спортсменов
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
					name = line.substr(13, line.rfind("\"") - 13); // 13 позиция начала поля Name 
				}
				if (line.find("Surname") != std::string::npos) { 
					surname = line.substr(16, line.rfind("\"") - 16);   // 16 позиция начала поля Surname
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

void parserFileResults(const std::string& File, Sptmn& Sportmens) { //Парсим файл с результатами
	std::ifstream in(File);
	if (in.is_open()) {
		unsigned char B, O, M; //Проверка на BOM
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
					Sportmens[sign].result.seconds = std::stof(seconds) - Sportmens[sign].result.seconds;
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

void outputResultsScr(Sptmn& Sportmens, const std::vector<int>& Keys) {
	Shapka();
	PromegStr();
	//int count = 0;
	for (int i = 0; i < Keys.size(); ++i) {
		int Z = Keys[i];
		std::wcout << L"\x2502";
		std::cout << std::setw(13) << std::left << i + 1;
		std::wcout << L"\x2502";
		std::cout << std::setw(15) << std::left << Z;
		std::wcout << L"\x2502";
		unsigned char zzzz = Sportmens[Z].Surname.c_str()[0];
		unsigned char zzzz1 = Sportmens[Z].Surname.c_str()[1];
		int k = Sportmens[Z].Surname.size() + (11 - Sportmens[Z].Surname.size() / 2);
		std::cout << std::setw(k) << std::left << Sportmens[Z].Surname;
		std::wcout << L"\x2502";
		std::cout << std::setw(14) << std::left << Sportmens[Z].Name << "\t";
		std::wcout << L"\x2502";
		std::cout << std::setfill('0') << std::setw(2) << std::right << Sportmens[Z].result.minutes << ":" << std::setfill(' ');
		std::string msek = std::to_string(Sportmens[Z].result.seconds);
		if ((int)Sportmens[Z].result.seconds < 10) msek = '0' + msek;
		msek = msek.substr(0, 5);
		std::cout << msek;
		std::wcout << L" \x2502" << std::endl;
		if (i < Keys.size() - 1) PromegStr();
		//++count;
		//if (count > 24)
		//{
		//	count = 0;
		//	system("pause");
		//}
	}
	ZaverTab();
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

void outputResultsFile(const std::string& File, Sptmn& Sportmens, const std::vector<int>& Keys) {
	std::wofstream out(File);
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	if (out.is_open()) {
		out.imbue(utf8_locale);
		wchar_t BOM = { 0xFEFF };
		out << BOM;  //Добавляем BOM
		out << "{\n";
		for (int i = 0; i < Keys.size(); ++i) {
			int Z = Keys[i];
			out << "    \"" << i + 1 << "\": {\n";
			out << "        \"";
			out << L"Нагрудный номер\": \"" << Z << "\",\n";
			out << "        \"";
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wstr = converter.from_bytes(Sportmens[Z].Surname.c_str());
			out << L"Имя\": \"" << wstr << "\",\n";
			out << "        \"";
			wstr = converter.from_bytes(Sportmens[Z].Name.c_str());
			out << L"Фамилия\": \"" << wstr << "\",\n";
			out << "        \"";
			std::string msek = std::to_string(Sportmens[Z].result.seconds);
			if ((int)Sportmens[Z].result.seconds < 10) msek = '0' + msek;
			msek = msek.substr(0, 5);
			wstr = converter.from_bytes(msek.c_str());
			out << L"Результат\": \""; 
			if (Sportmens[Z].result.minutes < 10) out << '0';
			out << Sportmens[Z].result.minutes << ":" << wstr << "\"\n";
			out << "    },\n";
		}
		out << "}";
	}
	out.close();
}