#include "smartfilter.hpp"

namespace filterclass
{
  unsigned int hit = 1; //return value to note if domain was found in vector
  unsigned int miss = 0; //return value to note domain wasnt found in vector

  //namespace search = boost::algorithm;
  std::string eachurl;
  std::string allurls;
  std::vector<std::string> badsites;
  int i = 0;
  std::ifstream file;


  //load file method..reads blocked domains "domains" from disk into vector
  //currently in the program directory but will later make it read from
  //a specified directory..make changes to iniparser's config reader for that
  //should read once..hence object made from this class should be reused through
  //entire program
  int filter::loadfile()
  {

    file.open("domains",std::ios::in | std::ios::binary);
    while(std::getline(file,eachurl))
      {
        badsites.push_back(eachurl);
      }
    file.close();
    return 1;
  }
    
  int filter::fastlookup(std::string& httpheader)
  {
    //will choose a different faster algorithm rather than this since i dont
    //like the work of the boyer moore alg. will like to choose BOOST foreach on
    //the vector and compare. 
    /*if(search::boyer_moore_search(allurls.begin(), allurls.end(),httpheader.begin() ,httpheader.end()) != allurls.end() )
      {
        
      }*/
    BOOST_FOREACH(auto &str , badsites)
      {
        if(httpheader == str)
          {
            return hit;//return found blocked host
          }
      }
//    std::async(writebad,httpheader);
    return miss;//return didnt find after an async call to writebad method
  }

  //a private method to write unknown visited hosts to file
  void filter::writebad(std::string& unknown)
  {
    std::ofstream writer;
    writer.open("write",std::ios::out | std::ios::app);
    writer<< unknown+"\n";
   // writer.close();
  }
}
