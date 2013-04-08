#include <string>
#include <vector>

using namespace std;

namespace model {
  
  class Tuple {
  public:
    Tuple(int size);
    
    void setValue(int index, int value);
    
    int getValue(int index) const;
    
  private:
    int _size;
    vector<int> _tuple;
  };
  
}