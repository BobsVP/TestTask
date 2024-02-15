#include<locale>
#include"Header.h"

const std::string FileSportsmens = "competitors2.json";
const std::string FileResultsRun = "results_RUN.txt";
const std::string FileResultsOutput = "final_results.json";

void parserFileSportsmens(const std::string& File, Sptmn& Sportmens, std::vector<int>& Keys); //Парсим файл с данными спортсменов
void parserFileResults(const std::string& File, Sptmn& Sportmens); //Парсим файл с результатами
void outputResultsScr(Sptmn& Sportmens, const std::vector<int>& Keys); //Выводим результаты на экран
void outputResultsFile(const std::string& File, Sptmn& Sportmens, const std::vector<int>& Keys); // Выводим результаты в файл
void Shapka(); // Печатаем шапку оформления таблицы
void PromegStr(); // Печать разделителей таблицы
void ZaverTab(); // Печать завершающей строки таблицы
bool operator<(const timer& arg1, const timer& arg2); 	//Для сортировки результатов

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	Sptmn Sportmens;
	std::vector<int> Keys;
	parserFileSportsmens(FileSportsmens, Sportmens, Keys);
	parserFileResults(FileResultsRun, Sportmens);
	std::sort(Keys.begin(), Keys.end(), 
		[&Sportmens](int a, int b) {return Sportmens[a].result < Sportmens[b].result; });
	outputResultsScr(Sportmens, Keys);
	outputResultsFile(FileResultsOutput, Sportmens, Keys);
	return 0;
}

