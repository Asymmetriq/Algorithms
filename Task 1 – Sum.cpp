#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	string line;
	long p = 0;
	int i = 0;
	int b, e;
	long long sum = 0;
	while (getline(cin, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			// В строчке условия ниже раньше использовался isdigit, но с ним код работал больше 1с,
			// что фейлило 10-ый тест. В связи с этим заменил на сравнение (line[i] >= '0') && (line[i] <= '9')
			if (((line[i] >= '0') && (line[i] <= '9')) 
				|| line[i] == '-' && ((line[i+1] >= '0') && (line[i+1] <= '9'))) // для отрицательных
			{
				b = i; // отмечаем начало 
				while (isdigit(line[++i])) 
				{
				}
				e = i; // отмечаем конец
				p = stoi(line.substr(b, e - b)); // вырезаем кусок
				sum += p;
			}
		}
	}
	cout << sum << endl;
	return 0;
}