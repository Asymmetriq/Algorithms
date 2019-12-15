#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <cmath>


struct Node
{
	int64_t key;
	std::string value;
	Node* left, * right, * parent;

	Node(const int64_t& k) : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
	Node(const int64_t& k, const std::string& v, Node* p) : key(k), value(v),
		left(nullptr), right(nullptr), parent(p) {}
};

class SplayTree
{
	Node* root;
	size_t size;

	// Является ли правым потомком
	bool isRight(Node* node)
	{
		if (node && node->parent)
			if (node->parent->right == node)
				return true;
			else
				return false;
	}

	// Вспомогательная функция поиска
	std::pair<int, Node*> Find(const int64_t& key)
	{
		Node* node = root;
		while (node)
		{
			if (node->key == key)
				return { 2, node }; // Нода найдена
			else if (node->key > key)
				if (node->left)
					node = node->left;
				else
					return { 0, node }; // Потенциальный левый потомок для Add
			else if (node->key < key)
				if (node->right)
					node = node->right;
				else
					return { 1, node }; // Потенциальный правый потомок для Add
		}
		return { 0, nullptr };
	}

	// Вспомогательная функция слияния
	void Merge(Node* left, Node* right)
	{
		if (!left && !right)
			root = nullptr;
		else if (!left)
		{
			root = right;
			right->parent = nullptr;
		}
		else if (!right)
		{
			root = left;
			left->parent = nullptr;
		}
		else
		{
			left = Max(left);
			left->parent = nullptr;
			left->right = right;
			right->parent = left;
		}
	}

	// Функции вращения 
	void rotateL(Node* node)
	{
		Node* p = node->parent;
		Node* r = node->right;
		if (p != nullptr)
			if (p->left == node)
				p->left = r;
			else
				p->right = r;
		Node* temp = r->left;
		r->left = node;
		node->right = temp;
		node->parent = r;
		r->parent = p;
		if (temp != nullptr)
			temp->parent = node;
	}

	void rotateR(Node* node)
	{
		Node* p = node->parent;
		Node* l = node->left;
		if (p != nullptr)
			if (p->left == node)
				p->left = l;
			else
				p->right = l;
		Node* temp = l->right;
		l->right = node;
		node->left = temp;
		node->parent = l;
		l->parent = p;
		if (temp != nullptr)
			temp->parent = node;
	}

	// 3 случая для Splay()
	void Zig(Node* node)
	{
		!isRight(node) ?
			rotateR(node->parent) : rotateL(node->parent);
	}

	void ZigZig(Node* node, bool side)
	{
		if (side)
		{
			rotateL(node->parent->parent);
			rotateL(node->parent);
		}
		else
		{
			rotateR(node->parent->parent);
			rotateR(node->parent);
		}
	}

	void ZigZag(Node* node, bool side)
	{
		if (side)
		{
			rotateL(node->parent);
			rotateR(node->parent);
		}
		else
		{
			rotateR(node->parent);
			rotateL(node->parent);
		}
	}

	// Основная функция Splay
	void Splay(Node* node)
	{
		while (node->parent != nullptr)
		{
			if (node->parent == root)
				Zig(node);
			else if (!isRight(node) && !isRight(node->parent)) // Левый, левый
				ZigZig(node, 0);
			else if (isRight(node) && isRight(node->parent))   // Правый, правый
				ZigZig(node, 1);
			else if (!isRight(node) && isRight(node->parent))  // Левый, правый
				ZigZag(node, 0);
			else											   // Правый, левый
				ZigZag(node, 1);
		}
		root = node;
	}

	// Вспомогательная функция печати ноды
	std::string printNode(Node* node)
	{
		std::string result;
		result += '[' + std::to_string(node->key) + ' ' + node->value;
		if (node->parent)
			result += ' ' + std::to_string(node->parent->key);
		result += ']';
		return result;
	}

	// Функции для поиска максимума / минимуму от ноды
	Node* Max(Node* node)
	{
		Node* temp = node;
		if (temp)
		{
			while (temp->right)
				temp = temp->right;
			Splay(temp);
			return temp;
		}
		else
			return nullptr;
	}

	Node* Min(Node* node)
	{
		Node* temp = node;
		if (temp)
		{
			while (temp->left)
				temp = temp->left;
			Splay(temp);
			return temp;
		}
		else
			return nullptr;
	}
public:

