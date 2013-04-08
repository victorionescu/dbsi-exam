#include <iostream>
#include <string>
#include <vector>

#include "model/Database.h";

using model::Database;

int main(int argc, char* argv[]) {
  
  vector<string> args;
  
  for (int i = 1; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  
  if (args.size() != 4 || args[0] != "-query" || args[2] != "-database") {
    printf("usage: sortmerge -query [queryfile] -database [databasefile]\n");
  }
  
  Database* db = new Database(args[3]);
  
  return 0;
}