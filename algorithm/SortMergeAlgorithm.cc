#include <string>
#include <vector>

#include "../model/Relation.h"
#include "../model/Tuple.h"
#include "../util/SimpleIterator.h"
#include "SortMergeAlgorithm.h"

using namespace std;

using model::Relation;

namespace algorithm {

  /*
    Compares two records by matching columns pointed at by indecesA with columns
    pointed at by indecesB.
    
    Returns:
    
    1 - recordA < recordB
    2 - recordA > recordB
    3 - recordA = recordB
  */
  int compareRecords(const Tuple& recordA,
                     const Tuple& recordB,
                     const vector<int>& indecesA,
                     const vector<int>& indecesB) {
                  
    for (unsigned int i = 0; i < indecesA.size(); i++) {
      if (recordA.getValue(indecesA[i]) < recordB.getValue(indecesB[i]))
        return 1;
      else if (recordA.getValue(indecesA[i]) > recordB.getValue(indecesB[i]))
        return 2;
    }
    
    return 3;
  }
  
  /*
    Joins relations firstRelation and secondRelation by looking at attributes.
    
    If output is true, the result will contain the joined tuples. Otherwise, only
    the number of records will be printed out. 
    
    output is false only for the final two-join.
  */
  
  Relation* joinedTwoRelation(Relation* firstRelation, Relation* secondRelation, const vector<string>& attributes, bool output) {
    /*
      Looks at the column names and finds out attributes that are common to the relations or are
      present in only one of them.
    */
    vector<int> firstCommonAttributes;
    vector<int> secondCommonAttributes;
    vector<string> commonNames;
    
    vector<int> firstOtherAttributes;
    vector<string> firstOtherNames;
    
    vector<int> secondOtherAttributes;
    vector<string> secondOtherNames;
    
    const map<string, int>& firstColumnNamesToIndeces = firstRelation->columnNamesToIndeces();
    const map<string, int>& secondColumnNamesToIndeces = secondRelation->columnNamesToIndeces();
    
    for (unsigned int i = 0; i < attributes.size(); i++) {
      try {
        int firstIndex = firstRelation->getColumnIndex(attributes[i]);
        
        int secondIndex = secondRelation->getColumnIndex(attributes[i]);
        
        firstCommonAttributes.push_back(firstIndex);
        
        secondCommonAttributes.push_back(secondIndex);
        
        commonNames.push_back(attributes[i]);
        
      } catch (int exceptionCode) {
        // Attribute is not common.
      }
    }
    
    for (map<string, int>::const_iterator it = firstColumnNamesToIndeces.begin();
        it != firstColumnNamesToIndeces.end(); ++it) {
          
      bool found = false;
      
      for (unsigned int i = 0; i < firstCommonAttributes.size(); i++)
        if (firstCommonAttributes[i] == it->second) {
          found = true;
          break;
        }
        
      if (!found) {
        firstOtherAttributes.push_back(it->second);
        firstOtherNames.push_back(it->first);
      }
    }
    
    for (map<string, int>::const_iterator it = secondColumnNamesToIndeces.begin();
        it != secondColumnNamesToIndeces.end(); ++it) {
          
      bool found = false;
      
      for (unsigned int i = 0; i < secondCommonAttributes.size(); i++)
        if (secondCommonAttributes[i] == it->second) {
          found = true;
          break;
        }
        
      if (!found) {
        secondOtherAttributes.push_back(it->second);
        secondOtherNames.push_back(it->first);
      }
    }
    
    
    /*
      We create two SimpleIterator's, one for each relation and we sort the relations by their,
      common attributes, in the order specified by the query file.
    */
    SimpleIterator* firstIterator = new SimpleIterator(firstRelation, firstCommonAttributes);
    
    SimpleIterator* secondIterator = new SimpleIterator(secondRelation, secondCommonAttributes);
    
    /*
      We compute the order and names of the columns of the joined relation.
    */
    
    vector<string> columnNames;
    
    
    
    for (unsigned int i = 0; i < commonNames.size(); i++)
      columnNames.push_back(commonNames[i]);
      
    for (unsigned int i = 0; i < firstOtherNames.size(); i++)
      columnNames.push_back(firstOtherNames[i]);
      
    for (unsigned int i = 0; i < secondOtherNames.size(); i++)
      columnNames.push_back(secondOtherNames[i]);
      
    Relation* result = new Relation("JOINED", columnNames);
    
    long long counter = 0;
    
    /*
      Finally, we do the SortMerge step.
    */
    while (!firstIterator->atEnd() && !secondIterator->atEnd()) {
      while (!firstIterator->atEnd() && compareRecords(firstIterator->record(), secondIterator->record(),
                                                       firstCommonAttributes, secondCommonAttributes) == 1) {
        firstIterator->next();
      }
      
      if (firstIterator->atEnd()) continue;
      
      while (!secondIterator->atEnd() && compareRecords(firstIterator->record(), secondIterator->record(),
                                                        firstCommonAttributes, secondCommonAttributes) == 2) {
        secondIterator->next();
      }
      
      if (secondIterator->atEnd()) continue;
      
      /*
        One we have found a match, we process all the equal elements and generate all possible pairs.
      */
      if (compareRecords(firstIterator->record(), secondIterator->record(),
                         firstCommonAttributes, secondCommonAttributes) == 3) {
        
        vector<vector<pair<int, int> > > firstTuplesToMatch;                                      
        
        const Tuple& recordA = firstIterator->record();
        
        
        while (!firstIterator->atEnd() && compareRecords(recordA, firstIterator->record(),
                                                         firstCommonAttributes, firstCommonAttributes) == 3) {
                                                           
          vector<pair<int, int> > tuplesToMatch;
          
          const Tuple& record = firstIterator->record();
          
          for (unsigned int i = 0; i < firstOtherAttributes.size(); i++)
            tuplesToMatch.push_back(make_pair(commonNames.size() + i, record.getValue(firstOtherAttributes[i])));
            
          firstTuplesToMatch.push_back(tuplesToMatch);
          
          firstIterator->next();
        }
        
        vector<vector<pair<int, int> > > secondTuplesToMatch;
        
        const Tuple& recordB = secondIterator->record();
        
        
        while (!secondIterator->atEnd() && compareRecords(recordB, secondIterator->record(),
                                                          secondCommonAttributes, secondCommonAttributes) == 3) {
          
          vector<pair<int, int> > tuplesToMatch;
          
          const Tuple& record = secondIterator->record();
          
          for (unsigned int i = 0; i < secondOtherAttributes.size(); i++)
            tuplesToMatch.push_back(make_pair(commonNames.size() + firstOtherNames.size() + i,
                                              record.getValue(secondOtherAttributes[i])));
          
          secondTuplesToMatch.push_back(tuplesToMatch);
          
          secondIterator->next();
        }
        
        // If output, we store those records in memory.
        if (output) {
        
          for (unsigned int i = 0; i < firstTuplesToMatch.size(); i++)
            for (unsigned int j = 0; j < secondTuplesToMatch.size(); j++) {
              Tuple* newTuple = new Tuple(columnNames.size());
            
              for (unsigned int k = 0; k < commonNames.size(); k++) {
                newTuple->setValue(k, recordA.getValue(firstCommonAttributes[k]));
              }
            
              for (unsigned int k = 0; k < firstTuplesToMatch[i].size(); k++) {
                newTuple->setValue(firstTuplesToMatch[i][k].first, firstTuplesToMatch[i][k].second);
              }
            
              for (unsigned int k = 0; k < secondTuplesToMatch[j].size(); k++) {
                newTuple->setValue(secondTuplesToMatch[j][k].first, secondTuplesToMatch[j][k].second);
              }
            
              result->addRecord(newTuple);
            }
        } else { // Otherwise, we just count them.
          counter += (long long)firstTuplesToMatch.size() * secondTuplesToMatch.size();
        }
        
      }
    }
    
    if (!output) {
      printf("%lld RECORDS.\n", counter);
    }
    
    return result;
  }
  
  /*
    We are trying to maximize the number of common attributes, so we are looking to level up the number
    of columns of the relations that we join. That is why, we try to join them using a tree structure.
    
    output has the same meaning as above.
  */ 
  Relation* binaryMerge(const vector<Relation*>& relations, int start, int end, const vector<string>& attributes, bool output) {
    if (start == end)
      return relations[start];
      
    int m = (start + end) / 2;
    
    Relation* firstRelation = binaryMerge(relations, start, m, attributes, true);
    Relation* secondRelation = binaryMerge(relations, m + 1, end, attributes, true);
    
    return joinedTwoRelation(firstRelation, secondRelation, attributes, output);
  }
  
  Relation* SortMergeAlgorithm::joinedRelation(vector<Relation*> relations,
                                               vector<string> attributes) {
    
    /*
      Here we just do a merge on the whole binary tree, setting output to 0. (since storing all the records runs out of memory)
    */                                             
    return binaryMerge(relations, 0, relations.size() - 1, attributes, false);
  }
} 