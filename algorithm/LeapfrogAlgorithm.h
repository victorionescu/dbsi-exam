#ifndef LEAPFROG_ALGORITHM_H
#define LEAPFROG_ALGORITHM_H

#include <string>
#include <vector>

#include "JoinAlgorithm.h"

namespace model {
  class Relation;
}

namespace util {
  class Trie;
  class TrieIterator;
}

using namespace std;

using model::Relation;
using util::Trie;
using util::TrieIterator;

namespace algorithm {
  /*
    Class implementing the Leapfrog TrieJoin algorithm
  */
  class LeapfrogAlgorithm : public JoinAlgorithm {
  public:
    LeapfrogAlgorithm();
    
    Relation* joinedRelation(vector<Relation*> relations,
                             vector<string> attributes);
                             
  private:
    /*
      Init leapfrog on the current level.
    */
    void leapfrogInit();
    
    /*
      Search for a match on the current level.
    */
    void leapfrogSearch();
    
    /*
      Move one position then search for a match on the current level.
    */
    void leapfrogNext();
    
    /*
      Move down to the next attribute.
    */
    void leapfrogDown();
    
    /*
      Move up to the previous attribute.
    */
    void leapfrogUp();
    
    /*
      Trie objects associated with the relations.
    */
    vector<Trie*> _tries;
    
    /*
      Trie iterators associated with the tries.
    */
    vector<TrieIterator*> _trieIterators;
    
    /*
      Those iterators that are located on the current level.
    */
    vector<TrieIterator*> _currentDepthIterators;
    
    bool _atEnd;
    int _p;
    int _depth;
    
    int _arity;
  };
}

#endif