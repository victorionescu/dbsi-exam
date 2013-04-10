#ifndef RELATION_H
#define RELATION_H

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace model {
  class Tuple;
  
  class Relation {
  public:
    Relation(string name, const vector<string>& columnNames);
    
    Relation(string file, string name, const vector<string>& columnNames);
    
    const Tuple& getRecord(int index) const;
    
    Tuple* getMutableRecord(int index);
    
    int getColumnIndex(string columnName) const;
    
    void sortBy(const vector<int>& indeces);
    
    int size() const;
    
    string name() const;
    
    void addRecord(Tuple* record);
    
    const map<string, int>& columnNamesToIndeces() const;
     
  private:
    
    string _name;
    
    map<string, int> _columnNamesToIndeces;
    
    vector<Tuple*> _records;
  };
}

#endif