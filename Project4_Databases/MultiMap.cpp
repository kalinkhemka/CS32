#include "MultiMap.h"

MultiMap::Node* MultiMap::getStartTree() const //Helper Function to get First Node in Order (NEVER REALLY USED. ONLY FOR TESTING)
{
	if (m_root->left == nullptr)
		return m_root;
	Node* temp = m_root;
	while (temp->left != nullptr)
		temp = temp->left;
	return temp;
}

MultiMap::Iterator::Iterator()
{
	m_node = nullptr;
}

MultiMap::Iterator::Iterator(Node* start)
{
	m_node = start;
}

bool MultiMap::Iterator::valid() const
{
	if (m_node != nullptr) //Checks 
		return true;
	else
		return false;
}

std::string	MultiMap::Iterator::getKey()	const
{
	if (valid())
		return (m_node->key); //Returns key if valid
	else
		exit(0);
}
unsigned int MultiMap::Iterator::getValue()	const
{
	if (valid())
		return (m_node->value); //rETURNS value if valid
	else
		exit(0);
}	
bool MultiMap::Iterator::next()
{
	if (!valid())
		return false;
	Node* temp = m_node;
	Node* start = m_node;
	bool checkLeft = false, checkTop = false, keepGoing = false;
	if (temp->right == nullptr) //Check the right side of the tree
		{
			if (temp->prev == nullptr)
			{
				m_node = nullptr;
				return false;
			}
			temp = temp->prev;
			if (temp->right == start)
				keepGoing = true;
			else
				keepGoing = false;
			if (temp->key >= start->key && !keepGoing)
			{
				m_node = temp;
				return true;
			}
			start = temp;
			checkTop = true;
		} else //Moving to the right side of the tree
		{
			temp = temp->right;
			checkLeft = true;
		}
	for (;;)
	{
		if (temp == nullptr) //Checking for the nullptr
		{
			m_node = nullptr;
			return false;
		}
		if (checkTop) //checking the top of the tree
		{
			if (temp->prev == nullptr)
			{
				m_node = nullptr;
				return false;
			}
			temp = temp->prev;
			if (temp->right == start)
				keepGoing = true;
			else
				keepGoing = false;
			if (temp->key >= start->key && !keepGoing)
			{
				m_node = temp;
				return true;
			}
			start = temp;
		}
		if (checkLeft) //checking the left of the tree
		{
			if (temp->left != nullptr)
				temp = temp->left;
			else
			{
				m_node = temp;
				return true;
			}
		}
	}
	m_node = temp;
	if (m_node == nullptr)
		return false;
	return true;
}	
bool MultiMap::Iterator::prev() //Going to the previous node
{
	if (!valid())
		return false;
	Node* temp = m_node;
	Node* start = m_node;
	bool checkRight = false, checkTop = false, keepGoing = false;
	if (temp->left == nullptr) //Checks the left tree
		{
			if (temp->prev == nullptr)
			{
				m_node = nullptr;
				return false;
			}
			temp = temp->prev;
			if (temp->left == start)
				keepGoing = true;
			else
				keepGoing = false;
			if (temp->key <= start->key && !keepGoing)
			{
				m_node = temp;
				return true;
			}
			start = temp;
			checkTop = true;
		} else //There is a left node
		{
			temp = temp->left;
			checkRight = true;
		}
	for (;;)
	{
		if (temp == nullptr) //Check if null
		{
			m_node = nullptr;
			return false;
		}
		if (checkTop) //If on the right side of the tree
		{
			if (temp->prev == nullptr)
			{
				m_node = nullptr;
				return false;
			}
			temp = temp->prev;
			if (temp->left == start)
				keepGoing = true;
			else
				keepGoing = false;
			if (temp->key <= start->key && !keepGoing)
			{
				m_node = temp;
				return true;
			}
			start = temp;
		}
		if (checkRight) //If on the left side of the tree
		{
			if (temp->right != nullptr)
				temp = temp->right;
			else
			{
				m_node = temp;
				return true;
			}
		}
	}
	m_node = temp;
	if (m_node == nullptr)
		return false;
	return true;
}

MultiMap::MultiMap() //Default nullptr
{
	m_root = nullptr;
}

MultiMap::~MultiMap() //Destruction
{

	clearMap(m_root);
	m_root = nullptr; 
}

void MultiMap::clear() //Clearing
{
	clearMap(m_root);
	m_root=nullptr; 
}
void MultiMap::insert(std::string key, unsigned int value)
{
	if (m_root == nullptr) //If null, just add it
	{
		m_root = new Node(key,value);
		return;
	}
	Node* cur = m_root;
	for (;;)
	{
		if (key <= cur->key) //Check the left tree
		{
			if (cur->left != NULL) 
				cur = cur->left;
			else
			{
			cur->left = new Node(key,value);
			cur->left->prev = cur;
			return;
			}
		}
		if (key > cur->key) //Check the right tree
		{
			if (cur->right != NULL)
				cur = cur->right;
			else 
			{
				cur->right = new Node(key,value);
				cur->right->prev = cur;
				return;
			}
		}
	}
}

MultiMap::Iterator MultiMap::findEqual(std::string	key)	const
{
	Iterator itr;
	Node* temp = m_root;
	while (temp != nullptr)
	{
		if (key < temp->key)//Checks the left tree
		{
			temp=temp->left;
		} else
		if (key > temp->key)//Checks the right tree
		{
			temp=temp->right;
		} else
		if (key == temp->key)//Checks if equal
		{
			itr = temp;
			temp = temp->left;
		}
	}
	return itr;
}

MultiMap::Iterator	MultiMap::findEqualOrSuccessor(std::string	key)	const
{
	Iterator itr;
	Node* temp = m_root;
	while (temp != nullptr)
	{
		if (key < temp->key)//Checks the left tree
		{
			itr = temp;
			temp=temp->left;
		} else
		if (key > temp->key)//Checks the right tree
		{
			temp=temp->right;
		} else
		if (key == temp->key)//Checks if equal
		{
			itr = temp;
			temp = temp->left;
		}
	}
	return itr;
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string	key)	const
{
	Iterator itr;
	Node* temp = m_root;
	while (temp != nullptr)
	{
		if (key < temp->key) //Checks the left tree
		{
			temp=temp->left;
		} else
		if (key > temp->key)//Checks the right tree
		{
			itr = temp;
			temp=temp->right;
		} else
		if (key == temp->key) //Checks if equal
		{
			itr = temp;
			temp = temp->left;
		}
	}
	return itr;
}

MultiMap::MultiMap(const MultiMap& other)
{
}

MultiMap& MultiMap::operator=(const MultiMap& rhs)
{
	return *this;
}

void MultiMap::clearMap(Node* input) //Clears the map without the use of recursion
{
	if (input == nullptr)
		return;
	if (input->prev == nullptr && input->left == nullptr && input->right == nullptr)
	{
		delete input;
		input = nullptr; 
	}
	if (input->left == nullptr && input->right == nullptr)
	{
		Node *temp = input; 
		input = input->prev;

		if(input->right!=nullptr && input->right==temp)
		{
			input->right = nullptr ;
		}

		else if(input->left!=nullptr && input->left==temp)
		{
			input->left = nullptr; 
		}
		delete temp;
		temp = nullptr; 
	}

	if(input->left == nullptr)
		input = input->right;
	else if(input->right == nullptr)
		input = input->left;

	else 
		input = input->right;
}