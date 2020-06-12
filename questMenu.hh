#ifndef QUESTMENU_HH_
#define QUESTMENU_HH_

#include "quest.hh"
#include "questList.hh"

class QuestMenu{
private:

public:

  //Checks for a valid answer in the quest menu
  static int getQuestAnswer(std::string answer){
    int answerNum = -1;
    try{
      answerNum = std::stoi(answer);
    } catch(std::exception& e){
      std::cout << "Invalid answer." << std::endl;
      return -1;
    }
    if(answerNum > 5 || answerNum < 1){
      std::cout << "Invalid answer." << std::endl;
      return -1;
    }
    return answerNum;
  }

  static bool orderLtS(int i, int j){
    if(i > j){
      return true;
    }
    return false;
  }

  static std::vector<int> splitRemoveQuestAnswer(std::string answer){
    std::vector<std::string> splitWords;
    while(answer.find(" ") != -1){
      splitWords.push_back(answer.substr(0, answer.find(" ")));
      answer = answer.substr(answer.find(" ") + 1, answer.length() - answer.find(" ") - 1);
    }
    splitWords.push_back(answer);
    std::vector<int> responseInts;
    for(int i = 0; i < splitWords.size(); i++){
      int curAnswer = 0;
      try{
        curAnswer = std::stoi(splitWords[i]);
      } catch(std::exception& e){
        std::cout << "Quest number " << i << ": " << splitWords[i] << " is invalid." << std::endl;
        std::vector<int> emptyVector;
        return emptyVector;
      }
      responseInts.push_back(curAnswer);
    }
    std::sort(responseInts.begin(), responseInts.end(), orderLtS);
    return responseInts;
  }

  //Gives the menu for quest modification
  static void questMenu(QuestList* quests){
    bool done = false;
    while(!done){
      std::cout << "Would you like to remove a quest (1), reuse a quest (2), edit a quest (3), or leave to the main menu (4)?" << std::endl;
      std::string answer;
      std::cin >> answer;
      int answerNum = getQuestAnswer(answer);
      switch(answerNum){
        case 1: { //Removes a quest
          bool finishedRemoving = false;
          while(!finishedRemoving){
            int printedQuests = quests->printNumberedQuests();
            if(printedQuests != 0){
              std::cout << "Which quest(s) would you like to remove?\nUse space-separated numbers for multiple quests.\nEnter -1 to exit." << std::endl;
              std::cin.ignore();
              std::getline(std::cin, answer);
              if(answer == "-1"){
                finishedRemoving = true;
              } else {
                std::vector<int> splitAnswer = splitRemoveQuestAnswer(answer);
                if(splitAnswer.size() != 0){
                  finishedRemoving = true;
                  for(int i = 0; i < splitAnswer.size(); i++){
                    if(splitAnswer[i] < 0 || splitAnswer[i] >= printedQuests){
                      finishedRemoving = false;
                      std::cout << "Quest number " << i << ": " << splitAnswer[i] << " is invalid." << std::endl;
                      break;
                    }
                    quests->removeQuest(splitAnswer[i]);
                  }
                }
              }
            } else {
              std::cout << "No quests can be removed. Move a story point to open new quests." << std::endl;
              finishedRemoving = true;
            }
          }
          break;
        }
        case 2: {//Reuses a quest
          bool finishedRemoving = false;
          while(!finishedRemoving){
            int printedQuests = quests->printNumberedGarbageQuests();
            if(printedQuests != 0){
              std::cout << "Which quest(s) would you like to reuse?\nUse space-separated numbers for multiple quests.\nEnter -1 to exit." << std::endl;
              std::cin.ignore();
              std::getline(std::cin, answer);
              if(answer == "-1"){
                finishedRemoving = true;
              } else {
                std::vector<int> splitAnswer = splitRemoveQuestAnswer(answer);
                if(splitAnswer.size() != 0){
                  finishedRemoving = true;
                  for(int i = 0; i < splitAnswer.size(); i++){
                    if(splitAnswer[i] < 0 || splitAnswer[i] >= printedQuests){
                      finishedRemoving = false;
                      std::cout << "Quest number " << i << ": " << splitAnswer[i] << " is invalid." << std::endl;
                      break;
                    }
                    quests->reuseQuest(splitAnswer[i], 0);
                  }
                }
              }
            } else {
              finishedRemoving = true;
              std::cout << "No quests can be reused. Remove other quests for reuse before returning." << std::endl;
            }
          }
          break;
        }
        case 3: {//Modifies a quest's information
          bool questPicked = false;
          while(!questPicked){
            std::cout << "Which quest would you like to modify? (Use -1 for none)" << std::endl;
            int printedQuests = quests->printNumberedQuests();
            if(printedQuests != 0){
              std::cin >> answer;
              bool isNum = true;
              int questNum = -1;
              try{
                questNum = std::stoi(answer);
              } catch(std::exception& e){
                std::cout << "Invalid answer." << std::endl;
                isNum = false;
              }
              std::vector<Quest*> questList = quests->getQuests();
              if(isNum && (questNum < 0 || questNum >= quests->getQuests().size())){
                std::cout << "Please enter a quest's number or -1." << std::endl;
                isNum = false;
              }
              if(isNum){
                questPicked = true;
                if(questNum != -1){
                  bool doneEditing = false;
                  Quest* chosenQuest = questList[questNum];
                  while(!doneEditing){
                    chosenQuest->printQuest();
                    std::cout << "What would you like to edit? Enter either the value category you would like to edit or a boolean value you would like to add or remove. Capitalizations do matter." << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, answer);
                    std::vector<std::string> categories = chosenQuest->getValueCategories();
                    bool isCategory = false;
                    for(int i = 0; i < categories.size(); i++){
                      if(answer == categories[i]){
                        isCategory = true;
                        std::cout << "What would you like to change " << answer << " to?" << std::endl;
                        std::getline(std::cin, answer);
                        chosenQuest->changeValue(i, answer);
                      }
                    }
                    if(!isCategory){
                      std::vector<std::string> bools = chosenQuest->getBools();
                      bool isBool = false;
                      for(int i = 0; i < bools.size(); i++){
                        if(bools[i] == answer){
                          isBool = true;
                          chosenQuest->removeBool(i);
                          break;
                        }
                      }
                      if(!isBool){
                        chosenQuest->addBool(answer);
                      }
                    }
                    chosenQuest->printQuest();
                    bool answerGiven = false;
                    while(!answerGiven){
                      std::cout << "Are you finished modifying this quest? (yes/no)" << std::endl;
                      std::cin >> answer;
                      std::cout << answer << std::endl;
                      if(answer == "yes"){
                        answerGiven = true;
                        doneEditing = true;
                      } else if (answer == "no"){
                        answerGiven = true;
                      } else {
                        std::cout << "Invalid answer." << std::endl;
                      }
                    }
                  }
                }
              }
            } else {
              questPicked = true;
              std::cout << "No quests can be modified. Pass a story point to open quests for modification." << std::endl;
            }
          }
          break;
        }
        case 4: //Exits the quest menu
          done = !done;
          break;
      }
      if(!done){
        std::cout << "Are you finished editing quest information? (yes/no)" << std::endl;
        std::cin >> answer;
        if(answer == "yes"){
          done = !done;
        } else if(answer != "no"){
          std::cout << "Invalid response." << std::endl;
        }
      }
    }
  }

};

#endif
