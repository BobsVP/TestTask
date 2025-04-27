#include<locale>
#include"Header.h"

const std::string FileSportsmens = "competitors2.json";
const std::string FileResultsRun = "results_RUN.txt";
const std::string FileResultsOutput = "final_results.json";

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::vector<Sportsmen> Participants;
	parserFileSportsmens(FileSportsmens, Participants);			// Парсим файл с данными спортсменов
	parserFileResults(FileResultsRun, Participants);			// Парсим файл с результатами
	std::sort(Participants.begin(), Participants.end(),			// Сортировка результатов
		[](auto a, auto b) {return a.result < b.result; });
	outputResultsScr(Participants);								// Вывод результатов на экран
	outputResultsFile(FileResultsOutput, Participants);			// Вывод результатов в файл
	return 0;
}

