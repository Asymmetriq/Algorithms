#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

// 31-ое число Марсенна
const unsigned long M = 2147483647; 
// Массив простых чисел
const unsigned long long Primes[] =
{
	0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 
	89, 97, 101, 103, 107, 109, 113,	127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 
	181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 263, 269, 271, 277, 281
}; // 2 - первое простое число, считаем с элемента с индексом "1"

// Фильтр Блума
template <class T>
class Filter 
{
	std::vector<bool> bits;
	size_t size;
	size_t numHashes;

	// Семейство хэш-функций
	size_t getHash(const T& key,const size_t& hashIndex);

public:
	Filter() {};
	~Filter() {};

	bool Set(const int& n, const double& p);
	void Add(const T& key);
	void Print(); 
	bool Search(const T& key);
};

template <class T>
bool Filter<T>::Set(const int& n, const double& prob)
{
	size = round((-n * log2(prob)) / log(2));
	numHashes = round(-log2(prob));
	if (size == 0 || numHashes == 0)
	{
		std::cout << "error" << std::endl;
		return false;
	}
	else
	{
		bits.resize(size);
		std::cout << size << " " << numHashes << std::endl;
		return true;
	}
}
template <class T>
void Filter<T>::Add(const T& key)
{
	for (size_t i = 0; i < numHashes; i++)
	{
		size_t index = getHash(key, i); // Получаем индекс для каждой хэш-функции
		bits[index] = true; // Отмечаем биты по индексам 
	}
}

template <class T>
bool Filter<T>::Search(const T& key)
{
	for (size_t i = 0; i < numHashes; i++)
	{
		size_t index = getHash(key, i); // Получаем индекс для каждой хэш-функции
		if (!bits[index]) // Если бит не отмечен
		{
			return false; // Возвращаем негативный результат
		}
	}
	return true;
}

template <class T>
void Filter<T>::Print()
{
	for (size_t i = 0; i < size; i++)
	{
		std::cout << bits[i];
	}
	std:: cout << std::endl;
}

// Семейство хэш-функций
template <class T>
size_t Filter<T>::getHash(const T& key, const size_t& hashIndex)
{
	return (((hashIndex + 1) * (key % M) + Primes[hashIndex + 1]) % M) % size;
}


int main()
{
	Filter<uint64_t> filter;
	std::string command, line;
	uint64_t element;
	double prob;

	bool setFlag = false; // флаг показывает, задавался ли размер методом set()
	bool empty = false; // флаг показывает, пуста ли поданная в поток ввода строка
	while (std::getline(std::cin, line))
	{
		std::istringstream is(line);
		if (is.rdbuf()->in_avail() == 0) // проверяем на пустоту
		{
			empty = true;
		}
		command.clear();
		is >> command >> element >> prob; // сначала считывается команда
		if (command == "set")
		{
			if (!setFlag && element != 0 && prob != 0 && prob < 1)
			{
				setFlag = filter.Set(element, prob);
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else if (command == "add")
		{
			is >> element; // считывается аргумент
			if (is.rdbuf()->in_avail() == 0 && setFlag)
			{

				filter.Add(element);
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else if (command == "search")
		{
			is >> element; // считывается аргумент
			if (is.rdbuf()->in_avail() == 0 && setFlag)
			{
				std::cout << filter.Search(element) <<std::endl;
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else if (command == "print" && setFlag)
		{
			if (is.rdbuf()->in_avail() == 0) // если строка состоит только из print
			{
				filter.Print();
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else
		{
			if (!empty) // если строка не пустая и не является ни одной из команд
				std::cout << "error" << std::endl;
		}
	}
	return 0;
}
