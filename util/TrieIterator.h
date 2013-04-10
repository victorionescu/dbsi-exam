#ifndef TRIE_ITERATOR_H
#define TRIE_ITERATOR_H

#include <vector>

#include "LinearIterator.h"
#include "Trie.h"

using namespace std;

namespace util {
  
  /*
    TrieContainer wrapper class over trie. Implements the comparison operators so that
    it can be sent as a type parameter to LinearIterator.
  */
  class TrieContainer {
  public:
    TrieContainer() : _trie(NULL) {}
    
    TrieContainer(Trie* trie) : _trie(trie) {}
    
    TrieContainer(const TrieContainer& other) : _trie(other._trie) {} 
    
    friend bool operator<(TrieContainer& t1, TrieContainer& t2);
    friend bool operator>(TrieContainer& t1, TrieContainer& t2);
    
    friend bool operator<=(TrieContainer& t1, TrieContainer& t2);
    friend bool operator>=(TrieContainer& t1, TrieContainer& t2);
    
    Trie* getTrie() {
      return _trie;
    }
  private:
    Trie* _trie;
  };
  
  
  /*
    TrieIterator class used for the leapfrog TrieJoin.
  */
  class TrieIterator {
  public:
    TrieIterator(Trie* trie);
    
    /*
      Key of the iterator on the current level.
    */
    int key();
    
    /*
      Is the iterator on the current level at the end?
    */
    bool atEnd();
    
    /*
      Move the iterator on the current level to the next position.
    */
    void next();
    
    /*
      Seek a key greater than or equal to seekKey on the current level.
    */
    void seek(int seekKey);
    
    /*
      Move down one level in the trie. (to the leftmost key)
    */
    void open();
    
    /*
      Move up one level in the trie.
    */
    void up();
    
    /*
      Depth of the current trie node. (based on join attribute order)
    */
    int depth();
    
    /*
      Number of tuples that terminate in this node. Non-zero only for leaves.
    */
    int numberOfTuples();
  private:
    /*
      Stack of linear operators.
    */
    vector<LinearIterator<TrieContainer>* > _iterators;
    
    /*
      Depth of the current trie node. (based on the join attribute order)
    */
    vector<int> _depth;
  };
}

#endif