#ifndef Quest_HH_
#define Quest_HH_

#include <string>
#include <vector>

class Quest{
private:
  std::vector<std::string> values;
  std::vector<std::string> valueCategories;
  std::vector<std::string> bools;
  int storyPoint = 0;

public:
  void addValue(std::string value, std::string valueCategory){
    values.push_back(value);
    valueCategories.push_back(valueCategory);
  }

  void addBool(std::string boolValue){
    bools.push_back(boolValue);
  }

  void printQuest(){
    for(int i = 0; i < values.size(); i++){
      std::cout << valueCategories[i] << ": " << values[i] << std::endl;
    }
    for(int i = 0; i < bools.size(); i++){
      std::cout << bools[i] << std::endl;
    }
  }

  void printQuest(std::string category){
    std::cout << valueCategories[0] << ": " << values[0] << std::endl;
    bool found = false;
    for(int i = 1; i < values.size(); i++){
      if(valueCategories[i] == category){
        std::cout << category << ": " << values[i] << std::endl;
        found = true;
      }
    }
    if(!found){
      std::cout << category << ": None" << std::endl;
    }
  }

  bool hasCategory(std::string category){
    for(int i = 0; i < valueCategories.size(); i++){
      if(valueCategories[i] == category){
        return true;
      }
    }
    return false;
  }

  void setStoryPoint(int point){
    storyPoint = point;
  }

  int getStoryPoint(){
    return storyPoint;
  }

};

#endif
