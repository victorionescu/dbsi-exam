#include <string>
#include <vector>

#include "../model/Relation.h"
#include "../model/Tuple.h"
#include "../util/SimpleIterator.h"
#include "SortMergeAlgorithm.h"

using namespace std;

using model::Relation;

namespace algorithm {
  
  bool anyIteratorDepleted(vector<SimpleIterator*> iterators) {
    for (unsigned int i = 0; i < iterators.size(); i++) {
      if (iterators[i]->atEnd()) return true;
    }
    
    return false;
  }
  
  Relation* SortMergeAlgorithm::joinedRelation(vector<const Relation*> relations,
                                               vector<string> attributes) {
      
    printf("Starting SortMerge\n");
    _iterators.clear();
    
    for (unsigned int i = 0; i < relations.size(); i++) {
      _iterators.push_back(new SimpleIterator(*relations[i]));
    }
    
    while (!anyIteratorDepleted(_iterators)) {
      
    }
    
    return NULL;
  }
} 