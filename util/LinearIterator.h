#ifndef LINEAR_ITERATOR_H
#define LINEAR_ITERATOR_H

#include <vector>

using namespace std;

namespace util {
  
  struct Treap {
    int key, priority;
    
    Treap *left, *right, *dad;
    
    int minVal, maxVal;
    
    bool isNil;
    
    Treap() {}
    
    Treap(int key, int priority, Treap* left, Treap* right, Treap* dad, bool isNil) {
      this->key = key;
      this->priority = priority;
      
      this->left = left;
      this->right = right;
      this->dad = dad;
      
      this->minVal = this->key;
      this->maxVal = this->key;
      
      this->isNil = isNil;
    }
  };
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  
  class LinearIterator {
  public:
    LinearIterator(const vector<int>& keyCollection);
    
    int key();
    
    void next();
    
    bool atEnd();
    
    void seek(int seekKey);
  
  private:
    Treap* _bst;
    
    Treap* _currentPosition;
    
    vector<int> _stack;
  };
}


#endif