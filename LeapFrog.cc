#include <cstdio>
#include <vector>

#include "util/LinearIterator.h"

using namespace std;

using util::LinearIterator;

int main() {
  
  printf("Starting Leapfrog\n");
  
  vector<int> coll;
  coll.push_back(15);
  coll.push_back(15);
  coll.push_back(100);
  coll.push_back(6);
  coll.push_back(5);
  coll.push_back(31);
  coll.push_back(5);
  coll.push_back(1);
  coll.push_back(23);
  coll.push_back(5);
  coll.push_back(3);
  coll.push_back(5);
  coll.push_back(10);
  coll.push_back(2);
  coll.push_back(35);
  coll.push_back(1);
  coll.push_back(41);
  coll.push_back(5);
  
  LinearIterator<int> *it = new LinearIterator<int>(coll);
  
  it->seek(30);
  
  while (!it->atEnd()) {
    printf("%d\n", it->key());
    it->next();
  }
  
  return 0;
}