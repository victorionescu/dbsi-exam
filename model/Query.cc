#include <fstream>
#include <string>
#include <vector>

#include "../algorithm/JoinAlgorithm.h"
#include "Database.h"
#include "Query.h"
#include "Relation.h"

using namespace std;

using algorithm::JoinAlgorithm;

namespace model {
  Query::Query(string file, Database* database, JoinAlgorithm* joinAlgorithm) :
      _database(database),
      _joinAlgorithm(joinAlgorithm) {
        
    ifstream inputStream(file.c_str());
    
    string line;
    unsigned int positionInLine;
    
    getline(inputStream, line);
    positionInLine = 0;
    
    while(true) {
      while (positionInLine < line.size() && !isalpha(line[positionInLine]))
        positionInLine++;
        
      if (positionInLine >= line.size()) break;
      
      string relationName = "";
      
      while (positionInLine < line.size() && isalpha(line[positionInLine]))
        relationName.push_back(line[positionInLine++]);
      
      try {  
        
        _relations.push_back(database->getMutableRelation(relationName));
        
        printf("LOG: Joining relation %s.\n", relationName.c_str());
      
      } catch(int exceptionNumber) {
        
        printf("ERROR: Relation %s is not in the database.\n", relationName.c_str());
        
      }
    }
    
    getline(inputStream, line);
    positionInLine = 0;
    
    while (true) {
      while (positionInLine < line.size() && !isalpha(line[positionInLine]))
        positionInLine++;
        
      if (positionInLine >= line.size()) break;
      
      string attributeName = "";
      
      while (positionInLine < line.size() && isalpha(line[positionInLine]))
        attributeName.push_back(line[positionInLine++]);
        
      _attributes.push_back(attributeName);
      
      printf("LOG: Joining on attribute %s.\n", attributeName.c_str());
    }
  }
  
  Relation* Query::executeQuery() {
    return _joinAlgorithm->joinedRelation(_relations, _attributes);
  }
}