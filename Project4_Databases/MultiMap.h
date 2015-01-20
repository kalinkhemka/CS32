#ifndef MULTIMAP_INCLUDED
#define MULTIMAP_INCLUDED

#include <string>

class	MultiMap	
{
private:
	struct Node
	{
		Node(std::string inKey, unsigned int inValue)
		{
			key = inKey;
			value = inValue;
			left = right = prev = nullptr;
		}
		std::string key;
		unsigned int value;
		Node* left;
		Node* right;
		Node* prev;
	};
	Node* getStartTree() const;
public:	
	//	You	must	implement	this	public	nested	MultiMap::Iterator	class
	class	Iterator	
	{	
	public:	
		Iterator();		//	You	must	have	a	default	constructor	
		Iterator(Node* start);
		bool	valid()	const;	
		std::string	getKey()	const;	
		unsigned	int	getValue()	const;
		bool	next();	
		bool	prev();	
	private:
		Node* m_node;
	};	
	
	MultiMap();	
	~MultiMap();	
	void	clear();	
	void	insert(std::string	key,	unsigned int value);	
	Iterator	findEqual(std::string	key)	const;	
	Iterator	findEqualOrSuccessor(std::string	key)	const;	
	Iterator	findEqualOrPredecessor(std::string	key)	const;
	
private:	
	//	To	prevent	Multimaps	from	being	copied	or	assigned,	declare	these	members	
	//	private	and	do	not	implement	them.	
	MultiMap(const	MultiMap&	other);	
	MultiMap&	operator=(const	MultiMap&	rhs);
	void clearMap(Node* input);
	Node* m_root;
};
#endif


//#ifndef MULTIMAP_INCLUDED
//#define MULTIMAP_INCLUDED
//
//// Implementation of MultiMap using std::multimap
//
//// This is provided solely so that you can test Database even if you
//// are not confident that your implementation of MultiMap is correct.
//// Of course, since it uses std::multimap, the spec forbids you from using
//// this as the MultiMap implementation you turn in.
//
//// All the implementation is in this file, so to use it, you do not need
//// a MultiMap.cpp file with it, or if you had one, it can be an empty file.
//
//#include <map>
//#include <string>
//#include <utility> // for make_pair
//#include <cassert>
//
//class MultiMap
//{
//private:
//	typedef std::multimap<std::string, unsigned int> MMap;
//	typedef MMap::const_iterator MMapIterator;
//public:
//	class Iterator
//	{
//	public:
//		Iterator() : m_mmap(nullptr) {}
//		Iterator(const MMap& mm, MMapIterator iter)
//		 : m_mmap(iter != mm.end() ? &mm : nullptr), m_iter(iter) {}
//		bool valid() const { return m_mmap != nullptr; }
//		std::string getKey() const { assert(valid()); return m_iter->first; }
//		unsigned int getValue() const { assert(valid()); return m_iter->second; }
//		bool next()
//		{
//			if (!valid())
//				return false;
//			if (++m_iter != m_mmap->end())
//				return true;
//			m_mmap = nullptr;
//			return false;
//		}
//		bool prev()
//		{
//			if (!valid())
//				return false;
//			if (m_iter != m_mmap->begin())
//			{
//				m_iter--;
//				return true;
//			}
//			m_mmap = nullptr;
//			return false;
//		}
//	private:
//		  // Class invariant:  m_mmap == nullptr || m_iter != m_mmap->end()
//		const MMap* m_mmap;
//		MMapIterator m_iter;
//	};
//
//	MultiMap() {}
//	~MultiMap() {}
//	void clear() { m_mmap.clear(); }
//	void insert(std::string key, unsigned int value)
//	{ m_mmap.insert(make_pair(key, value)); }
//	Iterator findEqual(std::string key) const
//	{
//		MMapIterator p = m_mmap.lower_bound(key);
//		if (p == m_mmap.end()  ||  p->first != key)
//			return Iterator();
//		return Iterator(m_mmap, p);
//	}
//	Iterator findEqualOrSuccessor(std::string key) const
//	{
//		MMapIterator p = m_mmap.lower_bound(key);
//		if (p == m_mmap.end())
//			return Iterator();
//		return Iterator(m_mmap, p);
//	}
//	Iterator findEqualOrPredecessor(std::string key) const
//	{
//		MMapIterator p = m_mmap.upper_bound(key);
//		if (p == m_mmap.begin())
//			return Iterator();
//		return Iterator(m_mmap, --p);
//	}
//
//private:
//	MMap m_mmap;
//
//	  // To prevent MultiMaps from being copied or assigned, declare these members
//	  // private and do not implement them.
//	MultiMap(const MultiMap& other);
//	MultiMap& operator=(const MultiMap& rhs);
//};
//
//#endif // MULTIMAP_INCLUDED