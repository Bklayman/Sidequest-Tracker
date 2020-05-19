#ifndef SAVE_HH
#define SAVE_HH

class Save{
private:

public:
  //Saves data to a specified file. Requirements are shown in the README.
  static void save(QuestList* quests){
    std::cout << "What would you like to name your file? (This will overwrite any existing file with the same name)" << std::endl;
    std::string answer;
    std::cin >> answer;
    if(answer.substr(answer.length() - 4, 4) != ".txt"){
      answer += ".txt";
    }
    std::string saveText = "";
    //Saves quests
    std::vector<Quest*> questsList = quests->getQuests();
    for(int i = 0; i < questsList.size(); i++){
      std::vector<std::string> curValues = questsList[i]->getValues();
      bool emptyQuest = false;
      if(curValues.size() == 0){
        emptyQuest = !emptyQuest;
      }
      if(!emptyQuest){
        saveText+= "..+!L";
        for(int j = 0; j < curValues.size(); j++){
          saveText+= "..+!A";
          saveText+= curValues[j];
        }
        std::vector<std::string> curCategories = questsList[i]->getValueCategories();
        for(int j = 0; j < curCategories.size(); j++){
          saveText+= "..+!W";
          saveText+= curCategories[j];
        }
        std::vector<std::string> curBools = questsList[i]->getBools();
        for(int j = 0; j < curBools.size(); j++){
          saveText+= "..+!B";
          saveText+= curBools[j];
        }
        std::string textToAdd = "..+!U" + std::to_string(questsList[i]->getStoryPoint());
        saveText+= textToAdd;
      }
    }
    //saves garbage quests
    std::vector<Quest*> questsGarbageList = quests->getQuestGarbage();
    for(int i = 0; i < questsGarbageList.size(); i++){
      saveText+= "..-!Y";
      std::vector<std::string> curValues = questsGarbageList[i]->getValues();
      for(int j = 0; j < curValues.size(); j++){
        saveText+= "..+!A";
        saveText+= curValues[j];
      }
      std::vector<std::string> curCategories = questsGarbageList[i]->getValueCategories();
      for(int j = 0; j < curCategories.size(); j++){
        saveText+= "..+!W";
        saveText+= curCategories[j];
      }
      std::vector<std::string> curBools = questsGarbageList[i]->getBools();
      for(int j = 0; j < curBools.size(); j++){
        saveText+= "..+!B";
        saveText+= curBools[j];
      }
      std::string textToAdd = "..+!U" + std::to_string(questsGarbageList[i]->getStoryPoint());
      saveText+= textToAdd;
    }
    //Saves story points
    std::vector<std::string> storyPoints = quests->getStoryPoints();
    for(int i = 0; i < storyPoints.size(); i++){
      saveText+= "..*!S";
      saveText+= storyPoints[i];
    }
    //Saves garbage story points
    std::vector<std::string> storyPointsGarbage = quests->getStoryPointGarbage();
    for(int i = 0; i < storyPointsGarbage.size(); i++){
      saveText+= "..*!D";
      saveText+= storyPointsGarbage[i];
    }
    //Saves everything to specified file
    std::ofstream file;
    answer = "Save_Files/" + answer;
    file.open(answer);
    file << saveText;
    file.close();
  }

  //Asks the user how to save
  static void saveMenu(QuestList* quests){
    bool doneSaving = false;
    while(!doneSaving){
      std::cout << "Would you like to save your progress? (yes / no)" << std::endl;
      std::string answer;
      std::cin >> answer;
      if(answer == "yes"){
        save(quests);
        doneSaving = true;
      } else if (answer == "no"){
        doneSaving = true;
      } else {
        std::cout << "Invalid answer given." << std::endl;
      }
    }
  }

};

#endif