	Node* getRoot() { return root; }

	SplayTree() : root(nullptr), size(0) { }

	SplayTree(uint64_t key)
	{
		root = new Node(key);
		size = 1;
	}

	~SplayTree()
	{
		if (root)
		{
			std::stack<Node*> toDelete;
			toDelete.push(root);
			while (!toDelete.empty())
			{
				Node* node = toDelete.top();
				if (node->left)
				{
					toDelete.push(node->left);
					node->left = nullptr;
				}
				else if (node->right)
				{
					toDelete.push(node->right);
					node->right = nullptr;
				}
				else
				{
					toDelete.pop();
					delete node;
				}
			}
		}
	}

	bool Add(const int64_t& key, const std::string& value)
	{
		if (!root)
		{
			root = new Node(key, value, nullptr);
			size++;
			return true;
		}
		else
		{
			std::pair<int, Node*> result = Find(key);
			if (result.first == 2)
				return false;
			else if (result.first == 0)
			{
				result.second->left = new Node(key, value, result.second);
				Splay(result.second->left);
				size++;
				return true;
			}
			else
			{
				result.second->right = new Node(key, value, result.second);
				Splay(result.second->right);
				size++;
				return true;
			}
		}
	}

	Node* Search(const int64_t& key)
	{
		if (root)
		{
			std::pair<int, Node*> result = Find(key);
			if (result.first == 2)
			{
				Splay(result.second);
				return result.second;
			}
			Splay(result.second);
		}
		return nullptr;
	}

	Node* Min() { return Min(root); }
	Node* Max() { return Max(root); }

	bool Set(const int64_t& key, const std::string& value)
	{

		if (!root)
			return false;
		std::pair<int, Node*> result = Find(key);
		if (result.first == 2)
		{
			result.second->value = value;
			Splay(result.second);
			return true;
		}
		else
			return false;
	}

	bool Delete(const int64_t& key)
	{
		if (!root)
			return false;
		std::pair<int, Node*> result = Find(key);
		if (result.first == 2)
		{
			Splay(result.second);
			Node* subL = result.second->left;
			Node* subR = result.second->right;
			Merge(subL, subR);
			delete result.second;
			size--;
			return true;
		}
		return false;
	}

	std::string Print()
	{
		std::string result;
		std::queue<Node*> toPrint;
		toPrint.push(root);
		size_t counter = size;
		size_t space = 0;
		size_t i = 0;
		while (!toPrint.empty())
		{
			Node* node = toPrint.front();
			toPrint.pop();
			space++;

			if (node)
			{
				result += printNode(node);
				toPrint.push(node->left);
				toPrint.push(node->right);
				counter--;
			}
			else
			{
				result += "_";
				toPrint.push(nullptr);
				toPrint.push(nullptr);
			}

			if (space == pow(2, i))
			{
				result += "\n";
				if (counter != 0)
				{
					i++;
					space = 0;
				}
			}
			else
				result += " ";

			if (counter == 0 && space == pow(2, i))
				break;
		}
		return result;
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

	SplayTree tree;

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
			if (!tree.Add(key, value))
				os << "error" << std::endl;
		}
		else if (command == "set" && is.peek() != '\n'
			&& is >> key && is.peek() != '\n' && is >> value && is.peek() == '\n')
		{
			if (!tree.Set(key, value))
				os << "error" << std::endl;
		}
		else if (command == "delete" && is.peek() != '\n'
			&& is >> key && is.peek() == '\n')
		{
			if (!tree.Delete(key))
				os << "error" << std::endl;
		}
		else if (command == "search" && is.peek() != '\n'
			&& is >> key && is.peek() == '\n')
		{
			Node* temp = tree.Search(key);
			if (temp)
			{
				os << "1 "
					<< temp->value << std::endl;
			}
			else
				os << "0" << std::endl;
		}
		else if (command == "min" && is.peek() == '\n')
		{
			Node* temp = tree.Min();
			if (temp)
			{
				os << temp->key << " "
					<< temp->value << std::endl;
			}
			else
				os << "error" << std::endl;
		}
		else if (command == "max" && is.peek() == '\n')
		{
			Node* temp = tree.Max();
			if (temp)
			{
				os << temp->key << " "
					<< temp->value << std::endl;
			}
			else
				os << "error" << std::endl;
		}
		else if (command == "print" && is.peek() == '\n')
		{
			os << tree.Print();
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