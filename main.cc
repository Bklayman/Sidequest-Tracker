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
  } else {
    //TODO: Load Save
  }
  return quests;
}

//Prints a list of the possible actions to take in the main menu
void help(){
  std::cout << "List of actions:\n\thelp / h: Get this list\n\texit / e: Exit this program (does not save progress)\n\tsave / s: Go to save menu\n\tstory / st: Go to story point menu\n\tquest / q: Go to quest menu\n\tprint / p: Go to print menu" << std::endl;
}

//Executes the action desired from the main menu
bool takeAction(int code, QuestList* quests){
  switch(code){
    case 1:
      help();
      break;
  }
  //TODO Make this go to every action
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
  QuestList* quests = getQuestList(argv[1], answer);
  bool wantExit = false;
  while(!wantExit){
    std::cout << "Enter what action you would like to take (type 'help' or 'h' for help)" << std::endl;
    std::cin >> answer;
    int actionCode = checkActions(answer);
    wantExit = takeAction(actionCode, quests);
  }
}
