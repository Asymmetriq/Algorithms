#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>

// 31-ое число Марсенна
const unsigned long M = 2147483647;
// Массив простых чисел
const unsigned long long Primes[] =
{
	0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
	89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
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
	size_t getHash(const T& key, const size_t& hashIndex);

public:
	Filter() {};
	~Filter() {};

	void Set(const size_t& n, const size_t& p);
	void Add(const T& key);
	std::string Print();
	bool Search(const T& key);
};

template <class T>
void Filter<T>::Set(const size_t& n, const size_t& l)
{
	size = n;
	numHashes = l;
	bits.resize(size);
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
std::string Filter<T>::Print()
{
	std::string out;
	for (size_t i = 0; i < size; i++)
	{
		if (bits[i])
			out += "1";
		else
			out += "0";
	}
	return out;
}

// Семейство хэш-функций
template <class T>
size_t Filter<T>::getHash(const T& key, const size_t& hashIndex)
{
	return (((hashIndex + 1) * (key % M) + Primes[hashIndex + 1]) % M) % size;
}

// Чтение всех данных в строку
std::string Read()
{
	std::string data;
	std::getline(std::cin, data, '\0');
	return data + "\n";
}

// Расчёт параметров
std::pair<size_t, size_t> Calculate(const int& n, const double& prob)
{
	size_t size = round((-n * log2(prob)) / log(2));
	size_t numb = round(-log2(prob));
	return { size, numb };
}

int main()
{
	std::istringstream is(Read());
	std::ostringstream os;

	Filter<uint64_t> filter;

	std::string command;
	uint64_t element;
	double prob;
	bool setFlag = false; // флаг показывает, задавался ли размер методом set()
	bool empty = false; // флаг показывает, пуста ли поданная в поток ввода строка

	while (is >> command)
	{
		if (command.empty())
		{
			empty = true;
		}
		if (command == "set" && is.peek() != '\n'
			&& is >> element && is.peek() != '\n'
			&& is >> prob && prob < 1 && is.peek() == '\n' && !setFlag)
		{
			std::pair<size_t, size_t> params = Calculate(element, prob);
			if (params.first != 0 && params.second != 0)
			{
				os << params.first << " " << params.second << std::endl;
				filter.Set(params.first, params.second);
				setFlag = true;
			}
			else
				os << "error" << std::endl;
		}
		else if (command == "add" && is.peek() != '\n'
			&& is >> element && is.peek() == '\n' && setFlag)
		{
			filter.Add(element);
		}
		else if (command == "search" && is.peek() != '\n'
			&& is >> element && is.peek() == '\n' && setFlag)
		{
			os << filter.Search(element) << std::endl;
		}
		else if (command == "print" && is.peek() == '\n' && setFlag)
		{
			os << filter.Print() << std::endl;
		}
		else
		{
			if (!empty) // если строка не пустая и не является ни одной из команд
				os << "error" << std::endl;
		}
	}
	std::cout << os.str();
	return 0;
}