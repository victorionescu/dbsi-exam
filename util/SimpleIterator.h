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
  /* SimpleIterator class. Sorts the relation based on indecesToSort and outputs
     the values in order.
  */
  class SimpleIterator {
  public:
    SimpleIterator(Relation* relation, const vector<int>& indecesToSort);
    
    // Returns the record that the iterator is currently pointing at.
    const Tuple& record();
    
    // Moves the iterator to the next position.
    void next();
    
    // Checks if the iterator has finished its job.
    bool atEnd();
    
  private:
    // Relation on which the iterator acts.
    Relation* _relation;
    
    // Current index in the sorted relation.
    int _currentIndex;
  };
}

#endif