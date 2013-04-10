#include <vector>

#include "../model/Tuple.h"
#include "Trie.h"

using namespace std;

namespace util {
  
  Trie::Trie(int key, int depth) : _key(key), _depth(depth) {}

  void Trie::addToTrie(Tuple* tuple, const vector<int>& indeces, const vector<int>& depths, unsigned int currentIndex) {
    
    if (currentIndex >= indeces.size()) {
      _tuples.push_back(tuple);
      return;
    }
    
    
    int trieKey = tuple->getValue(indeces[currentIndex]);
    
    if (_sons.empty() || trieKey > _sons[_sons.size() - 1]->getKey()) {
      _sons.push_back(new Trie(trieKey, depths[currentIndex]));
    }
    
    _sons[_sons.size() - 1]->addToTrie(tuple, indeces, depths, currentIndex + 1);
  }
  
  int Trie::getKey() {
    return _key;
  }
  
  int Trie::getDepth() {
    return _depth;
  }
  
  unsigned int Trie::numberOfSons() {
    return _sons.size();
  }
  
  Trie* Trie::getSon(int index) {
    return _sons[index];
  }
  
  int Trie::numberOfTuples() {
    return _tuples.size();
  }
  
}