#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "Relation.h"
#include "Tuple.h"

using namespace std;

namespace model {
  
  // COMPARE CLASS IMPLEMENTATION
  
  class CompareClass {
  public:
    CompareClass(const vector<int>& indeces);
    
    bool operator()(const Tuple* recordA, const Tuple* recordB);
  private:
    const vector<int>& _indeces;
  };
  
  
  CompareClass::CompareClass(const vector<int>& indeces) : _indeces(indeces) {}
  
  
  inline bool CompareClass::operator()(const Tuple* recordA, const Tuple* recordB) {
    
    for (unsigned int i = 0; i < _indeces.size(); i++) {
      if (recordA->getValue(_indeces[i]) < recordB->getValue(_indeces[i]))
        return true;
      else if (recordA->getValue(_indeces[i]) > recordB->getValue(_indeces[i])) 
        return false;
    }
      
    return false;
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////////
  Relation::Relation(string name, const vector<string>& columnNames) : _name(name) {
    int counter = 0;
    
    for (vector<string>::const_iterator it = columnNames.begin(); it != columnNames.end(); ++it) {
      _columnNamesToIndeces[*it] = counter++;
    }
  }
  
  Relation::Relation(string file, string name, const vector<string>& columnNames) : _name(name) {
  
    int counter = 0;
    
    for (vector<string>::const_iterator it = columnNames.begin(); it != columnNames.end(); ++it) {
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
      
      throw 1;
    
    }
  }
  
  void Relation::sortBy(const vector<int>& indeces) {
    CompareClass* compareObject = new CompareClass(indeces);
    
    sort(_records.begin(), _records.end(), *compareObject);
  }
  
  int Relation::size() const {
    return _records.size();
  }
  
  string Relation::name() const {
    return _name;
  }
  
  const map<string,int>& Relation::columnNamesToIndeces() const {
    return _columnNamesToIndeces;
  }
  
  void Relation::addRecord(Tuple* record) {
    _records.push_back(record);
  }
  
  /*
    PRIVATE MEMBERS
  */
  
  
  
  
}