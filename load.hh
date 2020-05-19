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

  //Gets the quest list associated with a base or save file
  static QuestList* getQuestList(char* fileName, std::string baseOrSave){
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
            curQuest->addValue(valueName);
            saveInfo = saveInfo.substr(nextValueStart, saveInfo.length() - nextValueStart);
          } else {
            std::string valueName = saveInfo.substr(5, categoryStart - 5);
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
            curQuest->addValueCategory(categoryName);
            saveInfo = saveInfo.substr(nextCategoryStart, saveInfo.length() - nextCategoryStart);
            garbageBefore-= nextCategoryStart;
          } else {
            std::string categoryName = saveInfo.substr(5, garbageBefore - 5);
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
            curQuest->addBool(boolName);
            saveInfo = saveInfo.substr(questSPStart, saveInfo.length() - questSPStart);
          } else {
            std::string boolName = saveInfo.substr(5, nextBoolStart - 5);
            curQuest->addBool(boolName);
            saveInfo = saveInfo.substr(nextBoolStart, saveInfo.length() - nextBoolStart);
          }
          boolStart = saveInfo.find("..+!B");
          questSPStart = saveInfo.find("..+!U");
        }
        int nextQuestStart = saveInfo.find("..+!L");
        if(nextQuestStart == -1){
          int garbageQuestStart = saveInfo.find("..-!Y");
          if(garbageQuestStart != -1){
            std::string storyPointName = saveInfo.substr(5, garbageQuestStart - 5);
            curQuest->setStoryPoint(std::stoi(storyPointName));
            saveInfo = saveInfo.substr(garbageQuestStart, saveInfo.length() - garbageQuestStart);
          } else {
            int storyPointStart = saveInfo.find("..*!S");
            if(storyPointStart != -1){
              std::string storyPointName = saveInfo.substr(5, storyPointStart - 5);
              curQuest->setStoryPoint(std::stoi(storyPointName));
              saveInfo = saveInfo.substr(storyPointStart, saveInfo.length() - storyPointStart);
            } else {
              std::string storyPointName = saveInfo.substr(5, saveInfo.length() - 5);
              curQuest->setStoryPoint(std::stoi(storyPointName));
              saveInfo = "";
            }
          }
        } else {
          std::string storyPointName = saveInfo.substr(5, nextQuestStart - 5);
          curQuest->setStoryPoint(std::stoi(storyPointName));
          saveInfo = saveInfo.substr(nextQuestStart, saveInfo.length() - nextQuestStart);
        }
        quests->addQuest(curQuest);
        questStart = saveInfo.find("..+!L");
      }
      int nextGarbageQuest = saveInfo.find("..-!Y");
      foundQuest = false;
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
            std::string valueName = saveInfo.substr(5, nextValueStart - 5);
            curQuest->addValue(valueName);
            saveInfo = saveInfo.substr(nextValueStart, saveInfo.length() - nextValueStart);
          } else {
            std::string valueName = saveInfo.substr(5, categoryStart - 5);
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
            curQuest->addValueCategory(categoryName);
            saveInfo = saveInfo.substr(nextCategoryStart, saveInfo.length() - nextCategoryStart);
            garbageBefore-= nextCategoryStart;
          } else {
            std::string categoryName = saveInfo.substr(5, garbageBefore - 5);
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
            curQuest->addBool(boolName);
            saveInfo = saveInfo.substr(questSPStart, saveInfo.length() - questSPStart);
          } else {
            std::string boolName = saveInfo.substr(5, nextBoolStart - 5);
            curQuest->addBool(boolName);
            saveInfo = saveInfo.substr(nextBoolStart, saveInfo.length() - nextBoolStart);
          }
          boolStart = saveInfo.find("..+!B");
          questSPStart = saveInfo.find("..+!U");
        }
        int nextQuestStart = saveInfo.find("..-!Y");
        //Fix this one for garbage quests
        if(nextQuestStart == -1){
          int storyPointStart = saveInfo.find("..*!S");
          if(storyPointStart != -1){
            std::string storyPointName = saveInfo.substr(5, storyPointStart - 5);
            curQuest->setStoryPoint(std::stoi(storyPointName));
            saveInfo = saveInfo.substr(storyPointStart, saveInfo.length() - storyPointStart);
          } else {
            std::string storyPointName = saveInfo.substr(5, saveInfo.length() - 5);
            curQuest->setStoryPoint(std::stoi(storyPointName));
            saveInfo = "";
          }
        } else {
          std::string storyPointName = saveInfo.substr(5, nextQuestStart - 5);
          curQuest->setStoryPoint(std::stoi(storyPointName));
          saveInfo = saveInfo.substr(nextQuestStart, saveInfo.length() - nextQuestStart);
        }
        quests->addQuestGarbage(curQuest);
        nextGarbageQuest = saveInfo.find("..-!Y");
      }
      int storyPointStart = saveInfo.find("..*!S");
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

};

#endif
