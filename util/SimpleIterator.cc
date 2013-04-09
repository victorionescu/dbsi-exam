#include <string>
#include <vector>

#include "../model/Relation.h"
#include "SimpleIterator.h"

namespace util {
  SimpleIterator::SimpleIterator(const Relation& relation)
      : _relation(relation) {
    
    _currentIndex = 0;
  }
  
  const Tuple& SimpleIterator::record() {
    return _relation.getRecord(_currentIndex);
  }
  
  void SimpleIterator::next() {
    _currentIndex++;
  }
  
  bool SimpleIterator::atEnd() {
    return (_currentIndex >= _relation.size());
  }
}