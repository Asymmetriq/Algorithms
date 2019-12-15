#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

// Реализация циклической очереди с использованием массива
template <class T>
class Queue
{
	int size = 0;
	T* Array = nullptr;
	int front = 0;
	int rear = 0;

public:
	Queue();
	// Пустой конструктор, поля задаются методом set_size
	~Queue()
	{
		if (Array)
			delete[] Array;
	}
	void push(T element, std::ofstream&);
	void pop(std::ofstream& os);
	void print(std::ofstream& os);
	void set_size(int Size); // метод, задающий поля
};
template <class T>
Queue<T>::Queue()
{}
template <class T>
void Queue<T>::push(T element, std::ofstream& os)
{
	if ((rear == (size - 1) && front == 0) // Проверка: голова очереди - первый элемент, хвост - последний
		|| rear == (front - 1)) // Проверка: хвост догнал голову по кругу
	{
		os << "overflow" << std::endl;
	}
	// Если элемент вставляется в пустую очередь
	else if (front == -1)
	{
		rear = front = 0;
		Array[rear] = element;
	}
	// Если места в конце кончились, а в начале свободны
	else if (rear == (size - 1)) 
	{
		rear = 0;
		Array[rear] = element;
	}
	else
	{
		Array[++rear] = element;
	}
}
template <class T>
void Queue<T>::pop(std::ofstream& os)
{
	if (rear <= -1)
	{
		os << "underflow" << std::endl;
	}
	// Если элемент единственный - обнуляем
	else if (rear == front)
	{
		os << Array[front] << std::endl;
		rear = front = -1;
	}
	// Если голова добралась до конца - круг сначала
	else if (front == size - 1)
	{
		os << Array[front] << std::endl;
		front = 0;
	}
	else
	{
		os << Array[front] << std::endl;
		front++;
	}
}
template <class T>
void Queue<T>::print(std::ofstream& os)
{
	if (rear <= -1)
	{
		os << "empty" << std::endl;
	}
	else if (front <= rear)
	{
		for (size_t i = front; i < rear; i++)
		{
			os << Array[i] << " ";
		}
		os << Array[rear] << std::endl;
	}
	else
	{
		for (size_t i = front; i < size; i++)
		{
			os << Array[i] << " ";
		}
		for (size_t i = 0; i < rear; i++)
		{
			os << Array[i] << " ";
		}
		os << Array[rear] << std::endl;
	}
}
template <class T>
void Queue<T>::set_size(int Size)
{
	size = Size;
	front = -1;
	rear = -1;
	Array = new T[size];
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		std::string in = argv[1];
		std::string out = argv[2];
		std::ifstream infile(in); // открываем файл ввода
		std::ofstream outfile(out); // открываем файл вывода
		std::string command, line, element;
		Queue<std::string> queue;
		bool sizeSetFlag = false; // флаг показывает, задавался ли размер очереди методом set_size()
		bool empty = false; // флаг показывает, пуста ли поданная в поток ввода строка
		while (std::getline(infile, line))
		{
			std::cout << line << std::endl;
			std::istringstream is(line);
			if (is.rdbuf()->in_avail() == 0) // проверяем на пустоту
			{
				empty = true;
			}
			command.clear();
			is >> command; // сначала считывается команда
			if (command == "set_size")
			{
				if (!sizeSetFlag) // дополнительно проверяется, задан ли размер очереди
				{
					is >> element; // считывается аргумент
					queue.set_size(std::stoi(element));
					sizeSetFlag = true;
				}
				else
				{
					outfile << "error" << std::endl;
				}
			}
			else if (command == "push")
			{
				is >> element; // считывается аргумент
				if (is.rdbuf()->in_avail() == 0 && sizeSetFlag)
				{
					queue.push(element, outfile);
				}
				else
				{
					outfile << "error" << std::endl;
				}
			}
			else if (command == "pop")
			{
				if (is.rdbuf()->in_avail() == 0 && sizeSetFlag) // если строка состоит только из pop
				{
					queue.pop(outfile);
				}
				else
				{
					outfile << "error" << std::endl;
				}
			}
			else if (command == "print" && sizeSetFlag)
			{
				if (is.rdbuf()->in_avail() == 0) // если строка состоит только из print
				{
					queue.print(outfile);
				}
				else
				{
					outfile << "error" << std::endl;
				}
			}
			else
			{
				if (!empty) // если строка не пустая и не является ни одной из команд
					outfile << "error" << std::endl;
			}
		}
		infile.close();
		outfile.close();
	}
	return 0;
}