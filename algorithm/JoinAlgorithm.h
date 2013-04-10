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
  
  /*
    Pure interface for the join algorithms.
  */
  class JoinAlgorithm {
  public:
    // A relation that represents the join by attributes of all relations. 
    virtual Relation* joinedRelation(vector<Relation*> relations,
                                     vector<string> attributes) = 0;
  };
}

#endif