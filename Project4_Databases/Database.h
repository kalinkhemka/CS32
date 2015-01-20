#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED


#include "MultiMap.h"
#include <string>
#include <vector>

class	Database	
{	
public:	
	enum IndexType {	it_none,	it_indexed	};	
	enum OrderingType {	ot_ascending,	ot_descending	};	
	
	struct	FieldDescriptor	
	{	
		std::string	name;
		IndexType	index;
	};	
	
	struct	SearchCriterion	
	{	
		std::string	fieldName;	
		std::string	minValue;	
		std::string	maxValue;	
	};	
	
	struct	SortCriterion	
	{	
		std::string	fieldName;		
		OrderingType ordering;	
	};	
	 	
	static const int ERROR_RESULT = -1;	
	
	Database();	
	~Database();	
	bool	specifySchema(const	std::vector<FieldDescriptor>&	schema);	
	bool	addRow(const	std::vector<std::string>&	rowOfData);	
	bool	loadFromURL(std::string	url);	
	bool	loadFromFile(std::string	filename);	
	int	getNumRows()	const;	
	bool	getRow(int	rowNum,	std::vector<std::string>&	row)	const;	
	int	search(const	std::vector<SearchCriterion>&	searchCriteria, const	std::vector<SortCriterion>&	sortCriteria, std::vector<int>&	results);	 		
	
private:	
	//	To	prevent	Databases	from	being	copied	or	assigned,	declare	these	members	
	//	private	and	do	not	implement	them.	
	Database(const	Database&	other);	
	Database&	operator=(const	Database&	rhs);

	void setDatabaseInvalid();
	bool doesABelongBeforeB(int a, int b, const std::vector<SortCriterion>& sortCriteria) const;
	int Partition(std::vector<int>& inRows, int low, int high, const std::vector<Database::SortCriterion>& sortCriteria);
	void QuickSort(std::vector<int>& rowsIn,int a,int b, const std::vector<Database::SortCriterion>& sortCriteria);

	std::vector<FieldDescriptor> m_schema;
	std::vector<std::vector<std::string>> m_rows;
	std::vector<MultiMap*> m_fieldIndex;
};

#endif