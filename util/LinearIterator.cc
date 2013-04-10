#include "LinearIterator.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

namespace util {
  
  void rotateLeft(Treap* node) {
    Treap* A = node->left;
    Treap* B = node->right->left;
    Treap* C = node->right->right;
    
    Treap* rightNode = node->right;
    
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
  
  void rotateRight(Treap* node) {
    Treap* A = node->left->left;
    Treap* B = node->left->right;
    Treap* C = node->right;
    
    Treap* leftNode = node->left;
    
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
  
  void balance(Treap* node) {
    if (node->left->priority > node->priority)
      rotateRight(node);
    else if (node->right->priority > node->priority)
      rotateLeft(node);
  }
  
  void insert(Treap* node, int key, int priority) {
    if (node->isNil) {
      node->isNil = false;
      
      node->key = key;
      node->priority = priority;
      
      node->left =  new Treap(0, -1, NULL, NULL, node, true);
      node->right = new Treap(0, -1, NULL, NULL, node, true);
      
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
  
  void printTreap(Treap* node) {
    if (node->isNil) return;
    
    printTreap(node->left);
    
    printf("%d ", node->key);
    
    printTreap(node->right);
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  
  LinearIterator::LinearIterator(const vector<int>& keyCollection) { 
    _bst = new Treap(0, -1, NULL, NULL, NULL, true);
    _bst->right = new Treap(0, -1, NULL, NULL, _bst, true);
    
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
  
  int LinearIterator::key() {
    assert(!atEnd());
    
    return _currentPosition->key;
  }
  
  void LinearIterator::next() {
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
  
  bool LinearIterator::atEnd() {
    return _currentPosition->isNil;
  }
  
  
  void LinearIterator::seek(int seekKey) {
    while (!_currentPosition->isNil && _currentPosition->maxVal < seekKey) {
      _stack.pop_back();
      _currentPosition = _currentPosition->dad;
    }
    
    
    if (_currentPosition->isNil) return;
    
    bool cont = true;
    
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