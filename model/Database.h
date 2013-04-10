#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>

using namespace std;

namespace model {
  class Relation;
  
  /*
    Database class implemented as a collection of relations. It maps relation names to
    Relation objects.
  */
  class Database {
  public:
    // Initialize database from file.
    Database(string file);
    
    // Get relation by name.
    const Relation* getRelation(string name) const;
    
    // Get mutable relation by name.
    Relation* getMutableRelation(string name);
    
  private:
    // Map object holding the name -> Relation mappings.
    map<string, Relation*> _relationsByName;
  };
}

#endif