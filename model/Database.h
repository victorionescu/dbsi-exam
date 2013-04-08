#include <map>
#include <string>

using namespace std;

namespace model {
  class Relation;
  
  class Database {
  public:
    Database(string file);
    
    const Relation& getRelation(string name) const;
    
  private:
    map<string, Relation*> _relationsByName;
  };
}