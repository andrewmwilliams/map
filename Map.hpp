#ifndef AWILLI64_MAP_HPP
#define AWILLI64_MAP_HPP

#include <random>    //to generate random node height in insert()
#include <stdexcept> //to throw std::out_of_range in at()

#pragma GCC diagnostic ignored "-Wunknown-pragmas"     //tells clang to ignore the next pragma
#pragma GCC diagnostic ignored "-Wnon-template-friend" //ignore spurious warnings about non-templated friend functions

namespace cs540 {
  template <typename Key_T, typename Mapped_T>
  class Map {
  
    typedef std::pair<const Key_T, Mapped_T> ValueType;
    //forward declerations of nested classes
  public:  
    class Iterator;
    class ConstIterator;
    class ReverseIterator;

  private:
    class SentinelNode;
    class DataNode;

    //MAP METHOD PROTOTYPES
    //Constructors and Assignment Operator
  public:
    Map            (); 
    Map            (const Map &);
    Map &operator= (const Map &);
    Map            (std::initializer_list<std::pair<const Key_T, Mapped_T>>);
    ~Map           ();
    //************************************

    //Size
    size_t size  () const;
    bool   empty () const;
    //************************************

    //Iterators
    Iterator        begin  ();
    Iterator        end    ();
    ConstIterator   begin  () const;
    ConstIterator   end    () const;
    ReverseIterator rbegin ();
    ReverseIterator rend   ();
    //************************************

    //Element Access
    Iterator       find        (const Key_T &);
    ConstIterator  find        (const Key_T &) const;
    Mapped_T       &at         (const Key_T &);
    const Mapped_T &at         (const Key_T &) const;
    Mapped_T       &operator[] (const Key_T &);
    //************************************

    //Modifiers
    std::pair<Iterator, bool> insert (const ValueType &);
    template <typename IT_T>
    void                    insert (IT_T range_beg, IT_T range_end);
    void                    erase  (Iterator pos);
    void                    erase  (const Key_T &);
    void                    clear  ();
    //************************************

    void print() const;
    void traceInsert(const ValueType &);


    //Comparison
    friend bool operator== (const Map &lhs, const Map &rhs) {
      if (lhs.numNodes != rhs.numNodes)
	return false;
      
      auto lhsIt = lhs.begin();
      auto rhsIt = rhs.begin();
      while (lhsIt != lhs.end()) {
	//if ((*lhsIt).first != (*rhsIt).first || (*lhsIt).second != (*rhsIt).second)
	if (!(lhsIt->first == rhsIt->first) || !(lhsIt->second == rhsIt->second))
	  return false;
	lhsIt++;
	rhsIt++;
      }
      return true;
    }

    friend bool operator!= (const Map &lhs, const Map &rhs) {
      return (!(lhs == rhs));
    }
    
    friend bool operator<  (const Map &lhs, const Map &rhs) {
      auto lhsIt = lhs.begin();
      auto rhsIt = rhs.begin();
      while (lhsIt != lhs.end() && rhsIt != rhs.end()) {
	if ((*lhsIt) < (*rhsIt))
	  return true;
	lhsIt++;
	rhsIt++;
      }
      if (lhs.numNodes < rhs.numNodes)
	return true;
      return false;
    }

    class ConstIterator {
      friend bool operator== (const Iterator &, const ConstIterator &);
      friend bool operator== (const ConstIterator &, const Iterator &);
      friend bool operator!= (const Iterator &, const ConstIterator &);
      friend bool operator!= (const ConstIterator &, const Iterator &);
    public:
      ConstIterator (DataNode *nodeIn) : cur (nodeIn) {}
   
      ConstIterator (const Iterator &inIt) : cur (inIt.cur) {}

      ConstIterator   &operator++ ();
      ConstIterator   operator++  (int);
      ConstIterator   &operator-- ();
      ConstIterator   operator--  (int);
      const ValueType &operator*  () const;
      const ValueType *operator-> () const;

      friend bool operator== (const ConstIterator &lhs, const ConstIterator &rhs) {
	return (lhs.cur == rhs.cur);
      }

      friend bool operator!= (const ConstIterator &lhs, const ConstIterator &rhs) {
	return (lhs.cur != rhs.cur);
      }

