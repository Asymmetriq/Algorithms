#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>

using namespace std;

class Graph
{
	struct Edge
	{
		string v1;
		string v2;
	};

	string type; // Тип графа
	string start; // Начальная вершина
	string search; // Тип обхода

	// Список смежности
	map<string, vector<string>> adjacency; 

	void DFS(unordered_map<string, bool>& used, const string& start); // Обход в глубину
	void BFS(unordered_map<string, bool>& used, const string& start); // Обход в ширину

public:
	Graph();

	void Read();
	void Search();
};

Graph::Graph()
{
	cin >> type >> start >> search; // Конструктор считывает в поля тип графа, нач. вершину и тип поиска
}

void Graph::Read()
{
	Edge edge; // Ребро графа
	vector<Edge> edges; // Список всех рёбер

	// Считываем рёбра
	while (cin >> edge.v1 >> edge.v2)
	{
		edges.push_back(edge);

		// Добавляем вершины рёбер в список смежности
		adjacency[edge.v1] = vector<string>();
		adjacency[edge.v2] = vector<string>();
	}

	// Заполняем список смежности
	for (size_t i = 0; i < edges.size(); i++)
	{
		string v1 = edges[i].v1;
		string v2 = edges[i].v2;

		adjacency[v1].push_back(v2); // Добавляем v1-v2

		// Если граф неориентированный
		if (type == "u")
		{
			adjacency[v2].push_back(v1); // То добавляем v2-v1
		}
	}
	// Сортируем смежные вершины лексикографически
	for (auto &[key, value] : adjacency)
	{
		std::sort(value.begin(), value.end());
	}
}

// Обход по графу
void Graph::Search()
{
	unordered_map<string, bool> used; // Были ли использованы вершины

	if (search == "d")
	{
		DFS(used, start);
	}
	else
	{
		BFS(used, start);
	}
}

// Обход в глубину
void Graph::DFS(unordered_map<string, bool>& used, const string& start)
{
	used[start] = true; // Помечем вершину как посещённую
	cout << start << endl; // Выводим вершину

	auto& vertexes = adjacency[start]; // Получаем смежные вершины

	// Проходимся по ним
	for (auto &v : vertexes)
	{
		if (!used[v]) // Если не использована
		{
			DFS(used, v); // Запускаем из неё обход в глубину
		}
	}
}

// Обход в ширину
void Graph::BFS(unordered_map<string, bool>& used, const string& start)
{
	queue<string> q;
	q.push(start);

	used[start] = true; // Помечем вершину как посещённую

	cout << start << endl; // Выводим вершину

	while (!q.empty()) 
	{
		string now = q.front();
		q.pop();

		auto& vertexes = adjacency[now]; // Получаем смежные вершины

		// Проходимся по смежным вершинам
		for (auto& v : vertexes)
		{
			if (!used[v]) // Если не использована
			{
				used[v] = true; // Помечаем использованной
				q.push(v); // Добавляем в очередь
				cout << v << endl; // Выводим
			}
		}
	}
}

int main()
{
	Graph graph; // Создаём граф

	graph.Read(); // Считываем рёбра графа
	graph.Search(); // Запускаем обход
}