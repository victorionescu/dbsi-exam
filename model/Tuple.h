#ifndef TUPLE_H
#define TUPLE_H

#include <string>
#include <vector>

using namespace std;

namespace model {
  
  class Tuple {
  public:
    Tuple(int size);
    
    void setValue(int index, int value);
    
    int getValue(int index) const;
    
    int size() const;
  private:
    int _size;
    vector<int> _tuple;
  };
  
}

#endif