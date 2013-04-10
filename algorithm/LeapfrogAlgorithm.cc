#include <string>
#include <vector>

#include "../model/Relation.h"
#include "../util/Trie.h"
#include "../util/TrieIterator.h"
#include "LeapfrogAlgorithm.h"

using namespace std;

using model::Relation;
using util::Trie;
using util::TrieIterator;

namespace algorithm {
  
  LeapfrogAlgorithm::LeapfrogAlgorithm() : _depth(-1) {}
  
  void LeapfrogAlgorithm::leapfrogInit() {
    
    for (unsigned int i = 0; i < _currentDepthIterators.size(); i++)
      if (_trieIterators[i]->atEnd()) {
        _atEnd = true;
        return;
      }
      
    _atEnd = false;
    
    // Quadratic sort by key.
    for (unsigned int i = 0; i < (_currentDepthIterators.size() - 1); i++)
      for (unsigned int j = i + 1; j < _currentDepthIterators.size(); j++) {
        
        if (_currentDepthIterators[i]->key() > _currentDepthIterators[j]->key()) {
          TrieIterator* aux = _currentDepthIterators[i];
          _currentDepthIterators[i] = _currentDepthIterators[j];
          _currentDepthIterators[j] = aux;
        }
      
      }
      
    _p = 0;
    
  }
  
  void LeapfrogAlgorithm::leapfrogSearch() {
    int prev = _p - 1;
    if (prev < 0) prev = _currentDepthIterators.size() - 1;
    
    int maxKey = _currentDepthIterators[prev]->key();
    
    while (true) {
      int key = _currentDepthIterators[_p]->key();
      if (key == maxKey) {
        return;
      } else {
        _currentDepthIterators[_p]->seek(maxKey);
        
        if (_currentDepthIterators[_p]->atEnd()) {
          _atEnd = true;
          return;
        } else {
          _p++;
          
          if (_p >= _currentDepthIterators.size()) _p = 0;
        }
      }
    }
  }
  
  void LeapfrogAlgorithm::leapfrogNext() {
    _currentDepthIterators[_p]->next();
    if (_currentDepthIterators[_p]->atEnd()) {
      _atEnd = true;
    } else {
      _p++;
      
      if (_p >= _currentDepthIterators.size()) _p = 0;
      
      leapfrogSearch();
    }
  }
  
  void LeapfrogAlgorithm::leapfrogDown() {
    _depth++;
    
    if (_depth < _arity) {
    
      for (unsigned int i = 0; i < _trieIterators.size(); i++) {
        if (_trieIterators[i]->depth() < _depth)
          _trieIterators[i]->open();
      
        if (_trieIterators[i]->depth() > _depth)
          _trieIterators[i]->up();
      }
    
      _currentDepthIterators.clear();
    
      for (unsigned int i = 0; i < _trieIterators.size(); i++) {
        if (_trieIterators[i]->depth() == _depth) {
          _currentDepthIterators.push_back(_trieIterators[i]);
        }
      }
    }
    
  }
  
  void LeapfrogAlgorithm::leapfrogUp() {
    for (unsigned int i = 0; i < _trieIterators.size(); i++) {
      if (_trieIterators[i]->depth() == _depth) {
        _trieIterators[i]->up();
      }
    }
    
    _depth--;
    
    _currentDepthIterators.clear();
    
    for (unsigned int i = 0; i < _trieIterators.size(); i++) {
      if (_trieIterators[i]->depth() == _depth) {
        _currentDepthIterators.push_back(_trieIterators[i]);
      }
    }
  }
  
  
  
  Relation* LeapfrogAlgorithm::joinedRelation(vector<Relation*> relations,
                                              vector<string> attributes) {
                                                
    _tries.clear();
    _trieIterators.clear();
    _arity = attributes.size();
    
    for (unsigned int i = 0; i < relations.size(); i++) {
      
      // For trie construction.
      vector<int> indeces;
      vector<int> depths;
      
      for (unsigned int j = 0; j < attributes.size(); j++) {
        try {
          int columnIndex = relations[i]->getColumnIndex(attributes[j]);
          
          indeces.push_back(columnIndex);
          depths.push_back(j);
          
        } catch (int exceptionCode) {
          // Does not have column.
        }
      }
      
      Trie* relationTrie = new Trie(0, -1);
      
      for (int j = 0; j < relations[i]->size(); j++) {
        relationTrie->addToTrie(relations[i]->getMutableRecord(j), indeces, depths, 0);
      }
      
      _tries.push_back(relationTrie);
      _trieIterators.push_back(new TrieIterator(relationTrie));
      
    }
    
    
    /*
      Backtracking stage.
    */
    leapfrogDown();
    leapfrogInit();
    leapfrogSearch();
    
    int numberOfResults = 0;
    
    while (_depth > -1) {
      if (_depth >= _arity) {
        int product = 1;
        
        for (unsigned int i = 0; i < _trieIterators.size(); i++) 
          product *= _trieIterators[i]->numberOfTuples();
          
        numberOfResults += product;
        
        leapfrogUp();
        leapfrogInit();
      } else {
        
        if (_atEnd) {
          leapfrogUp();
          if (_depth > -1) {
            leapfrogInit();
            leapfrogNext();
          }
          
        } else {
          leapfrogDown();
          if (_depth < attributes.size()) {
            leapfrogInit();
            leapfrogSearch();
          }
        
        }
      } 
    }
    
    printf("%d\n", numberOfResults);
    
    
    return NULL;
  }
}