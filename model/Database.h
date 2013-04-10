#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>

using namespace std;

namespace model {
  class Relation;
  
  class Database {
  public:
    Database(string file);
    
    const Relation* getRelation(string name) const;
    
    Relation* getMutableRelation(string name);
    
  private:
    map<string, Relation*> _relationsByName;
  };
}

#endif