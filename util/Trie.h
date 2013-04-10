#ifndef TRIE_H
#define TRIE_H

#include <vector>

namespace model {
  class Tuple;
}

using namespace std;

using model::Tuple;

namespace util {
  /*
    Class implementing a trie.
  */
  class Trie {
  public:
    Trie(int key, int depth);
    
    /*
      Add tuple to current trie. The next value to add is at depth depths[currentIndex] and is contained in the
      tuple at indeces[currentIndex].
    */
    void addToTrie(Tuple* tuple, const vector<int>& indeces, const vector<int>& depths, unsigned int currentIndex);
    
    /*
      Get key for the current trie node.
    */
    int getKey();
    
    /*
      Get depth of the current trie node. (with respect to the join attribute order)
    */
    int getDepth();
    
    /*
      Number of sons of this node.
    */
    unsigned int numberOfSons();
    
    /*
      Get son at index.
    */
    Trie* getSon(int index);
    
    /*
      Number of tuples that end at this node. Non-zero only for leaves.
    */
    int numberOfTuples();
    
  private:
    int _key;
    int _depth;
    
    vector<Trie*> _sons;
    
    vector<Tuple*> _tuples;
  };
  
}

#endif