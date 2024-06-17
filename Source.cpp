#include<locale>
#include"Header.h"

const std::string FileSportsmens = "competitors2.json";
const std::string FileResultsRun = "results_RUN.txt";
const std::string FileResultsOutput = "final_results.json";

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

