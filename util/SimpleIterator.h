#ifndef SIMPLE_ITERATOR_H
#define SIMPLE_ITERATOR_H

#include <string>
#include <vector>

namespace model {
  class Tuple;
  class Relation;
}

using namespace std;

using model::Relation;
using model::Tuple;

namespace util {
  class SimpleIterator {
  public:
    SimpleIterator(const Relation& relation);
    
    const Tuple& record();
    
    void next();
    
    bool atEnd();
    
  private:
    const Relation& _relation;
    
    int _currentIndex;
  };
}

#endif