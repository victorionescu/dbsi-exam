#include <string>
#include <vector>

#include "../model/Relation.h"
#include "../model/Tuple.h"
#include "../util/SimpleIterator.h"
#include "SortMergeAlgorithm.h"

using namespace std;

using model::Relation;

namespace algorithm {
  
  bool anyIteratorDepleted(vector<SimpleIterator*> iterators) {
    for (unsigned int i = 0; i < iterators.size(); i++) {
      if (iterators[i]->atEnd()) return true;
    }
    
    return false;
  }
  
  
  
  bool allIteratorsEqual(vector<SimpleIterator*> iterators, vector<int> fields) {
    int valueToCheck = iterators[0]->record().getValue(fields[0]);
    
    for (unsigned int i = 1; i < iterators.size(); i++)
      if (iterators[i]->record().getValue(fields[i]) != valueToCheck) return false;
      
    return true;
  }
  
  
  
  bool fieldsCoincide(SimpleIterator *iterator, vector<pair<int, int> > constFields) {
    const Tuple& record = iterator->record();
    
    for (unsigned int i = 0; i < constFields.size(); i += 1) {
      if (record.getValue(constFields[i].first) != constFields[i].second)
        return false;
    }
    
    return true;
  } 
  
  
  
  
  bool pumpIteratorUp(SimpleIterator *iterator, int field, int maxValue, vector<pair<int, int> > constFields) {
    while (!iterator->atEnd() && fieldsCoincide(iterator, constFields) && iterator->record().getValue(field) < maxValue) {
      
      iterator->next();
      
    }
      
    return !iterator->atEnd() && fieldsCoincide(iterator, constFields);
  }
  
  
  
  
  bool levelUpIterators(vector<SimpleIterator*> iterators,
                        vector<int> fields,
                        vector<vector<pair<int, int> > > constFields) {
    
    bool brokenConstFields = false;
    
    
    while (!brokenConstFields && !anyIteratorDepleted(iterators) && !allIteratorsEqual(iterators, fields)) {
      int maxValue = -1;
      
      for (unsigned int i = 0; i < iterators.size(); i++) {
        if (iterators[i]->record().getValue(fields[i]) > maxValue) {
          maxValue = iterators[i]->record().getValue(fields[i]);
        }
      }
      
    
      for (unsigned int i = 0; i < iterators.size(); i++) {
        brokenConstFields = brokenConstFields || !pumpIteratorUp(iterators[i], fields[i], maxValue, constFields[i]);
        
        if (brokenConstFields) break;
      }
      
    }
    
    return (!brokenConstFields && !anyIteratorDepleted(iterators));
  }
  
  
  /*int backtracking(const vector<vector<const Tuple*> >& tuples) {
    unsigned int stackSize = tuples.size();
    
    vector<unsigned int> stack;
    
    int head = 0;
    
    stack.push_back(0);
    
    while (head >= 0) {
      if (head == stackSize) {
        
        
        continue;
      }
      
      if (stack[head] < tuples[head].size()) {
        stack.push_back(0);
        head++;
      } else {
        stack.pop_back();
        head--;
        stack[head]++;
      }
    }
  }*/
  
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
  
  Relation* joinedTwoRelation(Relation* firstRelation, Relation* secondRelation, const vector<string>& attributes, bool output) {
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
    
    printf("Common attributes: ");
    
    for (unsigned int i = 0; i < commonNames.size(); i++) {
      printf("(%d %d) -> %s ", firstCommonAttributes[i], secondCommonAttributes[i], commonNames[i].c_str());
    }
    
    printf("\n");
    
    printf("First other attributes: ");
    
    for (unsigned int i = 0; i < firstOtherNames.size(); i++) {
      printf("%d -> %s ", firstOtherAttributes[i], firstOtherNames[i].c_str());
    }
    
    printf("\n");
    
    printf("Second other attributes: ");
    
    for (unsigned int i = 0; i < secondOtherNames.size(); i++) {
      printf("%d -> %s ", secondOtherAttributes[i], secondOtherNames[i].c_str());
    }
    
    printf("\n");
    
    
    SimpleIterator* firstIterator = new SimpleIterator(firstRelation, firstCommonAttributes);
    
    SimpleIterator* secondIterator = new SimpleIterator(secondRelation, secondCommonAttributes);
    
    
    /*
    while (!firstIterator->atEnd()) {
      const Tuple& record = firstIterator->record();
      
      for (unsigned int i = 0; i < record.size(); i++) {
        printf("%d ", record.getValue(i));
      }
      
      printf("\n");
      
      firstIterator->next();
    }*/
  
    
    vector<string> columnNames;
    
    
    
    for (unsigned int i = 0; i < commonNames.size(); i++)
      columnNames.push_back(commonNames[i]);
      
    for (unsigned int i = 0; i < firstOtherNames.size(); i++)
      columnNames.push_back(firstOtherNames[i]);
      
    for (unsigned int i = 0; i < secondOtherNames.size(); i++)
      columnNames.push_back(secondOtherNames[i]);
      
    Relation* result = new Relation("JOINED", columnNames);
    
    long long counter = 0;
    
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
        } else {
          counter += (long long)firstTuplesToMatch.size() * secondTuplesToMatch.size();
        }
        
      }
    }
    
    if (!output) {
      printf("%lld RECORDS.\n", counter);
    }
    
    return result;
  }
  
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
                                                 
    return binaryMerge(relations, 0, relations.size() - 1, attributes, false);
  }
} 