#include <iostream>
#include <string>
#include <vector>

#include "algorithm/JoinAlgorithm.h"
#include "algorithm/SortMergeAlgorithm.h"
#include "model/Database.h"
#include "model/Query.h"
#include "model/Relation.h"

using algorithm::JoinAlgorithm;
using algorithm::SortMergeAlgorithm;
using model::Database;
using model::Query;

int main(int argc, char* argv[]) {
  
  vector<string> args;
  
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  
  if (args.size() != 4 || args[0] != "-query" || args[2] != "-database") {
    printf("usage: sortmerge -query [queryfile] -database [databasefile]\n");
    return 0;
  }
  
  Database* db = new Database(args[3]);
  
  JoinAlgorithm* joinAlgorithm = new SortMergeAlgorithm();
  
  Query* query = new Query(args[1], db, joinAlgorithm);
  
  Relation* result = query->executeQuery();
  
  //printf("%d records.\n", result->size());
  
  return 0;
}