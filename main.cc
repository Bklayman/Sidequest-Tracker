#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

#include "quest.hh"
#include "questList.hh"

//Removes spaces at the beginning and end of a string
std::string trim(std::string originalString){
  while(originalString.substr(0, 1) == " "){
    originalString = originalString.substr(1, originalString.length() - 1);
  }
  while(originalString.substr(originalString.length() - 1, 1) == " "){
    originalString = originalString.substr(0, originalString.length() - 1);
  }
  return originalString;
}

//Gets the quest list associated with a base or save file
QuestList* getQuestList(char* fileName, std::string baseOrSave){
  std::ifstream file;
  file.open(fileName);
  if(!file.is_open()){
    std::cout << "ERROR: File " << fileName << " does not exist." << std::endl;
    exit(1);
  }
  std::string fileLine;
  QuestList* quests = new QuestList();
  //Loads info from base file
  if(baseOrSave == "1"){
    int storyPointNum = 0;
    bool readSuccess = true;
    if(!(std::getline(file, fileLine))){
      readSuccess = false;
    } else {
      fileLine = trim(fileLine);
    }
    while(readSuccess){
      if(fileLine.substr(0, 1) == "-"){
        storyPointNum++;
        quests->addStoryPoint(fileLine.substr(1, fileLine.length() - 1));
        if(!(std::getline(file, fileLine))){
          readSuccess = false;
        } else {
          fileLine = trim(fileLine);
        }
      } else {
        Quest* curQuest = new Quest();
        curQuest->setStoryPoint(storyPointNum);
        bool sameQuest = true;
        while(sameQuest){
          int spacePlace = fileLine.find(" ");
          if(spacePlace == -1 && fileLine.length() != 1){
            curQuest->addBool(fileLine);
          } else if(fileLine.length() != 1){
            std::string word1 = fileLine.substr(0, spacePlace);
            std::string word2 = fileLine.substr(spacePlace + 1, fileLine.length() - spacePlace);
            if(word1.substr(word1.length() - 1, 1) == ":"){
              word1 = word1.substr(0, word1.length() - 1);
            }
            curQuest->addValue(word2, word1);
          }
          if(!(std::getline(file, fileLine))){
            readSuccess = false;
          }
          if(readSuccess){
            fileLine = trim(fileLine);
            if(fileLine.substr(0, 1) == "-" || fileLine.substr(0, 5) == "Title"){
              sameQuest = false;
              quests->addQuest(curQuest);
            }
          } else {
            sameQuest = false;
            quests->addQuest(curQuest);
          }
        }
      }
    }
  //Loads info from save file
  } else {
    std::string saveInfo = "";
    std::string info;
    while(file >> info){
      saveInfo+= " " + info;
    }
    int questStart = saveInfo.find("..+!L");
    bool foundQuest = false;
    while(questStart != -1){
      if(foundQuest){
        saveInfo = saveInfo.substr(5, saveInfo.length() - 5);
      } else {
        saveInfo = saveInfo.substr(6, saveInfo.length() - 5);
      foundQuest = !foundQuest;
      }
      Quest* curQuest = new Quest();
      int valueStart = saveInfo.find("..+!A");
      int categoryStart = saveInfo.find("..+!W");
      while(valueStart < categoryStart && valueStart != -1){
        int nextValueStart = saveInfo.find("..+!A", valueStart + 1);
        if(nextValueStart != -1 && nextValueStart < categoryStart){
          std::string valueName = saveInfo.substr(5, nextValueStart - 5);
          std::cout << "Value Name: " << valueName << std::endl;
          curQuest->addValue(valueName);
          saveInfo = saveInfo.substr(nextValueStart, saveInfo.length() - nextValueStart);
        } else {
          std::string valueName = saveInfo.substr(5, categoryStart - 5);
          std::cout << "Value Name: " << valueName << std::endl;
          curQuest->addValue(valueName);
          saveInfo = saveInfo.substr(categoryStart, saveInfo.length() - categoryStart);
        }
        valueStart = saveInfo.find("..+!A");
        categoryStart = saveInfo.find("..+!W");
      }
      int boolStart = saveInfo.find("..+!B");
      int questSPStart = saveInfo.find("..+!U");
      int garbageBefore = 0;
      if(boolStart == -1 || boolStart > questSPStart){
        garbageBefore = questSPStart;
      } else {
        garbageBefore = boolStart;
      }
      while(categoryStart != -1 && categoryStart < garbageBefore){
        int nextCategoryStart = saveInfo.find("..+!W", categoryStart + 1);
        if(nextCategoryStart != -1 && nextCategoryStart < garbageBefore){
          std::string categoryName = saveInfo.substr(5, nextCategoryStart - 5);
          std::cout << "Category Name: " << categoryName << std::endl;
          curQuest->addValueCategory(categoryName);
          saveInfo = saveInfo.substr(nextCategoryStart, saveInfo.length() - nextCategoryStart);
          garbageBefore-= nextCategoryStart;
        } else {
          std::string categoryName = saveInfo.substr(5, garbageBefore - 5);
          std::cout << "Category Name: " << categoryName << std::endl;
          curQuest->addValueCategory(categoryName);
          saveInfo = saveInfo.substr(garbageBefore, saveInfo.length() - garbageBefore);
        }
        categoryStart = saveInfo.find("..+!W");
      }
      questSPStart = saveInfo.find("..+!U");
      boolStart = saveInfo.find("..+!B");
      while(boolStart != -1 && boolStart < questSPStart){
        int nextBoolStart = saveInfo.find("..+!B", boolStart + 1);
        if(nextBoolStart == -1 || nextBoolStart > questSPStart){
          std::string boolName = saveInfo.substr(5, questSPStart - 5);
          std::cout << "Bool Name: " << boolName << std::endl;
          curQuest->addBool(boolName);
          saveInfo = saveInfo.substr(questSPStart, saveInfo.length() - questSPStart);
        } else {
          std::string boolName = saveInfo.substr(5, nextBoolStart - 5);
          std::cout << "Bool Name: " << boolName << std::endl;
          curQuest->addBool(boolName);
          saveInfo = saveInfo.substr(nextBoolStart, saveInfo.length() - nextBoolStart);
        }
        boolStart = saveInfo.find("..+!B");
        questSPStart = saveInfo.find("..+!U");
      }
      int nextQuestStart = saveInfo.find("..+!L");
      if(nextQuestStart == -1){
        int garbageQuestStart = saveInfo.find("..-!Y");
        //Breaks Here
        if(garbageQuestStart != -1){
          std::string storyPointName = saveInfo.substr(5, garbageQuestStart - 5);
          std::cout << "Story Point Num: " << storyPointName << std::endl;
          curQuest->setStoryPoint(std::stoi(storyPointName));
          saveInfo = saveInfo.substr(garbageQuestStart, saveInfo.length() - garbageQuestStart);
        } else {
          int storyPointStart = saveInfo.find("..*!S");
          if(storyPointStart != -1){
            std::string storyPointName = saveInfo.substr(5, storyPointStart - 5);
            std::cout << "Stpry Point Num: " << storyPointName << std::endl;
            curQuest->setStoryPoint(std::stoi(storyPointName));
            saveInfo = saveInfo.substr(storyPointStart, saveInfo.length() - storyPointStart);
          } else {
            std::string storyPointName = saveInfo.substr(5, saveInfo.length() - 5);
            std::cout << "Stpry Point Num: " << storyPointName << std::endl;
            curQuest->setStoryPoint(std::stoi(storyPointName));
            saveInfo = "";
          }
        }
      } else {
        std::string storyPointName = saveInfo.substr(5, nextQuestStart - 5);
        std::cout << "Story Point Num: " << storyPointName << std::endl;
        curQuest->setStoryPoint(std::stoi(storyPointName));
        saveInfo = saveInfo.substr(nextQuestStart, saveInfo.length() - nextQuestStart);
      }
      quests->addQuest(curQuest);
    }
    int nextGarbageQuest = saveInfo.find("..-!Y");
    while(nextGarbageQuest != -1){
      saveInfo = saveInfo.substr(5, saveInfo.length() - 5);
      Quest* curQuest = new Quest();
      int valueStart = saveInfo.find("..+!A");
      int categoryStart = saveInfo.find("..+!W");
      while(valueStart < categoryStart){
        int nextValueStart = saveInfo.find("..+!A", valueStart + 1);
        if(nextValueStart != -1 && nextValueStart < categoryStart){
          std::string valueName = saveInfo.substr(5, nextValueStart - 5);
          curQuest->addValue(valueName);
          saveInfo = saveInfo.substr(nextValueStart, saveInfo.length() - nextValueStart);
        } else {
          std::string valueName = saveInfo.substr(5, categoryStart - 5);
          curQuest->addValue(valueName);
          saveInfo = saveInfo.substr(categoryStart, saveInfo.length() - categoryStart);
        }
        valueStart = saveInfo.find("..+!A");
      }
      int boolStart = saveInfo.find("..+!B");
      int questSPStart = saveInfo.find("..+!U");
      int garbageBefore = 0;
      if(boolStart == -1 || boolStart > questSPStart){
        garbageBefore = questSPStart;
      } else {
        garbageBefore = boolStart;
      }
      while(categoryStart != -1 && categoryStart < garbageBefore){
        int nextCategoryStart = saveInfo.find("..+!W", categoryStart + 1);
        if(nextCategoryStart != -1 && nextCategoryStart < garbageBefore){
          std::string categoryName = saveInfo.substr(5, nextCategoryStart - 5);
          curQuest->addValueCategory(categoryName);
          saveInfo = saveInfo.substr(nextCategoryStart, saveInfo.length() - nextCategoryStart);
        } else {
          std::string categoryName = saveInfo.substr(5, garbageBefore - 5);
          curQuest->addValueCategory(categoryName);
          saveInfo = saveInfo.substr(garbageBefore, saveInfo.length() - garbageBefore);
        }
        categoryStart = saveInfo.find("..+!W");
      }
      while(boolStart != -1 && boolStart < questSPStart){
        int nextBoolStart = saveInfo.find("..+!B", boolStart + 1);
        if(nextBoolStart == -1 || nextBoolStart > questSPStart){
          std::string boolName = saveInfo.substr(5, questSPStart - 5);
          curQuest->addBool(boolName);
          saveInfo = saveInfo.substr(questSPStart, saveInfo.length() - questSPStart);
        } else {
          std::string boolName = saveInfo.substr(5, nextBoolStart - 5);
          curQuest->addBool(boolName);
          saveInfo = saveInfo.substr(nextBoolStart, saveInfo.length() - nextBoolStart);
        }
        boolStart = saveInfo.find("..+!B");
      }
      int nextQuestStart = saveInfo.find("..+!L");
      if(nextQuestStart == -1){
        int garbageQuestStart = saveInfo.find("..-!Y");
        std::string storyPointName = saveInfo.substr(5, garbageQuestStart - 5);
        curQuest->setStoryPoint(std::stoi(storyPointName));
        saveInfo = saveInfo.substr(garbageQuestStart, saveInfo.length() - garbageQuestStart);
      } else {
        std::string storyPointName = saveInfo.substr(5, nextQuestStart - 5);
        curQuest->setStoryPoint(std::stoi(storyPointName));
        saveInfo = saveInfo.substr(nextQuestStart, saveInfo.length() - nextQuestStart);
      }
      quests->addQuestGarbage(curQuest);
    }
    int storyPointStart = saveInfo.find("..*!S");
    while(storyPointStart != -1){
      int nextStoryPoint = saveInfo.find("..*!S", storyPointStart + 1);
      if(nextStoryPoint == -1){
        int garbageSP = saveInfo.find("..*!D");
        std::string storyPointName = saveInfo.substr(5, garbageSP - 5);
        quests->addStoryPoint(storyPointName);
        saveInfo = saveInfo.substr(garbageSP, saveInfo.length() - garbageSP);
      } else {
        std::string storyPointName = saveInfo.substr(5, nextStoryPoint - 5);
        quests->addStoryPoint(storyPointName);
        saveInfo = saveInfo.substr(nextStoryPoint, saveInfo.length() - nextStoryPoint);
      }
      storyPointStart = saveInfo.find("..*!S");
    }
    int garbageSPStart = saveInfo.find("..*!D");
    while(garbageSPStart != -1){
      int nextGarbagePoint = saveInfo.find("..*!D", garbageSPStart + 1);
      if(nextGarbagePoint != -1){
        std::string garbagePointName = saveInfo.substr(5, saveInfo.length() - 5);
        quests->addGarbageStoryPoint(garbagePointName);
        saveInfo = "";
      } else {
        std::string garbagePointName = saveInfo.substr(5, nextGarbagePoint - 5);
        quests->addGarbageStoryPoint(garbagePointName);
        saveInfo = saveInfo.substr(nextGarbagePoint, saveInfo.length() - nextGarbagePoint);
      }
    }
  }
  return quests;
}

