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
    saveText = encrypt(saveText);
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

  static std::string flipByGroup(std::string input, int groupSize){
    std::string result = "";
    for(int i = 0; i < input.length(); i+= groupSize){
      if(input.length() - i >= groupSize){
        result+= input.substr(i + groupSize - 1, 1);
        result+= input.substr(i + 1, groupSize - 2);
        result+= input.substr(i, 1);
      } else {
        for(int j = i; j < input.length(); j++){
          result+= input.substr(j, 1);
        }
        break;
      }
    }
    return result;
  }

  static std::string flipWhole(std::string input){
    std::string result = "";
    for(int i = 0; i < input.length(); i++){
      result+= input.substr(input.length() - i - 1, 1);
    }
    return result;
  }

  static std::string flipHalf(std::string input){
    std::string result = "";
    if(input.length() % 2 == 1){
      for(int i = 0; i < input.length(); i++){
        if(i % 2 == 0){
          result+= input.substr(input.length() - i - 1, 1);
        } else {
          result+= input.substr(i, 1);
        }
      }
    } else {
      for(int i = 0; i < input.length(); i++){
        if((i % 2 == 0 && i < input.length() / 2) || (i % 2 == 1 && i > input.length() / 2)){
          result+= input.substr(input.length() - i - 1, 1);
        } else {
          result+= input.substr(i, 1);
        }
      }
    }
    return result;
  }

  static std::string encrypt(std::string input){
    input = flipByGroup(input, 4);
    input = flipWhole(input);
    input = flipHalf(input);
    return input;
  }

  static std::string unEncrypt(std::string input){
    input = flipHalf(input);
    input = flipWhole(input);
    input = flipByGroup(input, 4);
    return input;
  }

};

#endif
