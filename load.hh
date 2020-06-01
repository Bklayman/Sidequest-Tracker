#ifndef LOAD_HH
#define LOAD_HH

#include "quest.hh"
#include "questList.hh"

class Load{
private:

public:

  //Removes spaces at the beginning and end of a string
  static std::string trim(std::string originalString){
    while(originalString.substr(0, 1) == " "){
      originalString = originalString.substr(1, originalString.length() - 1);
    }
    while(originalString.substr(originalString.length() - 1, 1) == " "){
      originalString = originalString.substr(0, originalString.length() - 1);
    }
    return originalString;
  }

  //Addsa portion of saveInfo to a given Quest category. Returns the rest of saveInfo.
  static std::string addToQuest(int nameLocation, int valueIndex, std::string saveInfo, Quest* curQuest){
    std::string categoryName = saveInfo.substr(5, nameLocation - 5);
    saveInfo = saveInfo.substr(nameLocation, saveInfo.length() - nameLocation);
    switch(valueIndex){
      case 0:
        curQuest->addValue(categoryName);
        break;
      case 1:
        curQuest->addValueCategory(categoryName);
        break;
      case 2:
        curQuest->addBool(categoryName);
        break;
      case 3:
        curQuest->setStoryPoint(std::stoi(categoryName));
        break;
    }
    return saveInfo;
  }

  //Gets the quest list associated with a base or save file
  static QuestList* getQuestList(char* fileName, int baseOrSave){
    std::ifstream file;
    file.open(fileName);
    if(!file.is_open()){
      std::cout << "ERROR: File " << fileName << " does not exist." << std::endl;
      exit(1);
    }
    std::string fileLine;
    QuestList* quests = new QuestList();
    //Loads info from base file
    if(baseOrSave == 1){
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
      //Saves quests
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
            saveInfo = addToQuest(nextValueStart, 0, saveInfo, curQuest);
          } else {
            saveInfo = addToQuest(categoryStart, 0, saveInfo, curQuest);
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
            saveInfo = addToQuest(nextCategoryStart, 1, saveInfo, curQuest);
            garbageBefore-= nextCategoryStart;
          } else {
            saveInfo = addToQuest(garbageBefore, 1, saveInfo, curQuest);
          }
          categoryStart = saveInfo.find("..+!W");
        }
        questSPStart = saveInfo.find("..+!U");
        boolStart = saveInfo.find("..+!B");
        while(boolStart != -1 && boolStart < questSPStart){
          int nextBoolStart = saveInfo.find("..+!B", boolStart + 1);
          if(nextBoolStart == -1 || nextBoolStart > questSPStart){
            saveInfo = addToQuest(questSPStart, 2, saveInfo, curQuest);
          } else {
            saveInfo = addToQuest(nextBoolStart, 2, saveInfo, curQuest);
          }
          boolStart = saveInfo.find("..+!B");
          questSPStart = saveInfo.find("..+!U");
        }
        int nextQuestStart = saveInfo.find("..+!L");
        if(nextQuestStart == -1){
          int garbageQuestStart = saveInfo.find("..-!Y");
          if(garbageQuestStart != -1){
            saveInfo = addToQuest(garbageQuestStart, 3, saveInfo, curQuest);
          } else {
            int storyPointStart = saveInfo.find("..*!S");
            if(storyPointStart != -1){
              saveInfo = addToQuest(storyPointStart, 3, saveInfo, curQuest);
            } else {
              addToQuest(saveInfo.length(), 3, saveInfo, curQuest);
              saveInfo = "";
            }
          }
        } else {
          saveInfo = addToQuest(nextQuestStart, 3, saveInfo, curQuest);
        }
        quests->addQuest(curQuest);
        questStart = saveInfo.find("..+!L");
      }
      int nextGarbageQuest = saveInfo.find("..-!Y");
      foundQuest = false;
      //Saves garbage quests
      while(nextGarbageQuest != -1){
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
            saveInfo = addToQuest(nextValueStart, 0, saveInfo, curQuest);
          } else {
            saveInfo = addToQuest(categoryStart, 0, saveInfo, curQuest);
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
            saveInfo = addToQuest(nextCategoryStart, 1, saveInfo, curQuest);
            garbageBefore-= nextCategoryStart;
          } else {
            saveInfo = addToQuest(garbageBefore, 1, saveInfo, curQuest);
          }
          categoryStart = saveInfo.find("..+!W");
        }
        questSPStart = saveInfo.find("..+!U");
        boolStart = saveInfo.find("..+!B");
        while(boolStart != -1 && boolStart < questSPStart){
          int nextBoolStart = saveInfo.find("..+!B", boolStart + 1);
          if(nextBoolStart == -1 || nextBoolStart > questSPStart){
            saveInfo = addToQuest(questSPStart, 2, saveInfo, curQuest);
          } else {
            saveInfo = addToQuest(nextBoolStart, 2, saveInfo, curQuest);
          }
          boolStart = saveInfo.find("..+!B");
          questSPStart = saveInfo.find("..+!U");
        }
        int nextQuestStart = saveInfo.find("..-!Y");
        if(nextQuestStart == -1){
          int storyPointStart = saveInfo.find("..*!S");
          if(storyPointStart != -1){
            saveInfo = addToQuest(storyPointStart, 3, saveInfo, curQuest);
          } else {
            addToQuest(saveInfo.length(), 3, saveInfo, curQuest);
            saveInfo = "";
          }
        } else {
          saveInfo = addToQuest(nextQuestStart, 3, saveInfo, curQuest);
        }
        quests->addQuestGarbage(curQuest);
        nextGarbageQuest = saveInfo.find("..-!Y");
      }
      int storyPointStart = saveInfo.find("..*!S");
      //Saves the story points
      while(storyPointStart != -1){
        int nextStoryPoint = saveInfo.find("..*!S", storyPointStart + 1);
        if(nextStoryPoint == -1){
          int garbageSP = saveInfo.find("..*!D");
          std::string storyPointName = saveInfo.substr(5, garbageSP - 5);
          quests->addStoryPoint(storyPointName);
          if(garbageSP == -1){
            saveInfo = "";
          } else {
            saveInfo = saveInfo.substr(garbageSP, saveInfo.length() - garbageSP);
          }
        } else {
          std::string storyPointName = saveInfo.substr(5, nextStoryPoint - 5);
          quests->addStoryPoint(storyPointName);
          saveInfo = saveInfo.substr(nextStoryPoint, saveInfo.length() - nextStoryPoint);
        }
        storyPointStart = saveInfo.find("..*!S");
      }
      int garbageSPStart = saveInfo.find("..*!D");
      //Saves the garbage story points
      while(garbageSPStart != -1){
        int nextGarbagePoint = saveInfo.find("..*!D", garbageSPStart + 1);
        if(nextGarbagePoint != -1){
          std::string garbagePointName = saveInfo.substr(5, nextGarbagePoint - 5);
          quests->addGarbageStoryPoint(garbagePointName);
          saveInfo = saveInfo.substr(nextGarbagePoint, saveInfo.length() - nextGarbagePoint);
        } else {
          std::string garbagePointName = saveInfo.substr(5, saveInfo.length() - 5);
          quests->addGarbageStoryPoint(garbagePointName);
          saveInfo = "";
        }
        garbageSPStart = saveInfo.find("..*!D");
      }
    }
    file.close();
    return quests;
  }

};

#endif
