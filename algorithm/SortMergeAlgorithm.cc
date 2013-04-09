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
    while (fieldsCoincide(iterator, constFields) && iterator->record().getValue(field) < maxValue) {
      
      iterator->next();
      
    }
      
    return fieldsCoincide(iterator, constFields);
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
  
  
  
  Relation* SortMergeAlgorithm::joinedRelation(vector<const Relation*> relations,
                                               vector<string> attributes) {
      
    printf("Starting SortMerge\n");
    _iterators.clear();
    
    for (unsigned int i = 0; i < relations.size(); i++) {
      _iterators.push_back(new SimpleIterator(*relations[i]));
      printf("LOG: Creates iterator for relation %s.\n", relations[i]->name().c_str());
    }
    
    vector<vector<int> > fieldsToLevelUp;
    vector<vector<int> > relationsToLevelUp;
    
    for (unsigned int i = 0; i < attributes.size(); i++) {
      
      vector<int> fieldsForAttribute;
      vector<int> relationsForAttribute;
      
      for (unsigned int j = 0; j < relations.size(); j++) {
        
        try {
          int index = relations[j]->getColumnIndex(attributes[i]);
          
          printf("LOG: Relation %s has attribute %s at index %d.\n", relations[j]->name().c_str(), attributes[i].c_str(), index);
          
          fieldsForAttribute.push_back(index);
          relationsForAttribute.push_back(j);
        
        } catch(int exceptionNumber) {
          
          printf("LOG: Relation %s doesn't have attribute %s.\n", relations[j]->name().c_str(), attributes[i].c_str());
        
        }
      }
      
      fieldsToLevelUp.push_back(fieldsForAttribute);
      relationsToLevelUp.push_back(relationsForAttribute);
    }
    
    while (!anyIteratorDepleted(_iterators)) {
      vector<vector<pair<int, int> > > constFields;
      
      for (unsigned int i = 0; i < relations.size(); i++) {
        vector<pair<int, int> > constFieldsForRelation;
        
        constFields.push_back(constFieldsForRelation);
      }
      
      
      bool failedLevelUp = false;
      
      for (unsigned int i = 0; i < attributes.size() && !failedLevelUp; i++){
        vector<SimpleIterator*> iteratorsToLevelUp;
        vector<vector<pair<int, int> > > constFieldsToLevelUp;
        
        for (unsigned int j = 0; j < relationsToLevelUp[i].size(); j++) {
          int relationIndex = relationsToLevelUp[i][j];
          
          iteratorsToLevelUp.push_back(_iterators[relationIndex]);
          constFieldsToLevelUp.push_back(constFields[relationIndex]);
        }
        
        failedLevelUp = failedLevelUp || !levelUpIterators(iteratorsToLevelUp, fieldsToLevelUp[i], constFieldsToLevelUp);
        
        if (!failedLevelUp) {
          
          for (unsigned int j = 0; j < relationsToLevelUp[i].size(); j++) {
            int relationIndex = relationsToLevelUp[i][j];
            const Tuple& currentRecord = _iterators[relationIndex]->record();
          
            constFields[relationIndex].push_back(
                make_pair(fieldsToLevelUp[i][j], currentRecord.getValue(fieldsToLevelUp[i][j]))
            );
          }
        
        }
      }
      
      if (!failedLevelUp) {
        printf("FOUND A MATCH\n");
        break;
      }
        
    }
    
    return NULL;
  }
} 