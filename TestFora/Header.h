#pragma once

#include<string>
#include<map>
#include<vector>
#include<algorithm>

struct timer
{
	long seconds = 0;
	long miliseconds = 0;
};

struct Sportsmen
{
	int Sign = -1;
	std::wstring Name;
	std::wstring Surname;
	std::wstring Resultat{ L"00:00,00" };
	timer result;
};

void parserFileSportsmens(const std::string& File, std::vector<Sportsmen>& Participants);	// Парсим файл с данными спортсменов
void parserFileResults(const std::string& File, std::vector<Sportsmen>& Participants);		// Парсим файл с результатами
void outputResultsScr(std::vector<Sportsmen>& Participants);								// Выводим результаты на экран
void outputResultsFile(const std::string& File, std::vector<Sportsmen>& Participants);		// Выводим результаты в файл
bool operator<(const timer& arg1, const timer& arg2); 										// Для сортировки результатов
