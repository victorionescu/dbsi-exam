#ifndef SORT_MERGE_ALGORITHM_H
#define SORT_MERGE_ALGORITHM_H

#include <string>
#include <vector>

#include "JoinAlgorithm.h"

namespace model {
  class Relation;
}

using namespace std;

using model::Relation;

namespace algorithm {
  class SortMergeAlgorithm : public JoinAlgorithm {
  public:
    Relation* joinedRelation(vector<const Relation*> relations,
                             vector<string> attributes);
  };
}

#endif