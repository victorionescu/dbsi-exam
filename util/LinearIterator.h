#ifndef LINEAR_ITERATOR_H
#define LINEAR_ITERATOR_H

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

namespace util {
  
  /*
    We will use treaps as the balanced search trees for our iterator.
    For the purpose of creating TrieIterator's we will create a template.
  */
  template <class T>
  struct Treap {
    T key;
    int priority;
    
    Treap<T> *left, *right, *dad;
    
    T minVal, maxVal;
    
    // If the node is terminal.
    bool isNil;
    
    Treap() {}
    
    Treap(int priority, Treap<T>* left, Treap<T>* right, Treap<T>* dad, bool isNil) {
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
  /*
    LinearIterator class implemented via treaps. Complexity per operation is O(log N) and amortised
    complexity per M successive accesses is O(1 + log(N/M))
  */
  template <class T>
  class LinearIterator {
  public:
    LinearIterator(const vector<T>& keyCollection);
    
    T key();
    
    void next();
    
    bool atEnd();
    
    void seek(T seekKey);
  
  private:
    // Binary search tree (balanced) used to implement this iterator.
    Treap<T>* _bst;
    
    // Pointer to the current position of the iterator
    Treap<T>* _currentPosition;
    
    // Stack used to move around the tree.
    vector<int> _stack;
  };
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /// IMPLEMENTATION
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  /*
    Left rotation of the treap.
  */
  template <class T>
  void rotateLeft(Treap<T>* node) {
    
    Treap<T>* B = node->right->left;
    
    
    Treap<T>* rightNode = node->right;
    
    B->dad = node;
    node->right = B;
    rightNode->left = node;
    
    rightNode->dad = node->dad;
    node->dad = rightNode;
    
    if (rightNode->dad != NULL) {
      if (node == rightNode->dad->left) {
        rightNode->dad->left = rightNode;
      } else {
        rightNode->dad->right = rightNode;
      }
    }
    
    if (!B->isNil) {
      node->maxVal = B->maxVal;
    } else {
      node->maxVal = node->key;
    }
    
    rightNode->minVal = node->minVal;
  }
  
  /*
    Right rotation of the treap.
  */
  template <class T>
  void rotateRight(Treap<T>* node) {
    
    Treap<T>* B = node->left->right;
    
    
    Treap<T>* leftNode = node->left;
    
    B->dad = node;
    node->left = B;
    leftNode->right = node;
    
    leftNode->dad = node->dad;
    node->dad = leftNode;
    
    if (leftNode->dad != NULL) {
      if (node == leftNode->dad->left) {
        leftNode->dad->left = leftNode;
      } else {
        leftNode->dad->right = leftNode;
      }
    }
    
    if (!B->isNil) {
      node->minVal = B->minVal;
    } else {
      node->minVal = node->key;
    }
    
    leftNode->maxVal = node->maxVal;
  }
  
  /*
    Balance treap.
  */
  template <class T>
  void balance(Treap<T>* node) {
    if (node->left->priority > node->priority)
      rotateRight<T>(node);
    else if (node->right->priority > node->priority)
      rotateLeft<T>(node);
  }
  
  /*
    Insert into treap.
  */
  template <class T>
  void insert(Treap<T>* node, T key, int priority) {
    if (node->isNil) {
      node->isNil = false;
      
      node->key = key;
      node->priority = priority;
      
      node->left =  new Treap<T>(-1, NULL, NULL, node, true);
      node->right = new Treap<T>(-1, NULL, NULL, node, true);
      
      node->minVal = key;
      node->maxVal = key;
      
      return;
    }
    
    
    if (key < node->key) {
      insert(node->left, key, priority);
      
      node->minVal = node->left->minVal;
    
    } else {
      insert(node->right, key, priority);
      
      node->maxVal = node->right->maxVal;
    }
      
    balance(node);
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  /*
    Initialize an iterator by adding all the elements to the treap.
    First key to point at is the leftmost node in the treap.
  */
  template <class T>
  LinearIterator<T>::LinearIterator(const vector<T>& keyCollection) { 
    _bst = new Treap<T>(-1, NULL, NULL, NULL, true);
    _bst->right = new Treap<T>(-1, NULL, NULL, _bst, true);
    
    srand(time(NULL));
    
    for (unsigned int i = 0; i < keyCollection.size(); i++)
      insert(_bst->right, keyCollection[i], rand() + 1);
      
      
    _currentPosition = _bst->right;
    
    _stack.push_back(1);
    
    while (!_currentPosition->left->isNil){
      _stack.push_back(1);
      
      _currentPosition = _currentPosition->left;
    }
    
  }
  
  /*
    Return the key the iterator is pointing at.
  */
  template <class T>
  T LinearIterator<T>::key() {
    assert(!atEnd());
    
    return _currentPosition->key;
  }
  
  /*
    Move the iterator to the next position using the
    Left -> Node -> Right order.
  */
  template <class T>
  void LinearIterator<T>::next() {
    assert(!atEnd());
    
    if (_stack[_stack.size() - 1] == 1 && !_currentPosition->right->isNil) {
      _stack[_stack.size() - 1]++;
      
      _stack.push_back(1);
      _currentPosition = _currentPosition->right;
      
      while (!_currentPosition->left->isNil) {
        _stack.push_back(1);
        _currentPosition = _currentPosition->left;
      }
    } else {
      _stack[_stack.size() - 1] = 2;
      
      while (!_currentPosition->isNil && _stack[_stack.size() - 1] == 2) {
        _stack.pop_back();
        _currentPosition = _currentPosition->dad;
      }
    }
  }
  
  /*
    Iterator is at end if a terminal node has been reached.
  */
  template <class T>
  bool LinearIterator<T>::atEnd() {
    return _currentPosition->isNil;
  }
  
  
  template <class T>
  void LinearIterator<T>::seek(T seekKey) {
    /*
      Move up the tree until we find a node from which we can start sinking.
    */
    while (!_currentPosition->isNil && _currentPosition->maxVal < seekKey) {
      _stack.pop_back();
      _currentPosition = _currentPosition->dad;
    }
    
    if (_currentPosition->isNil) return;
    
    bool cont = true;
    
    /*
      Go right until we find a larger key than seekKey, then sink to the left
      as much as we can.
    */
    while (cont) {
      
      cont = false;
    
      while (_currentPosition->key < seekKey) {
        _stack[_stack.size() - 1] = 2;
        _stack.push_back(1);
        _currentPosition = _currentPosition->right;
        
        cont = true;
      }
    
    
    
      while (!_currentPosition->left->isNil && _currentPosition->left->maxVal >= seekKey) {
        _stack.push_back(1);
        _currentPosition = _currentPosition->left;
        
        cont = true;
      }
    }
    
  }
}


#endif