//Prints a list of the possible actions to take in the main menu
void help(){
  std::cout << "List of actions:\n\thelp / h: Get this list\n\texit / e: Exit this program (does not save progress)\n\tsave / s: Go to save menu\n\tstory / st: Go to story point menu\n\tquest / q: Go to quest menu\n\tprint / p: Go to print menu" << std::endl;
}

//Saves data to a specified file. Requirements are shown in the README.
void save(QuestList* quests){
  std::cout << "What would you like to name your file? (This will overwrite any existing file with the same name)" << std::endl;
  std::string answer;
  std::cin >> answer;
  if(answer.substr(answer.length() - 4, 4) != ".txt"){
    answer += ".txt";
  }
  std::string saveText = "";
  //Saves quests
  std::vector<Quest*> questsList = quests->getQuests();
  for(int i = 1; i < questsList.size(); i++){
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
void saveMenu(QuestList* quests){
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

void storyPointMenu(QuestList* quests){
  //TODO
}

void questMenu(QuestList* quests){
  //TODO
}

//Asks the user what to print
void printMenu(QuestList* quests){
  bool donePrinting = false;
  while(!donePrinting){
    std::cout << "Would you like to print all quest information (1) or only one information category (2)?" << std::endl;
    std::string answer;
    std::cin >> answer;
    if(answer == "1"){
      quests->printOpen();
    } else if(answer == "2"){
      std::cout << "What category would you like to print by?" << std::endl;
      std::cin >> answer;
      quests->printCategory(answer);
    } else {
      std::cout << "That is not an accepted input." << std::endl;
    }
    std::cout << "Would you like to return to the main menu? (yes/no)" << std::endl;
    std::cin >> answer;
    if(answer == "yes"){
      donePrinting = !donePrinting;
    }
  }
}

//Executes the action desired from the main menu
bool takeAction(int code, QuestList* quests){
  switch(code){
    case -1:
      std::cout << "Invalid action. Use help or h for a list of actions." << std::endl;
      break;
    case 1:
      help();
      break;
    case 2:
      return true;
    case 3:
      saveMenu(quests);
      break;
    case 4:
      storyPointMenu(quests);
      break;
    case 5:
      questMenu(quests);
      break;
    case 6:
      printMenu(quests);
      break;
    default:
      std::cout << "Error: Invalid action code given." << std::endl;
      break;
  }
  return false;
}

//Checks to see if a given action at the main menu associates with a possible action (not case sensitive). Returns -1 if none are found.
//All codes should be self-explainable
int checkActions(std::string caseAction){
  std::locale loc;
  std::string action;
  for(std::string::size_type i = 0; i < caseAction.length(); i++){
    action += std::tolower(caseAction[i], loc);
  }
  if(action == "help" || action == "h"){
    return 1;
  }
  if(action == "exit" || action == "e"){
    return 2;
  }
  if(action == "save" || action == "s"){
    return 3;
  }
  if(action == "story" || action == "st"){
    return 4;
  }
  if(action == "quest" || action == "q"){
    return 5;
  }
  if(action == "print" || action == "p"){
    return 6;
  }
  return -1;
}

int main(int argc, char** argv){
  if(argc != 2){
    std::cout << "Usage: ./main [Base .txt file / Saved .txt file]" << std::endl;
    exit(0);
  }
  //Gets user input on the type of file used
  std::cout << "Is this file a Base or a Save? (1 or 2)" << std::endl;
  bool validAnswer = false;
  std::string answer;
  while(!validAnswer){
    std::cin >> answer;
    if(answer == "1" || answer == "2"){
      validAnswer = true;
    } else {
      std::cout << "Please enter either 1 (Base) or 2 (Save)." << std::endl;
    }
  }
  //Loads the file and uses the main menu to get user input and execute that input.
  QuestList* quests = getQuestList(argv[1], answer);
  bool wantExit = false;
  while(!wantExit){
    std::cout << "Enter what action you would like to take (type 'help' or 'h' for help)" << std::endl;
    std::cin >> answer;
    int actionCode = checkActions(answer);
    wantExit = takeAction(actionCode, quests);
  }
  std::vector<Quest*> questList = quests->getQuests();
  std::vector<Quest*> garbageList = quests->getQuestGarbage();
  for(int i = 0; i < questList.size(); i++){
    delete questList[i];
  }
  for(int i = 0; i < garbageList.size(); i++){
    delete garbageList[i];
  }
  delete quests;
}
