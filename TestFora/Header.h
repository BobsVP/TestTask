#pragma once

#include<string>
#include<map>
#include<vector>
#include<algorithm>

struct timer
{
	int hour = 0;
	int minutes = 0;
	float seconds = 0.0;
};

struct Sportsmen
{
	std::wstring Name;
	std::wstring Surname;
	std::wstring Resultat{ L"00:00,00" };
	timer result;
};

using Sptmn = std::map<int, Sportsmen>;

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens, std::vector<int>& Keys); //Парсим файл с данными спортсменов
void parserFileResults(const std::string& File, Sptmn& Sportmens); //Парсим файл с результатами
void outputResultsScr(Sptmn& Sportmens, const std::vector<int>& Keys); //Выводим результаты на экран
void outputResultsFile(const std::string& File, Sptmn& Sportmens, const std::vector<int>& Keys); // Выводим результаты в файл
void Shapka(); // Печатаем шапку оформления таблицы
void PromegStr(); // Печать разделителей таблицы
void ZaverTab(); // Печать завершающей строки таблицы
bool operator<(const timer& arg1, const timer& arg2); 	//Для сортировки результатов
