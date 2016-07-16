#ifndef CONTENTFILTER_H
#define CONTENTFILTER_H

#include <string>
#include <vector>
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/foreach.hpp>
#include <map>
#include <iostream>
namespace contentfilter{

class contentfilter
{
 public:
  contentfilter();
  void loadmystuff(std::vector<std::string> &ph,std::vector<unsigned int> &we,std::vector<unsigned int> &si);
  inline unsigned int counter(std::string& content,const std::string &lookingfor);
  bool loadallphrases(std::string &body);
  //bool loadallphrases(unsigned int isbadweight,std::vector<std::string> file);
 // bool verifyhtml(std::string &webcontent);
  // int checkweight(std::string &body);
  //void loadfilter(std::vector<std::string> &phrase,std::vector<int> &weight,std::vector<int> &sizes);


};
}

#endif // CONTENTFILTER_H
