#include <string>

#include "Tuple.h"

using namespace std;

namespace model {
  
  Tuple::Tuple(int size) : _size(size) { 
    for (int i = 0; i < size; i++) {
      _tuple.push_back(0);
    }
  }
  
  void Tuple::setValue(int index, int value) {
    _tuple[index] = value;
  }
  
  int Tuple::getValue(int index) const {
    return _tuple[index];
  }

}