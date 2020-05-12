#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "quest.hh"
#include "questList.hh"

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
    }
    while(readSuccess){
      std::cout << fileLine << std::endl;
      if(fileLine.substr(0, 1) == "-"){
        storyPointNum++;
        quests->addStoryPoint(fileLine.substr(1, fileLine.length() - 1));
      } else {
        //TODO: Create Quest
      }
      if(!(std::getline(file, fileLine))){
        readSuccess = false;
      }
    }
  } else {
    //TODO: Load Save
  }
  return quests;
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
  quests->printAll();
}
