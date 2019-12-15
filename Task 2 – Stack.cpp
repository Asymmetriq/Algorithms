#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <map>

// Реализация стека с использованием массива
template <class T>
class Stack
{
	int size = 0;
	T* Array = nullptr;
	int top = 0;

public:
	Stack();
	// Пустой конструктор, поля задаются методом set_size
	// ( в предыдущей версии кода set_size ввозвращала готовый стэк, 
	// при этом использовался Stack(int Size) )
	~Stack()
	{
		if (Array)
			delete[] Array;
	}
	void push(T element);
	void pop();
	void print();
	void set_size(int Size); // метод, задающий поля
};
template <class T>
Stack<T>::Stack()
{}
template <class T>
void Stack<T>::push(T element)
{
	if (top >= (size - 1))
	{
		std::cout << "overflow" << std::endl;
	}
	else
	{
		Array[++top] = element;
	}
}
template <class T>
void Stack<T>::pop()
{
	if (top < 0)
	{
		std::cout << "underflow" << std::endl;
	}
	else
	{
		std::cout << Array[top--] << std::endl;
	}

}
template <class T>
void Stack<T>::print()
{
	if (top < 0)
	{
		std::cout << "empty" << std::endl;
	}
	else
	{
		int i = -1;
		while (++i < top)
		{
			std::cout << Array[i] << " ";
		}
		std::cout << Array[top];
		std::cout << std::endl;
	}
}
template <class T>
void Stack<T>::set_size(int Size)
{
	size = Size;
	top = -1;
	Array = new T[size];
}

int main()
{
	std::string command, line, element;
	Stack<std::string> stack;
	bool sizeSetFlag = false; // флаг показывает, задавался ли размер стека методом set_size()
	bool empty = false; // флаг показывает, пуста ли поданная в поток ввода строка
	while (std::getline(std::cin, line))
	{
		std::istringstream is(line);
		if (is.rdbuf()->in_avail() == 0) // проверяем на пустоту
		{
			empty = true;
		}
		command.clear();
		is >> command; // сначала считывается команда
		if (command == "set_size")
		{
			if (!sizeSetFlag) // дополнительно проверяется, задан ли стэк
			{
				is >> element; // считывается аргумент
				stack.set_size(std::stoi(element));
				sizeSetFlag = true;
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else if (command == "push")
		{
			is >> element; // считывается аргумент
			if (is.rdbuf()->in_avail() == 0 && sizeSetFlag)
			{
				stack.push(element);
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else if (command == "pop") 
		{
			if (is.rdbuf()->in_avail() == 0 && sizeSetFlag) // если строка состоит только из pop
			{
				stack.pop();
			}
			else
			{
				std::cout << "error" << std::endl;
			}
		}
		else if (command == "print" && sizeSetFlag)
		{
			if (is.rdbuf()->in_avail() == 0) // если строка состоит только из print
			{
				stack.print();
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