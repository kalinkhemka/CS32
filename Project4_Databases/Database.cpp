#include "Database.h"
#include "MultiMap.h"
#include "http.h"
#include "Tokenizer.h"
#include <vector>
#include <unordered_set>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

Database::Database()
{
	//Nothing
}
Database::~Database() //Destructing
{
	m_schema.clear();
	int count = m_rows.size();
	for (int i = 0; i < count; i++)//Clearing the vector of vector of strings
	{
		m_rows[i].clear();
	}
	m_rows.clear();
	for (int i = 0; i < m_fieldIndex.size(); i++) //Clearing the vector of maps
	{
		delete m_fieldIndex[i];
	}
	m_fieldIndex.clear();
}
bool Database::specifySchema(const	std::vector<FieldDescriptor>&	schema)
{
	m_schema.clear();
	bool hasIndex = false; //Index to check if there is an indexed schema
	for (int i = 0; i < schema.size(); i++)
	{
		m_schema.push_back(schema[i]);
		if (schema[i].index == it_indexed)
		{
			m_fieldIndex.push_back(new MultiMap);
			hasIndex = true;
		}
	}
	if (hasIndex == true) //If index, then return true
	{
		return true;
	}
	else
		m_schema.clear();
	return false;
}	
bool Database::addRow(const	std::vector<std::string>& rowOfData)
{
	if (m_schema.empty())
		return false;
	if (rowOfData.empty()) //Checking for empty
		return false;
	if (rowOfData.size() != m_schema.size())
		return false;
	m_rows.push_back(rowOfData);
	for (int i = 0; i < m_schema.size(); i++) //Adds the row
	{
		if (m_schema[i].index == it_indexed)
			m_fieldIndex[i]->insert(rowOfData[i],m_rows.size()-1);
	}
	return true;
}

bool Database::loadFromURL(std::string	url)
{
	string text;
	if (!HTTP().get(url,text))
	{
		setDatabaseInvalid();
		return false;
	}

	std::string delimiter1 = "\n"; // new line
	std::string delimiter2 = ",";// Comma

	bool firstTime = true, ranOnce = false;
 
	Tokenizer t(text , delimiter1);
	string result, result2; 

	while (t.getNextToken(result))//Gets the line of strings
	{
		if (result.empty())
			continue;
		ranOnce = true;
		Tokenizer t2(result,delimiter2);
		if (firstTime)
		{
			vector<FieldDescriptor> newSchema;
			while (t2.getNextToken(result2)) //Gets the individual strings
			{
				if (result2.empty())
					continue;
				FieldDescriptor f;
				if (result2[result2.size()-1] == '*')
				{
					f.name = result2.substr(0,result2.size()-1);
					f.index = it_indexed;
				}
				else
				{
					f.name = result2;
					f.index = it_none;
				}
				newSchema.push_back(f);
			}
			if (!specifySchema(newSchema))
			{
				setDatabaseInvalid();
				return false;
			}
		}
		else //If not the first time
		{
			vector<string> nextRow;
			while (t2.getNextToken(result2))
			{
				if (result2.empty())
					continue;
				nextRow.push_back(result2);
			}
			if (!addRow(nextRow))
			{
				setDatabaseInvalid();
				return false;
			}

		}
		firstTime = false;
	}
	if (!ranOnce) //If didn't run
	{
		setDatabaseInvalid();
		return false;
	}
	return true;
}
bool Database::loadFromFile(std::string	filename)
{
	ifstream infile(filename);    // infile is a name of our choosing
	if (!infile)		        // Did opening the file fail?
	{
		setDatabaseInvalid();
		return false;
	}
	string text, result;
	std::string delimiter = ","; // new line

	bool firstTime = true, ranOnce = false;
 
	while(getline(infile, text)) //Gets the line of strings
	{
		if (text.empty())
			continue;
		Tokenizer t(text , delimiter);
		ranOnce = true;
		if (firstTime)
		{
			vector<FieldDescriptor> newSchema;
			while (t.getNextToken(result)) //Gets the individual strings
			{
				if (result == "")
					continue;
				FieldDescriptor f;
				if (result[result.size()-1] == '*') //checks for index
				{
					f.name = result.substr(0,result.size()-1);
					f.index = it_indexed;
				}
				else
				{
					f.name = result;
					f.index = it_none;
				}
				newSchema.push_back(f);
			}
			if (!specifySchema(newSchema))
			{
				setDatabaseInvalid();
				return false;
			}
		}
		else //otherwise if not first time, adds it
		{
			vector<string> nextRow;
			while (t.getNextToken(result))
			{
				if (result.empty())
					continue;
				nextRow.push_back(result);
			}
			if (!addRow(nextRow))
			{
				setDatabaseInvalid();
				return false;
			}

		}
		firstTime = false;
	}
	if (!ranOnce) //If didn't run
	{
		setDatabaseInvalid();
		return false;
	}
	return true;
}
int	Database::getNumRows()	const
{
	return m_rows.size(); //Returns size
}
bool Database::getRow(int	rowNum,	std::vector<std::string>&	row)	const
{
	if (rowNum >= m_rows.size()) //Checks the bounds
		return false;
	row.clear();
	for (int i = 0; i < m_rows[rowNum].size(); i++)
	{
		row.push_back(m_rows[rowNum][i]); //Puts values in the vector
	}
	return true;
}
int	Database::search(const	std::vector<SearchCriterion>&	searchCriteria, const	std::vector<SortCriterion>&	sortCriteria, std::vector<int>&	results)
{
	if (searchCriteria.empty())
	{
		results.clear();
		return ERROR_RESULT;
	}
	unordered_set<int> initial;
	bool firstRun = true;

	for (int i = 0; i < searchCriteria.size(); i++) //Cycles through the searchCriteria
	{
		unordered_set<int> everyRun;
		int fieldIndexNum;
		bool schemaMatch = false;
		for (int j = 0; j < m_schema.size(); j++)
		{
			if (searchCriteria[i].fieldName == m_schema[j].name && m_schema[j].index== it_indexed) //If the schema matches and has an index, then it can be searched
			{
				schemaMatch = true;
				fieldIndexNum = j;
			}
			if (searchCriteria[i].maxValue == "" && searchCriteria[i].minValue == "") //checks if no max or min
			{
				results.clear();
				return ERROR_RESULT;
			}
		}
		if (schemaMatch == false) //If no schema, then return error
		{
			results.clear();
			return ERROR_RESULT;
		}
		MultiMap::Iterator it;
		if (searchCriteria[i].minValue == "") //Checks the min value
			it = MultiMap::Iterator (m_fieldIndex[fieldIndexNum]->findEqualOrSuccessor(""));
		else
			it = (m_fieldIndex[fieldIndexNum]->findEqualOrSuccessor(searchCriteria[i].minValue));
		if (firstRun) //If its the first time, different implementation
		{
			if (searchCriteria[i].maxValue != "")
				while (it.valid() && it.getKey() <= searchCriteria[i].maxValue) //If no maxvalue
				{
					initial.insert(it.getValue());
					it.next();
				}
			else
				while (it.valid()) //If maxvalue
				{
					initial.insert(it.getValue());
					it.next();
				}
		}
		else //If running second time
		{
			if (searchCriteria[i].maxValue != "")//Nno maxvalue
				while (it.valid() && it.getKey() <= searchCriteria[i].maxValue)
				{
					int num = initial.size();
					initial.insert(it.getValue());
					if (initial.size() == num)
					{
						everyRun.insert(it.getValue());
					}
					it.next();
				}
			else
				while (it.valid()) //With maxvalue
				{
					int num = initial.size();
					initial.insert(it.getValue());
					if (initial.size() == num)
						everyRun.insert(it.getValue());
					it.next();
				}
			initial = everyRun;
		}
		firstRun = false;
	}

	unordered_set<int>::iterator itr = initial.begin();
	while (itr != initial.end()) //Putting the values into the vector form the unordered list
	{
		results.push_back(*itr);
		itr++;
	}

	//The Sorting Section

	int flag1 = 0;
    for(int i = 0; i < sortCriteria.size(); i++) //check if the sorting criteria are invalid
    {
        for(int j = 0; j < m_schema.size(); j++)
        {
             if(m_schema[j].name == sortCriteria[i].fieldName)
                flag1++;
        }
    }
	if(flag1 != sortCriteria.size())   //If the sorting criteria are invalid
		return ERROR_RESULT;

	QuickSort(results, 0, results.size()-1, sortCriteria);
	return results.size();
}

