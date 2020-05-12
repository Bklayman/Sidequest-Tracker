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

  std::vector<Quest*> getQuests(){
    return quests;
  }

  std::vector<Quest*> getQuestGarbage(){
    return questGarbage;
  }

  std::vector<std::string> getStoryPoints(){
    return storyPoints;
  }

  std::vector<std::string> getStoryPointGarbage(){
    return storyPointGarbage;
  }

  void addQuest(Quest* addedQuest){
    quests.push_back(addedQuest);
  }

  void removeQuest(int index){
    Quest* removedQuest = quests[index];
    quests.erase(quests.begin() + index);
    questGarbage.push_back(removedQuest);
  }

  void addStoryPoint(std::string addedPoint){
    storyPoints.push_back(addedPoint);
  }

  void removeStoryPoint(int index){
    std::string removedStoryPoint = storyPoints[index];
    storyPoints.erase(storyPoints.begin() + index);
    storyPointGarbage.push_back(removedStoryPoint);
  }

  void reuseQuest(int indexToUse, int indexToAdd){
    Quest* reusedQuest = questGarbage[indexToUse];
    questGarbage.erase(questGarbage.begin() + indexToUse);
    quests.insert(quests.begin() + indexToAdd, reusedQuest);
  }

  void reuseStoryPoint(int indexToUse, int indexToAdd){
    std::string reusedStoryPoint = storyPointGarbage[indexToUse];
    storyPointGarbage.erase(storyPointGarbage.begin() + indexToUse);
    storyPoints.insert(storyPoints.begin() + indexToAdd, reusedStoryPoint);
  }

  void printAll(){
    for(int i = 0; i < quests.size(); i++){
      quests[i]->printQuest();
      std::cout << std::endl;
    }
  }

};

#endif
