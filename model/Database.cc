#include <fstream>
#include <map>
#include <string>

#include "Database.h"
#include "Relation.h"

using namespace std;

namespace model {
  Database::Database(string file) {
    ifstream inputStream(file.c_str());
    
    while (!inputStream.eof()) {
      
      string line;
      
      getline(inputStream, line);
      
      if (line.length() < 1) continue;
      
      unsigned int positionInLine = 0;
      
      string relationFile = "";
      
      while (isalpha(line[positionInLine]) || line[positionInLine] == '.')
        relationFile.push_back(line[positionInLine++]);
        
      while (!isalpha(line[positionInLine]))
        positionInLine++;
        
      string relationName = "";
      
      while (isalpha(line[positionInLine]))
        relationName.push_back(line[positionInLine++]);
        
      vector<string> columnNames;
      
      while (true) {
        while (positionInLine < line.length() && !isalpha(line[positionInLine]))
          positionInLine++;
          
        if (positionInLine >= line.length()) break;
        
        string columnName = "";
        
        while (positionInLine < line.length() && isalpha(line[positionInLine]))
          columnName.push_back(line[positionInLine++]);
          
        columnNames.push_back(columnName);
      }
      
      printf("LOG: adding relation %s from file %s\n", relationName.c_str(), relationFile.c_str());
      _relationsByName[relationName] = new Relation(relationFile, relationName, columnNames);
    }
  }
  
  const Relation* Database::getRelation(string name) const{
    map<string, Relation*>::const_iterator it = _relationsByName.find(name);
    
    if (it != _relationsByName.end()) {
      return it->second;
    } else {
      throw 2;
    }
  }
}