#include <iostream>
#include <fstream>
#include <string>

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

int main(int argc, char** argv){
  
  if(argc != 2){
    std::cout << "Usage: ./main [.txt save file to update]" << std::endl;
    exit(1);
  }
  std::ifstream loadFile;
  loadFile.open(argv[1]);
  if(!loadFile.is_open()){
    std::cout << "Error: .txt save file given is not valid." << std::endl;
    exit(1);
  }
  std::string fileLine;
  std::string originalFileInfo = "";
  while(std::getline(loadFile, fileLine)){
    originalFileInfo+= fileLine;
  }
  loadFile.close();
  originalFileInfo = encrypt(originalFileInfo);
  std::ofstream saveFile;
  saveFile.open(argv[1]);
  saveFile << originalFileInfo;
  saveFile.close();

  /*
  std::string testString = "Brendan";
  testString = encrypt(testString);
  std::cout << testString << std::endl;
  testString = unEncrypt(testString);
  std::cout << testString << std::endl;
  */
  return 0;
}
