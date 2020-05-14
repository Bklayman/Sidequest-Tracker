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
            std::string word2 = fileLine.substr(spacePlace + 1, fileLine.length() - spacePlace - 1);
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
    //TODO: Load Save
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
  for(int i = 0; i < questsList.size(); i++){
    saveText+= "..+!L";
    std::vector<std::string> curValues = questsList[i]->getValues();
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
}
