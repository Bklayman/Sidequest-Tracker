#ifndef QuestList_HH_
#define QuestList_HH_

#include <vector>

#include "quest.hh"

class QuestList{
private:
  std::vector<Quest*> quests;
  std::vector<Quest*> questGarbage;
  std::vector<std::string> storyPoints;
  std::vector<std::string> storyPointGarbage;

public:

  //Getter function for quests.
  std::vector<Quest*> getQuests(){
    return quests;
  }

  //Getter function for questGarbage.
  std::vector<Quest*> getQuestGarbage(){
    return questGarbage;
  }

  //Getter function for storyPoints.
  std::vector<std::string> getStoryPoints(){
    return storyPoints;
  }

  //Getter function for storyPointGarbage.
  std::vector<std::string> getStoryPointGarbage(){
    return storyPointGarbage;
  }

  //Adds a quest to quests.
  void addQuest(Quest* addedQuest){
    quests.push_back(addedQuest);
  }

  //Adds a calue to the garbage quests
  void addQuestGarbage(Quest* garbage){
    questGarbage.push_back(garbage);
  }

  //Moves a quest from quests to questGarbage.
  void removeQuest(int index){
    Quest* removedQuest = quests[index];
    quests.erase(quests.begin() + index);
    questGarbage.push_back(removedQuest);
  }

  //Adds a story point to storyPoints.
  void addStoryPoint(std::string addedPoint){
    storyPoints.push_back(addedPoint);
  }

  //Adds a value to the garbage story points
  void addGarbageStoryPoint(std::string addedGarbagePoint){
    storyPointGarbage.push_back(addedGarbagePoint);
  }

  //Moves a story point from storyPoints to storyPointGarbage.
  void removeStoryPoint(int index){
    for(int i = index; i >= 0; i--){
      std::string removedStoryPoint = storyPoints[i];
      storyPoints.erase(storyPoints.begin() + i);
      storyPointGarbage.push_back(removedStoryPoint);
    }
  }

  //Moves a quest from questGarbage to quests.
  void reuseQuest(int indexToUse, int indexToAdd){
    Quest* reusedQuest = questGarbage[indexToUse];
    questGarbage.erase(questGarbage.begin() + indexToUse);
    quests.insert(quests.begin() + indexToAdd, reusedQuest);
  }

  //Moves a story point from storyPointGarbage to storyPoints.
  //If this story point is not the most recently deleted story point, all story points after will also be moved.
  void reuseStoryPoint(int indexToUse){
    for(int i = storyPointGarbage.size() - 1; i >= indexToUse; i--){
      std::string reusedStoryPoint = storyPointGarbage[i];
      storyPointGarbage.erase(storyPointGarbage.begin() + i);
      storyPoints.insert(storyPoints.begin(), reusedStoryPoint);
    }
  }

  //Prints all information for every quest in quests.
  void printAll(){
    for(int i = 0; i < quests.size(); i++){
      quests[i]->printQuest();
      std::cout << std::endl;
    }
  }

  //Prints all information for every avaliable quest in quests.
  void printOpen(){
    for(int i = 0; i < quests.size(); i++){
      if(quests[i]->getStoryPoint() <= storyPointGarbage.size()){
        quests[i]->printQuest();
        std::cout << std::endl;
      }
    }
  }

  //Prints all the story points (numbered).
  void printStoryPoints(){
    for(int i = 0; i < storyPoints.size(); i++){
      std::cout << i << ". " << storyPoints[i] << std::endl;
    }
  }

  //Prints the deleted story points
  void printStoryPointsGarbage(){
    for(int i = 0; i < storyPointGarbage.size(); i++){
      std::cout << i << ". " << storyPointGarbage[i] << std::endl;
    }
  }

  //Prints each quest's information for category.
  void printCategory(std::string category){
    std::string notIncluded = "";
    for(int i = 0; i < quests.size(); i++){
      quests[i]->printQuest(category);
    }
  }

  //Prints quests numbered from 0
  int printNumberedQuests(){
    int counter = 0;
    for(int i = 0; i < quests.size(); i++){
      std::vector<std::string> values = quests[i]->getValues();
      if(values.size() > 0 && quests[i]->getStoryPoint() <= storyPointGarbage.size()){
        std::cout << i << ": " << values[0] << std::endl;
        counter++;
      }
    }
    return counter;
  }

  int printNumberedGarbageQuests(){
    int counter = 0;
    for(int i = 0; i < questGarbage.size(); i++){
      std::vector<std::string> values = questGarbage[i]->getValues();
      if(values.size() > 0){
        counter++;
        std::cout << i << ": " << values[0] << std::endl;
      }
    }
    return counter;
  }

  //Getter function for the size of story points
  int getStoryPointsSize(){
    return storyPoints.size();
  }

  //Getter function for the size of garbage story points
  int getStoryPointGarbageSize(){
    return storyPointGarbage.size();
  }

};

#endif
