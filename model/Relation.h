#include <map>
#include <string>
#include <vector>

using namespace std;

namespace model {
  class Tuple;
  
  class Relation {
  public:
    Relation(string file, string name, vector<string> columnNames);
    
    const Tuple& getRecord(int index) const;
    
    int getColumnIndex(string columnName) const;
    
    int size() const;
     
  private:
    void addRecord(Tuple* record);
    
    string _name;
    
    map<string, int> _columnNamesToIndeces;
    
    vector<Tuple*> _records;
  };
}