bool Database::doesABelongBeforeB(int a, int b, const std::vector<Database::SortCriterion>& sortCriteria) const
{
	for (int k = 0; k < sortCriteria.size(); k++)
	{
		int numIndex;
		for (int j = 0; j < m_schema.size(); j++)
			if (sortCriteria[k].fieldName == m_schema[j].name)
				numIndex = j;
		if (sortCriteria[k].ordering == ot_ascending) //checks the ascending order
		{
			if (m_rows[a][numIndex] < m_rows[b][numIndex]) //checks the values against each other
				return true;
			if (m_rows[a][numIndex] > m_rows[b][numIndex])
				return false;
		}
		else //Checks the descending order
		{
			if (m_rows[a][numIndex] < m_rows[b][numIndex])//checks the values against each other
				return false;
			if (m_rows[a][numIndex] > m_rows[b][numIndex])
				return true;
		}
	}
	return true;
}

int Database::Partition(std::vector<int>& inRows, int low, int high, const std::vector<Database::SortCriterion>& sortCriteria)
{
	int pi = low;
	int pivot = inRows[low];  
	do
	{
		while ( low <= high && doesABelongBeforeB(inRows[low], pivot, sortCriteria) ) //Checks where to place the pivot value
			low++;
		while (high > pi &&  doesABelongBeforeB( pivot,inRows[high], sortCriteria)) //checks the place to move the highest values
			high--;
		if ( low < high )
		swap(inRows[low], inRows[high]);
	}
	while ( low < high );
	swap(inRows[pi], inRows[high]);
	pi = high;
	return(pi);
}

void Database::QuickSort(std::vector<int> &rowsIn,int a,int b, const std::vector<Database::SortCriterion>& sortCriteria)
{
  if (b - a >= 1 )
  {
    int PivotIndex;
    PivotIndex = Partition(rowsIn,a,b,sortCriteria);  //Get the pivot index value
    QuickSort(rowsIn,a,PivotIndex-1,sortCriteria); // left 
    QuickSort(rowsIn,PivotIndex+1,b,sortCriteria);  // right
  }
}

Database::Database(const	Database&	other)
{
}
Database& Database::operator=(const	Database&	rhs)
{
	return *this;
}

void Database::setDatabaseInvalid() //Clears the database
{
	m_schema.clear();
	int count = m_rows.size();
	for (int i = 0; i < count; i++)
	{
		m_rows[i].clear();
	}
	m_rows.clear();
}