    private:
      DataNode *cur;
    }; //end class ConstIterator
    
    class Iterator {
      friend ConstIterator::ConstIterator(const Iterator &);
     
    public:
      Iterator(DataNode *nodeIn) : cur(nodeIn) {}

      Iterator  &operator++ ();
      Iterator  operator++  (int);
      Iterator  &operator-- ();
      Iterator  operator--  (int);
      ValueType &operator*  () const;
      ValueType *operator-> () const;

      friend bool operator== (const Iterator &lhs, const Iterator &rhs) {
	return (lhs.cur == rhs.cur);
      }

      friend bool operator== (const Iterator &lhs, const ConstIterator &rhs) {
	return (lhs.cur == rhs.cur);
      }

      friend bool operator== (const ConstIterator &lhs, const Iterator &rhs) {
	return (lhs.cur == rhs.cur);
      }

      friend bool operator!= (const Iterator &lhs, const Iterator &rhs) {
	return (lhs.cur != rhs.cur);
      }

      friend bool operator!= (const Iterator &lhs, const ConstIterator &rhs) {
	return (lhs.cur != rhs.cur);
      }

      friend bool operator!= (const ConstIterator &lhs, const Iterator &rhs) {
	return (lhs.cur != rhs.cur);
      }
         
    private:
      DataNode *cur;
    }; //end class Iterator

    class ReverseIterator {
    public:
      ReverseIterator(DataNode *nodeIn) : cur(nodeIn) {}
 
      ReverseIterator &operator++ ();
      ReverseIterator operator++  (int);
      ReverseIterator &operator-- ();
      ReverseIterator operator--  (int);
      ValueType       &operator*  () const;
      ValueType       *operator-> () const;

      friend bool operator== (const ReverseIterator &op1, const ReverseIterator &op2) {
	return (op1.cur == op2.cur);
      }

      friend bool operator!= (const ReverseIterator &op1, const ReverseIterator &op2) {
	return (op1.cur != op2.cur);
      }

    private:
      DataNode *cur;
    }; //end class ReverseIterator

  private:
    static const int MAX_LEVELS = 32;

    SentinelNode *head;
    SentinelNode *tail;

    size_t numNodes;
    int height;

    //for randomly generating insert height
    std::random_device r;
    std::default_random_engine e;
    //************************************

    class SentinelNode {
    public:
      SentinelNode() : prev(nullptr) {
	for (int i = 0; i < MAX_LEVELS; ++i)
	  nextNodes[i] = nullptr;
      }

      DataNode *nextNodes[MAX_LEVELS];
      DataNode *prev;
    }; //end class SentinelNode

    class DataNode : public SentinelNode {
    public:
      DataNode() = delete;
      DataNode(ValueType valueIn) : value(valueIn) {}

