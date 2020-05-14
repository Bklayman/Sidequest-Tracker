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

  //Moves a story point from storyPoints to storyPointGarbage.
  void removeStoryPoint(int index){
    std::string removedStoryPoint = storyPoints[index];
    storyPoints.erase(storyPoints.begin() + index);
    storyPointGarbage.push_back(removedStoryPoint);
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
    for(int i = storyPointGarbage.size() - 1; i >= indexToUse; i++){
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
      if(quests[i]->getStoryPoint() < storyPointGarbage.size()){
        quests[i]->printQuest();
        std::cout << std::endl;
      }
    }
  }

  //Prints all the story points (numbered).
  void printStoryPoints(){
    for(int i = 0; i < storyPoints.size(); i++){
      std::cout << i + 1 << ". " << storyPoints[i] << std::endl;
    }
  }

  //Prints each quest's information for category.
  void printCategory(std::string category){
    std::string notIncluded = "";
    for(int i = 0; i < quests.size(); i++){
      quests[i]->printQuest(category);
    }
  }

};

#endif
