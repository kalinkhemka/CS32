#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

typedef std::string KeyType;
typedef double ValueType; 

class Map
{
  public:
    Map();
	Map(const Map& other);
	Map& operator= (const Map& other);
	~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
	void dump();
  private:
	struct data //The nodes for the project
	{
		KeyType m_key;
		ValueType m_value;
		data* m_nextData;
		data* m_lastData;
	};

	data* find(const KeyType& key) const //Returns the pointer to node with key or nullptr if not in list
	{
		data* search = m_head;
		while (search != nullptr)
		{
			if (search->m_key == key)
			{
				return search;
			}
			search = search->m_nextData;
		}
		return nullptr;
	}

	int m_length;
	data* m_head;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);


#endif