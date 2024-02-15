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
