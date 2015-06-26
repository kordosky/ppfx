
#include "CommonIMapIncludes.h"
#include <iostream>

int main(){
  using namespace IMap;
  for(int ivol=0; ivol<nvol; ivol++){
    std::cout<<ivol<<"  "<<volume[ivol]<<" : "<<materials[ivol]<<std::endl;
  }
  return 0;
}
