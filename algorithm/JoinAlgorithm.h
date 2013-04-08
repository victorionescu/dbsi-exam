#ifndef JOIN_ALGORITHM_H
#define JOIN_ALGORITHM_H

#include <string>
#include <vector>

namespace model {
  class Relation;
}

using namespace std;

using model::Relation;

namespace algorithm {
  class JoinAlgorithm {
  public:
    virtual Relation* joinedRelation(vector<const Relation*> relations,
                                     vector<string> attributes) = 0;
  };
}

#endif