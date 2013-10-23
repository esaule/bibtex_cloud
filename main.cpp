#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "parse.hpp"

#include "util.hpp"

int main (int argc, char* argv[]) {
  if (argc!=2) {
    std::cerr<<"Usage :"<<argv[0]<<" <filename>"<<std::endl;
    return -1;
  }

  std::map<std::string, std::vector<std::string> > key_to_keywords;

  extract_key_keywords(argv[1], key_to_keywords);

  //put everything to lower case
  for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
    for (std::string &s : it->second)
      s = str_tolower(s);
  }

  

  //print
  for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
    std::cout<<it->first<<"=>"; 
    for (std::string &s : it->second)
      std::cout<<s<<"|";
    std::cout<<std::endl;
  }

  //count occurences
  std::map<std::string, int> count_m;
  
  for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
    for (std::string s : it->second) {
      count_m[s] ++;
    }
  }

  std::vector<std::pair<std::string, int > > count;
  //print occurences
  for (auto it = count_m.begin(); it != count_m.end(); ++it) {
    count.push_back(std::pair<std::string, int > (it->first, it->second));
  }
  
  std::sort (count.begin(), count.end(), [] (const std::pair<std::string, int> & a, const std::pair<std::string, int> & b) -> bool {return a.second>b.second;} );

  //print occurences
  for (auto it = count.begin(); it != count.end(); ++it) {
    std::cout<<it->first<<" "<<it->second<<std::endl;
  }

  

  return 0;
}
