#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>

using namespace std;

namespace model {
  
  /*
    Tuple class used for holding database records. Fixed size.
  */
  class Tuple {
  public:
    // Create a new Tuple of a particular size.
    Tuple(int size);
    
    // Set the value at index.
    void setValue(int index, int value);
    
    // Retrieve value stored at index.
    int getValue(int index) const;
    
    // Retrieve size of the Tuple.
    int size() const;
  private:
    // Size of the Tuple.
    int _size;
    
    // Vector object storing the elements of the Tuple.
    vector<int> _tuple;
  };
  
}

#endif