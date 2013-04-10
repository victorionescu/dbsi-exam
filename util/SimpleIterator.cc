#include <string>
#include <vector>

#include "../model/Relation.h"
#include "SimpleIterator.h"

namespace util {
  SimpleIterator::SimpleIterator(Relation* relation, const vector<int>& indecesToSort)
      : _relation(relation) {
    
    // Sorts the relation by indecesToSort.
    _relation->sortBy(indecesToSort);
    
    // Sets the current index to the beginning of the relation.
    _currentIndex = 0;
  }
  
  const Tuple& SimpleIterator::record() {
    return _relation->getRecord(_currentIndex);
  }
  
  void SimpleIterator::next() {
    _currentIndex++;
  }
  
  bool SimpleIterator::atEnd() {
    return (_currentIndex >= _relation->size());
  }
}