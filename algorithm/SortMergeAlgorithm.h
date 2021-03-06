#ifndef SORT_MERGE_ALGORITHM_H
#define SORT_MERGE_ALGORITHM_H

#include <string>
#include <vector>

#include "JoinAlgorithm.h"

namespace model {
  class Relation;
}

namespace util {
  class SimpleIterator;
}

using namespace std;

using model::Relation;
using util::SimpleIterator;

namespace algorithm {
  /*
    A version of JoinAlgorithm which uses the SortMerge algorithm for joining.
  */
  class SortMergeAlgorithm : public JoinAlgorithm {
  public:
    Relation* joinedRelation(vector<Relation*> relations,
                             vector<string> attributes);
                             
  };
}

#endif