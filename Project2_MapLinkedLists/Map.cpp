#include "Map.h"
#include <iostream>

Map::Map()
{
	m_length = 0;
	m_head = nullptr;
}
Map::Map(const Map& other)
{
	m_length = other.m_length;
	data* copy = new data; //New Node for New List
	m_head = copy;//Start of new list
	data* oCopy = other.m_head;
	while(oCopy != nullptr)
	{
		copy->m_key = oCopy->m_key; //Copying Keys
		copy->m_value = oCopy->m_value; //Copying values
		(oCopy->m_nextData != nullptr) ? (copy->m_nextData = new data, copy->m_nextData->m_lastData = copy) : copy->m_nextData = nullptr; //If m_nextData esists, then the nextData is set to a new node and lastData of next Data is set to this node, else the nextData of this node is set to nullptr
		if (oCopy->m_lastData == nullptr) //Check for a nullptr
			copy->m_lastData = nullptr; //Sets the new lists pointer to that if it is true
		oCopy = oCopy->m_nextData;//Moves on	
		copy = copy->m_nextData;//Moves on
	}
}
Map& Map::operator= (const Map& other)
{
	if (this != &other)//Checks to see if the same
    {		
        Map temp(other);//Creates copy of other map
        swap(temp);//Swaps the copy and this map
    }
	return *this;
}
Map::~Map()
{
	data* remove = this->m_head;
	if (remove != nullptr)//Check for a nullptr, does nothing if it is nullptr
	{
		while (remove->m_nextData != nullptr)
		{
			remove = remove->m_nextData;//Sets itself to next node
			delete remove->m_lastData;//Deletes the previous node
		}
		delete remove;//Deletes current node
	}
}
bool Map::empty() const
{
	return (m_length == 0);
}
int Map::size() const
{
	return m_length;
}
bool Map::contains(const KeyType& key) const
{
	data* search = this->find(key);//Finds node with key
	if (search == nullptr)
		return false;
	else
		return true;
}
bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (this->contains(key))
		return false;

	data* nData = new data; //Creates new node
	nData->m_key = key; 
	nData->m_value = value;
	nData->m_nextData = m_head; //Sets next to the previous beginning of list
	nData->m_lastData = nullptr; //Sets previous to null

	if (nData->m_nextData != nullptr)
		nData->m_nextData->m_lastData = nData; //Sets the last data of the next node to this node

	m_head = nData;
	m_length++;//Increases length

	return true;
}
bool Map::update(const KeyType& key, const ValueType& value)
{
	data* search = this->find(key);//Finds node with key
	if (search == nullptr)
		return false;
	else
		search->m_value = value;//Updates this if found
	return true;

}
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	data* search = this->find(key);//Finds node with key
	if (search == nullptr)
		this->insert(key, value);//Inserts if not in list
	else
		search->m_value = value;//Updates value if in list
	return true;
}
bool Map::erase(const KeyType& key)
{
	data* search = this->find(key);//Finds node with key
	if (search == nullptr)
		return false;
	if (search->m_nextData != nullptr)//Checks if nextData exists
		if(search->m_lastData != nullptr)//Checks if previous data exists
		{
			search->m_nextData->m_lastData = search->m_lastData; //Sets the next node's previous pointer to this node's previous
			search->m_lastData->m_nextData = search->m_nextData; //Sets the previous node's next poitner to this node's next
		}
		else
			m_head = search->m_nextData; //Makes the head pointer point to the next node if nothing before this node
	else
		if (search->m_lastData != nullptr)
			search->m_lastData->m_nextData = nullptr; //Sets the previous node's nextData nullptr if at the end of list
	delete search;
	m_length--;//Decrements length
	return true;
}
bool Map::get(const KeyType& key, ValueType& value) const
{
	data* search = this->find(key);//Finds node with key
	if (search == nullptr)
		return false;
	value = search->m_value;//Updates the parameter's value if found
	return true;
}
bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= this->size() || i < 0)//Checks if i is within bounds
		return false;
	data* search = m_head;
	for (int j = 0; j < i; j++) //Scouts for the node number i
	{
		search = search->m_nextData;
	}
	key = search->m_key;//Updates key
	value = search->m_value; //Updates value
	return true;
}
void Map::swap(Map& other)
{
	data* temp = m_head; //Creates temp node to complete the swap
	m_head = other.m_head; //Swaps
	other.m_head = temp; //Swaps

	int temp2 = m_length; //Creates temp length
	m_length = other.m_length; //Swaps
	other.m_length = temp2; //Swaps
}
void Map::dump()
{
	data* printer = m_head;
	while (printer != nullptr) //Prints the keys and values in list
	{
		std::cerr << "Key: " << printer->m_key << " Value: " << printer->m_value << std::endl;
		std::cerr << "Next: " << printer->m_nextData << " Last: " << printer->m_lastData << std::endl;
		printer = printer->m_nextData;
	}

}
bool combine(const Map& m1, const Map& m2, Map& result)
{
	Map temp;//New temp map
	bool flag = true, flagIn = true;
	KeyType tempK1, tempK2;
	ValueType tempV1, tempV2;

	for (int i = 0; i < m1.size(); i++)//Cycles through first map
	{
		flagIn = true;//Resets flag
		m1.get(i, tempK1, tempV1); //Gets keys and values from first map
		for (int j = 0; j < m2.size(); j++) //Cycles through second map
		{
			m2.get(j, tempK2, tempV2); //Gets keys and values from second map
			if (tempK1 == tempK2) //checks if the keys are same
			{
				if (tempV1 != tempV2) //If values are different for same key
				{
					flag = false; //The flag to return is set to false
					flagIn = false;//The flag to add this key is set to false
				}
			}
		}
		if (flagIn == true)//Adds the key from map 1 only if flag unchanged
		{
			temp.insert(tempK1,tempV1);
		}
	}

	for (int i = 0; i < m2.size(); i++)//Cycles through second list
	{
		flagIn = true; //Resets flag
		m2.get(i, tempK1, tempV1);
		for (int j = 0; j < m1.size(); j++)//Cycles through first list
		{
			m1.get(j, tempK2, tempV2);
			if (tempK1 == tempK2)//Checks if keys are the same
			{
				if (tempV1 != tempV2) //If the values are different for same key
				{
					flag = false; //Return flag is set to false
					flagIn = false; //Adding flag is set to false
				}
			}
		}
		if (flagIn == true) //Only adds key from second map if flag unchanged
		{
			temp.insert(tempK1,tempV1);
		}
	}

	result = temp;
	return flag;
}
void subtract(const Map& m1, const Map& m2, Map& result)
{
	bool flag = true;
	Map temp;
	KeyType tempK1, tempK2;
	ValueType tempV1, tempV2;

	for (int i = 0; i < m1.size(); i++)//Cycles through map 1
	{
		bool flag = true;//Reset flag
		m1.get(i, tempK1, tempV1);
		for (int j = 0; j < m2.size(); j++) //cycles through map 2
		{
			m2.get(j, tempK2, tempV2);
			if (tempK1 == tempK2) //If the keys are the same, set flag to false
				flag = false;
		}
		if (flag == true) //Only add key from map 1 if flag unchanged
			temp.insert(tempK1,tempV1);
	}
	result = temp;
}