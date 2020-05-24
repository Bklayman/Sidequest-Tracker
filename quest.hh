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
  //Adds a value to values and the same value's category to valueCategories
  void addValue(std::string value, std::string valueCategory){
    values.push_back(value);
    valueCategories.push_back(valueCategory);
  }

  //Adds a value to this quest
  void addValue(std::string value){
    values.push_back(value);
  }

  //Adds a value category to this quest
  void addValueCategory(std::string valueCategory){
    valueCategories.push_back(valueCategory);
  }

  //Adds a bool value to bools
  void addBool(std::string boolValue){
    bools.push_back(boolValue);
  }

  void removeBool(int index){
    bools.erase(bools.begin() + index);
  }

  //Prints all details about a quest
  void printQuest(){
    for(int i = 0; i < values.size(); i++){
      std::cout << valueCategories[i] << ": " << values[i] << std::endl;
    }
    for(int i = 0; i < bools.size(); i++){
      std::cout << bools[i] << std::endl;
    }
  }

  //Prints a quest's information relating to a specified category.
  //Prints none if the category is not found (is case sensitive).
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

  //Returns true if a quest has a value in a specific category.
  //Returns false otherwise.
  bool hasCategory(std::string category){
    for(int i = 0; i < valueCategories.size(); i++){
      if(valueCategories[i] == category){
        return true;
      }
    }
    return false;
  }

  //Setter function for story point.
  void setStoryPoint(int point){
    storyPoint = point;
  }

  //Getter function for story point.
  int getStoryPoint(){
    return storyPoint;
  }

  //Getter function for values
  std::vector<std::string> getValues(){
    return values;
  }

  //Getter function for value categories
  std::vector<std::string> getValueCategories(){
    return valueCategories;
  }

  //Getter function for bools
  std::vector<std::string> getBools(){
    return bools;
  }

  //Changes the value at index to given value
  void changeValue(int index, std::string value){
    values[index] = value;
  }

  //Changes the category at index to given value
  void changeCategory(int index, std::string category){
    valueCategories[index] = category;
  }

  //Changes the bool at index to given value
  void changeBool(int index, std::string name){
    bools[index] = name;
  }

};

#endif
