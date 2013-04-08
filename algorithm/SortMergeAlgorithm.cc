#include <string>
#include <vector>

#include "../model/Relation.h"
#include "../model/Tuple.h"
#include "../util/SimpleIterator.h"
#include "SortMergeAlgorithm.h"

using namespace std;

using model::Relation;

namespace algorithm {
  
  Relation* SortMergeAlgorithm::joinedRelation(vector<const Relation*> relations,
                                               vector<string> attributes) {
      
    printf("Starting SortMerge\n");
    return NULL;
    
  }
} 