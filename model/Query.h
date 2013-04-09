#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <vector>

namespace algorithm {
  class JoinAlgorithm;
}

using namespace std;

using algorithm::JoinAlgorithm;

namespace model {
  class Database;
  class Relation;
  
  class Query{
  public:
    Query(string file, Database* database, JoinAlgorithm* joinAlgorithm);
    
    Relation* executeQuery();
  
  private:
    Database* _database;
    
    JoinAlgorithm* _joinAlgorithm;
    
    vector<const Relation*> _relations;
    
    vector<string> _attributes;
  };
}

#endif