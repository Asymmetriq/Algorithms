#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>

struct Element
{
	int64_t key;
	std::string value;
};

class Heap
{
	std::vector<Element> vec;
	std::unordered_map<int64_t, int> map;

	int getP(int i) { return (i - 1) / 2; }
	int getL(int i) { return (2 * i + 1); }
	int getR(int i) { return (2 * i + 2); }
	
	void siftUp(int index)
	{
		int64_t iKey = 0;
		int64_t pKey = 0;

		while (index > 0)
		{
			iKey = vec[index].key;
			pKey = vec[getP(index)].key;

			if (pKey > iKey)
			{
				std::swap(vec[index], vec[getP(index)]);
				std::swap(map[pKey], map[iKey]);
				index = getP(index);
			}
			else
				break;
		}
	}

	void siftDown(int index)
	{
		int left;
		int right;
		int smallest;

		while (getL(index) < vec.size())
		{
			smallest = index;
			left = getL(index);
			right = getR(index);

			if (left < vec.size() && vec[left].key < vec[smallest].key)
				smallest = left;
			if (right < vec.size() && vec[right].key < vec[smallest].key)
				smallest = right;
			if (smallest == index)
				break;

			std::swap(vec[index], vec[smallest]);
			std::swap(map[vec[index].key], map[vec[smallest].key]);

			index = smallest;
		}
	}

	std::string printElement(int index)
	{
		std::string result = '[' + std::to_string(vec[index].key) + ' ' + vec[index].value;
		if (index) 
			result +=  ' ' + std::to_string(vec[getP(index)].key);
		result+= ']';
		return result;
	}

public:

	Heap() {};
	~Heap() {};

	bool Add(int64_t& key, std::string& value)
	{
		if (map.count(key))
			return false;

		vec.push_back({ key, value });
		map[key] = vec.size() - 1;
		siftUp(vec.size() - 1);
		return true;
	}

	bool Set(int64_t& key, std::string& value)
	{
		if (!map.count(key))
			return false;

		vec[map[key]].value = value;
		return true;
	}

	bool Delete(int64_t& key)
	{
		if (map.count(key))
		{
			int index = map[key];
			int64_t newkey = vec.back().key;

			std::swap(vec[index], vec.back());
			std::swap(map[vec[index].key], map[vec.back().key]);

			vec.pop_back();
			map.erase(key);
			
			if (newkey < key)
				siftUp(index);
			else
				siftDown(index);
			
			return true;
		}
		return false;
	}

	std::tuple<bool, int, std::string> Search(int64_t& key)
	{
		if (!map.count(key))
			return {0, 0, ""};
		else
			return { 1, map[key], vec[map[key]].value };
	}

	std::tuple<bool, int, Element> Min()
	{
		if (vec.size())
			return { 1, 0, vec[0] };
		else
			return { 0, 0, {0,""} };
	}

	std::tuple<bool, int, Element> Max()
	{
		if (!vec.size())
			return { 0, 0, {0,""} };
		else
		{
			Element result = vec[0];
			int index = 0;
			for (size_t i = 0; i < vec.size(); i++)
			{
				if (vec[i].key > result.key)
				{
					result = vec[i];
					index = i;
				}
			}
			return { 1, index, result };
		}
	}

	std::pair<bool, Element> Extract()
	{
		if (!vec.size())
			return { 0, {0,""} };
		else
		{
			Element result = vec[0];
			map.erase(vec[0].key);
			vec[0] = vec.back();
			map[vec[0].key] = 0;
			vec.pop_back();
			siftDown(0);
			return { 1, result };
		}
	}

	std::string Print()
	{
		std::ostringstream result;
		size_t space = 0;
		size_t j = 0;

		if (!vec.size())
		{
			result << "_\n";
			return result.str();
		}

		for (size_t i = 0; i < vec.size(); i++)
		{
			result << printElement(i);
			space++;

			if (space == pow(2, j))
			{
				result << "\n";
				if (i == vec.size() - 1)
					return result.str();
				j++;
				space = 0;
			}
			else
				result << " ";
		}
		for (; space != pow(2, j) - 1; space++)
			result << "_ ";
		result << "_\n";
		return result.str();
	}
				
};


int main()
{
	// Считываем все данные в строку
	std::string data;
	std::getline(std::cin, data, '\0');
	data += '\n';

	std::istringstream is(data);
	std::ostringstream os;

	Heap heap;

	int64_t key;
	std::string command, value;

	bool empty = false; // флаг показывает, пуста ли поданная в поток ввода строка

	while (is >> command)
	{
		if (command.empty())
		{
			empty = true;
		}
		if (command == "add" && is.peek() != '\n'
			&& is >> key && is.peek() != '\n' && is >> value && is.peek() == '\n')
		{
			if (!heap.Add(key, value))
				os << "error" << std::endl;
		}
		else if (command == "set" && is.peek() != '\n'
			&& is >> key && is.peek() != '\n' && is >> value && is.peek() == '\n')
		{
			if (!heap.Set(key, value))
				os << "error" << std::endl;
		}
		else if (command == "delete" && is.peek() != '\n'
			&& is >> key && is.peek() == '\n')
		{
			if (!heap.Delete(key))
				os << "error" << std::endl;
		}
		else if (command == "search" && is.peek() != '\n'
			&& is >> key && is.peek() == '\n')
		{
			std::tuple<bool, int, std::string> result = heap.Search(key);
			if (std::get<0>(result))
			{
				os << "1 "
					<< std::get<1>(result) << " "
					<< std::get<2>(result) << std::endl;
			}
			else
				os << "0" << std::endl;
		}
		else if (command == "min" && is.peek() == '\n')
		{
			std::tuple<bool, int, Element> result = heap.Min();
			if (std::get<0>(result))
			{
				os << std::get<2>(result).key << " "
					<< std::get<1>(result) << " "
					<< std::get<2>(result).value << std::endl;
			}
			else
				os << "error" << std::endl;
		}
		else if (command == "max" && is.peek() == '\n')
		{
			std::tuple<bool, int, Element> result = heap.Max();
			if (std::get<0>(result))
			{
				os << std::get<2>(result).key << " "
					<< std::get<1>(result) << " "
					<< std::get<2>(result).value << std::endl;
			}
			else
				os << "error" << std::endl;
		}
		else if (command == "extract" && is.peek() == '\n')
		{
			std::pair<bool, Element> result = heap.Extract();
			if (result.first)
				os << result.second.key << " " 
				<< result.second.value << std::endl;
			else
				os << "error" << std::endl;
		}
		else if (command == "print" && is.peek() == '\n')
		{
			os << heap.Print();
		}
		else
		{
			if (!empty) // если строка не пустая и не является ни одной из команд
				os << "error" << std::endl;
		}
	}
	std::cout << os.str();
	return 0;
};