#ifndef Quest_HH_
#define Quest_HH_

#include <string>

class Quest{
private:
std::string title;

public:
  Quest(std::string name){
    title = name;
  }

  std::string getTitle(){
    return title;
  }

  void setTitle(std::string name){
    title = name;
  }

};

#endif
