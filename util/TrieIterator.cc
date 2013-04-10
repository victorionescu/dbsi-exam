#include <cassert>

#include "TrieIterator.h"

using namespace std;

namespace util {
  /*
    Comparison operators for TrieContainer class.
  */
  bool operator<(TrieContainer& t1, TrieContainer& t2) {
    return t1._trie->getKey() < t2._trie->getKey();
  }
  
  bool operator>(TrieContainer& t1, TrieContainer& t2) {
    return t1._trie->getKey() > t2._trie->getKey();
  }
  
  bool operator<=(TrieContainer& t1, TrieContainer& t2) {
    return t1._trie->getKey() <= t2._trie->getKey();
  }
  
  bool operator>=(TrieContainer& t1, TrieContainer& t2) {
    return t1._trie->getKey() >= t2._trie->getKey();
  }
  
  TrieIterator::TrieIterator(Trie* trie) {
    _depth.push_back(-1);
    
    vector<TrieContainer> trieContainers;
    
    for (unsigned int i = 0; i < trie->numberOfSons(); i++) {
      TrieContainer trieContainer(trie->getSon(i));
      trieContainers.push_back(trieContainer);
    }
    
    _depth.push_back(trie->getSon(0)->getDepth());
    _iterators.push_back(new LinearIterator<TrieContainer>(trieContainers));
  }
  
  int TrieIterator::key() {
    assert(!atEnd());
    
    return _iterators[_iterators.size() - 1]->key().getTrie()->getKey();
  }
  
  bool TrieIterator::atEnd() {
    return (_iterators.empty() || _iterators[_iterators.size() - 1]->atEnd());
  }
  
  void TrieIterator::next() {
    assert(!atEnd());
    
    _iterators[_iterators.size() - 1]->next();
  }
  
  void TrieIterator::seek(int seekKey) {
    assert(!atEnd());
    
    /* Wrap integer in a TrieContainer value */
    Trie* keyTrie = new Trie(seekKey, -1);
    
    TrieContainer keyContainer(keyTrie);
    
    _iterators[_iterators.size() - 1]->seek(keyContainer);
  }
  
  void TrieIterator::open() {
    assert(!atEnd());
    
    Trie* trie = _iterators[_iterators.size() - 1]->key().getTrie();
    
    vector<TrieContainer> trieContainers;
    
    /* Populate the LinearIterator with the sons found in the trie. */
    for (unsigned int i = 0; i < trie->numberOfSons(); i++) {
      TrieContainer trieContainer(trie->getSon(i));
      trieContainers.push_back(trieContainer);
    }
    
    _depth.push_back(trie->getSon(0)->getDepth());
    _iterators.push_back(new LinearIterator<TrieContainer>(trieContainers));
  }
  
  void TrieIterator::up() {
    _depth.pop_back();
    _iterators.pop_back();
  }
  
  int TrieIterator::depth() {
    return _depth[_depth.size() - 1];
  }
  
  int TrieIterator::numberOfTuples() {
    assert(!atEnd());
    
    return _iterators[_iterators.size() - 1]->key().getTrie()->numberOfTuples();
  }
}