      ValueType value;
    }; //end class DataNode
  }; //end class Map

  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::Map() {
    numNodes = 0;
    height = 0;
    head = new SentinelNode;
    tail = new SentinelNode;
    for (int curLevel = 0; curLevel < MAX_LEVELS; ++curLevel)
      head->nextNodes[curLevel] = static_cast<DataNode*>(tail);
    tail->prev = static_cast<DataNode*>(head);
    
    e.seed(r());
  }   

  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::Map(const Map &mapIn) {
    numNodes = 0;
    height = 0;
    head = new SentinelNode;
    tail = new SentinelNode;
    for (int curLevel = 0; curLevel < MAX_LEVELS; ++curLevel)
      head->nextNodes[curLevel] = static_cast<DataNode*>(tail);
    tail->prev = static_cast<DataNode*>(head);
    e.seed(r());
    
    DataNode *trav = mapIn.head->nextNodes[0];
    while (trav != static_cast<DataNode*>(mapIn.tail)) {
      insert(trav->value);
      trav = trav->nextNodes[0];
    }
  }
  
  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>& Map<Key_T, Mapped_T>::operator=(const Map<Key_T, Mapped_T> &mapIn) {
    if (&mapIn != this) {  //check for (and ignore) self assignment
      clear(); 
      DataNode *trav = mapIn.head->nextNodes[0];

      for (int curLevel = 0; curLevel < MAX_LEVELS; ++curLevel)
	head->nextNodes[curLevel] = static_cast<DataNode*>(tail);
      tail->prev = static_cast<DataNode*>(head);
      
      while (trav != static_cast<DataNode*>(mapIn.tail)) {
	insert(trav->value);
	trav = trav->nextNodes[0];
      }
    }    
    return *this;
  }
  
  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> initList) {
    numNodes = 0;
    height = 0;
    e.seed(r);
    head = new SentinelNode;
    tail = new SentinelNode;
    for (int curLevel = 0; curLevel < MAX_LEVELS; ++curLevel)
      head->nextNodes[curLevel] = static_cast<DataNode*>(tail);
    tail->prev = static_cast<DataNode*>(head);    

    for (auto it = initList.begin(); it != initList.end(); ++it)
      insert ({it->first, it->second});
  }
  
  template <typename Key_T, typename Mapped_T>
  Map<Key_T, Mapped_T>::~Map<Key_T, Mapped_T>() {
    clear();
    delete head;
    delete tail;
  }
  
  template <typename Key_T, typename Mapped_T>
  size_t Map<Key_T, Mapped_T>::size() const {
    return numNodes;
  }

  template <typename Key_T, typename Mapped_T>
  bool Map<Key_T, Mapped_T>::empty() const {
    return (numNodes == 0);
  }
  
  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin() {
    Iterator retIt (head->nextNodes[0]);
    return retIt;
  }
  
  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end () {
    Iterator retIt(static_cast<DataNode*>(tail));
    return retIt;  
  }
    
  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const {
    ConstIterator retIt (head->nextNodes[0]);
    return retIt;
  }

  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const {
    ConstIterator retIt(static_cast<DataNode*>(tail));
    return retIt;
  }

  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin() {
    ReverseIterator retIt(tail->prev);
    return retIt;
  }

  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend() {
    ReverseIterator retIt(static_cast<DataNode*>(head));
    return retIt;
  }

  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find (const Key_T &keyIn) {
    int curLevel = height - 1;
    DataNode *trav = static_cast<DataNode*>(head);
    while (curLevel >= 0) {
      while (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first < keyIn)
	trav = trav->nextNodes[curLevel];
      if (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first == keyIn)
	return trav->nextNodes[curLevel];
      --curLevel;
    }
    return static_cast<DataNode*>(tail);
  }

  template <typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find (const Key_T &keyIn) const {
    int curLevel = height - 1;
    DataNode *trav = static_cast<DataNode*>(head);
    while (curLevel >= 0) {
      while (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first < keyIn)
	trav = trav->nextNodes[curLevel];
      if (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first == keyIn)
	return trav->nextNodes[curLevel];
      --curLevel;
    }
    return static_cast<DataNode*>(tail);    
  }
  
  template <typename Key_T, typename Mapped_T>
  Mapped_T &Map<Key_T, Mapped_T>::at (const Key_T &keyIn) {
    auto retIt = find(keyIn);
    if (retIt == end())
      throw std::out_of_range("value not found while using at()");
    return (*retIt).second;
  }

  template <typename Key_T, typename Mapped_T>
  const Mapped_T &Map<Key_T, Mapped_T>::at (const Key_T &keyIn) const {
    auto retIt = find(keyIn);
    if (retIt == end())
      throw std::out_of_range("value not found while using at()");
    return (*retIt).second;
    
  }

  template <typename Key_T, typename Mapped_T>
  Mapped_T &Map<Key_T, Mapped_T>::operator[] (const Key_T &keyIn) {
    auto retIt = find(keyIn);
    if (retIt == end()) {  //if key isn't in map, create a new entry for it
      Mapped_T newMapped{};
      ValueType newVal(keyIn, newMapped);
      auto retPair = insert(newVal);
      return (*retPair.first).second;
    }
    return (*retIt).second;
  }

  template <typename Key_T, typename Mapped_T>
  std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>  Map<Key_T, Mapped_T>::insert(const ValueType &valueIn) {
    //check that valueIn.key is not already in map
    auto findIt = find(valueIn.first);
    if (findIt != end()) {
      std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>retPair = {findIt, false};
      return retPair;
    }
    std::uniform_int_distribution<int> u(0,1);
    int insertHeight = 1;
    bool repeat = true;
   
    while (repeat && insertHeight < MAX_LEVELS) {
      repeat = u(e);
      insertHeight++;
     }


    DataNode *newNode = new DataNode(valueIn);
    ++numNodes;
    if (insertHeight > height)
      height = insertHeight;


    //int curLevel = insertHeight - 1;
    int curLevel = height - 1;
    DataNode *trav = static_cast<DataNode*>(head);
    DataNode *tmp;
    
    while (curLevel >= 0) {
      while (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first < valueIn.first)
	trav = trav->nextNodes[curLevel];
      if (curLevel < insertHeight) {
	tmp = trav->nextNodes[curLevel];
	trav->nextNodes[curLevel] = newNode;
	newNode->nextNodes[curLevel] = tmp;
	if (curLevel == 0) {
	  tmp->prev = newNode;
	  newNode->prev = trav;
	}
      }
      --curLevel;
    }
    
    std::pair<Map<Key_T, Mapped_T>::Iterator, bool> retPair ({newNode}, true);
    return retPair;
  }

  template <typename Key_T, typename Mapped_T>
  void  Map<Key_T, Mapped_T>::traceInsert(const ValueType &valueIn) {
    //check that valueIn.key is not already in map
    auto findIt = find(valueIn.first);
    if (findIt != end()) {
      //std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool>retPair = {findIt, false};
      //return retPair;
      return;
    }
    

    int insertHeight = 1;
    bool repeat = true;
    std::uniform_int_distribution<int> u(0,1);    

    while (repeat && insertHeight < MAX_LEVELS) {
      repeat = u(e);
      insertHeight++;
    }
    
    printf("inserting %d at height %d\n", valueIn.first, insertHeight);

    DataNode *newNode = new DataNode(valueIn);
    ++numNodes;
    if (insertHeight > height)
      height = insertHeight;
 
    //int curLevel = insertHeight - 1;
    int curLevel = height - 1;
    DataNode *trav = static_cast<DataNode*>(head);
    DataNode *tmp;
    
    while (curLevel >= 0) {
      //printf("at level %d\n", curLevel);
      while (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first < valueIn.first) {
	printf("moving to level %d node %d\n", curLevel, trav->nextNodes[curLevel]->value.first);
	trav = trav->nextNodes[curLevel];
	//printf("moving to node %d\n", trav->value.first);
      }
      if (curLevel < insertHeight) {
	tmp = trav->nextNodes[curLevel];
	trav->nextNodes[curLevel] = newNode;
	newNode->nextNodes[curLevel] = tmp;
	if (curLevel == 0) {
	  tmp->prev = newNode;
	  newNode->prev = trav;
	}
      }
      --curLevel;
    }
    printf("\n");


  }



  
  template <typename Key_T, typename Mapped_T>
  template <typename IT_T>
  void Map<Key_T, Mapped_T>::insert (IT_T range_beg, IT_T range_end) {
    for (IT_T trav = range_beg; trav != range_end; trav++) {
      insert({(*trav).first, (*trav).second});
    }
  }

  template <typename Key_T, typename Mapped_T>
  void Map<Key_T, Mapped_T>::erase (Map<Key_T, Mapped_T>::Iterator pos) {
    erase((*pos).first);
  }
  
  template <typename Key_T, typename Mapped_T>
  void Map<Key_T, Mapped_T>::erase (const Key_T &keyIn) {
    DataNode *toDelete = nullptr;
    
    int curLevel = height - 1;
    DataNode *trav = static_cast<DataNode*>(head);

    while (curLevel >= 0) {
      while (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) &&  trav->nextNodes[curLevel]->value.first < keyIn)
	trav = trav->nextNodes[curLevel];
      if (trav->nextNodes[curLevel] != static_cast<DataNode*>(tail) && trav->nextNodes[curLevel]->value.first == keyIn) {
	toDelete = trav->nextNodes[curLevel];
	trav->nextNodes[curLevel] = trav->nextNodes[curLevel]->nextNodes[curLevel];
	if (curLevel == 0)
	  trav->nextNodes[curLevel]->prev = trav;
      }
      --curLevel;
    }

    if (toDelete == nullptr)
      throw std::out_of_range("attempted to delete a key which is not in the map");
    delete toDelete;
    --numNodes;

    //update height in case we just deleted the only elem from the top level
    for (curLevel = height - 1; curLevel >= 0; --curLevel) {
      if (head->nextNodes[curLevel] != static_cast<DataNode*>(tail)) {
	height = curLevel + 1;
	break;
      }
    }
  }
  
  template <typename Key_T, typename Mapped_T>
  void Map<Key_T, Mapped_T>::clear() {
    DataNode *trav = head->nextNodes[0];
    DataNode *prev;
    while (trav != static_cast<DataNode*>(tail)) {
      prev = trav;
      trav = trav->nextNodes[0];
      delete prev;
    }
    
    for (int curLevel = 0; curLevel < MAX_LEVELS; ++curLevel)
      head->nextNodes[curLevel] = static_cast<DataNode*>(tail);
    tail->prev = static_cast<DataNode*>(head);
    height = 0;
    numNodes = 0;
  }

  template <typename Key_T, typename Mapped_T>
  void Map<Key_T, Mapped_T>::print() const {
    DataNode *trav;
    printf("numNodes=%lu, height=%d\n", numNodes, height);
    for(int i = height - 1; i >= 0; --i) {
      printf("level %d:", i);
      trav = head->nextNodes[i];
      while (trav != static_cast<DataNode*>(tail)) {
	printf("-->{%d, %d}", trav->value.first, trav->value.second);
	trav = trav->nextNodes[i];
      }
      printf("\n");
    }
    printf("reverse level 0:");
    trav = tail->prev;
    while (trav != static_cast<DataNode*>(head)) {
      printf("-->{%d, %d}", trav->value.first, trav->value.second);
      trav = trav->prev;
    }
    printf("\n**************************\n");
  }


  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator &Map<Key_T, Mapped_T>::Iterator::operator++() {
    cur = cur->nextNodes[0];
    return *this;
  }
  
  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::operator++(int) {
    auto tmp = cur;
    cur = cur->nextNodes[0];
    return tmp;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator &Map<Key_T, Mapped_T>::Iterator::operator--() {
    cur = cur->prev;
    return *this;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::operator--(int) {
    auto tmp = cur;
    cur = cur->prev;
    return tmp;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::Iterator::operator* () const {
    return cur->value;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ValueType *Map<Key_T, Mapped_T>::Iterator::operator->() const {
    return &cur->value;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator &Map<Key_T, Mapped_T>::ConstIterator::operator++() {
    cur = cur->nextNodes[0];
    return *this;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::ConstIterator::operator++(int) {
    auto tmp = cur;
    cur = cur->nextNodes[0];
    return tmp;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator &Map<Key_T, Mapped_T>::ConstIterator::operator--() {
    cur = cur->prev;
    return *this;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::ConstIterator::operator--(int) {
    auto tmp = cur;
    cur = cur->prev;
    return tmp;
  }

  template<typename Key_T, typename Mapped_T>
  const typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::ConstIterator::operator*() const {
    return cur->value;
  }

  template<typename Key_T, typename Mapped_T>
  const typename Map<Key_T, Mapped_T>::ValueType *Map<Key_T, Mapped_T>::ConstIterator::operator->() const {
    return &cur->value;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ReverseIterator &Map<Key_T, Mapped_T>::ReverseIterator::operator++() {
    cur = cur->prev;
    return *this;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::ReverseIterator::operator++(int) {
    auto tmp = cur;
    cur = cur->prev;
    return tmp;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ReverseIterator &Map<Key_T, Mapped_T>::ReverseIterator::operator--() {
    cur = cur->nextNodes[0];
    return *this;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::ReverseIterator::operator--(int) {
    auto tmp = cur;
    cur = cur->nextNodes[0];
    return tmp;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::ReverseIterator::operator*() const {
    return cur->value;
  }

  template<typename Key_T, typename Mapped_T>
  typename Map<Key_T, Mapped_T>::ValueType *Map<Key_T, Mapped_T>::ReverseIterator::operator->() const {
    return &cur->value;
  }
} //end namespace cs540
#endif
