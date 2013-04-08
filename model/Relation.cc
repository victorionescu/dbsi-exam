#include <fstream>
#include <string>
#include <vector>

#include "Relation.h"
#include "Tuple.h"

using namespace std;

namespace model {
  Relation::Relation(string file, string name, vector<string> columnNames) : _name(name) {
  
    int counter = 0;
    
    for (vector<string>::iterator it = columnNames.begin(); it != columnNames.end(); ++it) {
      _columnNamesToIndeces[*it] = counter++;
    }
    
    ifstream inputStream(file.c_str());
    
    while (!inputStream.eof()) {
      string line;
      
      getline(inputStream, line);
      
      if (line.length() < 1) continue;
      
      unsigned int positionInLine = 0;
      Tuple* record = new Tuple(columnNames.size());
      
      for (unsigned int i = 0; i < columnNames.size(); i += 1) {
        while (!isdigit(line[positionInLine]))
          ++positionInLine;
          
        int value = 0;
        
        while (positionInLine < line.size() && isdigit(line[positionInLine]))
          value = value * 10 + line[positionInLine++] - '0';
          
        record->setValue(i, value);
      }
      
      addRecord(record);
    }
  }
  
  
  const Tuple& Relation::getRecord(int index) const {
    return *_records[index];
  }
  
  int Relation::getColumnIndex(string columnName) const {
    map<string, int>::const_iterator it = _columnNamesToIndeces.find(columnName);
    
    if (it != _columnNamesToIndeces.end()) {
      return it->second;
    } else {
      throw;
    }
  }
  
  
  int Relation::size() const {
    return _records.size();
  }
  
  /*
    PRIVATE MEMBERS
  */
  
  void Relation::addRecord(Tuple* record) {
    _records.push_back(record);
  }
  
}