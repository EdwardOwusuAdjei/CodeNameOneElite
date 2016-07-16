#include "iniparser.hpp"

using namespace ini;

iniparser::iniparser()
{

}
//the method to help parse both blocked
//phrases files and config files

void iniparser::readconfig()
 {
    unsigned int count  = 0;
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("phrases.ini", pt);//parse phrase.ini
    p->loadfile();//This reads a list of blocked hosts and loads it.
    
    //iterator to find phrases and their weight into a vector
    //i.e. weight vector and phrase vector
    //each will tracked to a category
    //NB:used vector instead of maps..
    delete p;
for (auto& section : pt)
  {
    category.push_back(section.first);
    for (auto& key : section.second){

      phrases.push_back(key.first);
      weights.push_back(key.second.get_value<unsigned int>());


      count++;
    }
    sizeperphraseclass.push_back(count);
    count = 0;
  }
//Debugging purpose..display size of each category
for(auto const& y : sizeperphraseclass)
  {
    std::cout<<y<<std::endl;
  }

//then pass on vector objects to contentfilter class
contentfilt->loadmystuff(phrases,weights,sizeperphraseclass);

std::cout<<"Hello im here"<<std::endl;
/*  for (auto const& x : weightedphraselist)
    {
    std::cout << x.first  // string (key)
    << ':'
      << x.second // string's value
    << std::endl ;
    }
*/


//read config.ini file which contains listening port and forwarding port and etc.

boost::property_tree::ini_parser::read_ini("config.ini", pt);
//std::cout << pt.get<std::string>("AsProxy.Bool") << std::endl;

//changes will be made but works fine now
try{
  std::string BoolForProxy = pt.get<std::string>("AsProxy.Bool") ;
  if(BoolForProxy == "True")
    {
      localip = pt.get<std::string>("AsProxy.localip");
      listeningport = pt.get<std::string>("AsProxy.listeningport");
      forwardip = pt.get<std::string>("AsProxy.forwardip");
      forwardport = pt.get<std::string>("AsProxy.forwardport");
    }
  else
    {
      std::cerr<<"Not configured properly..";
    }
  //std::cout << pt.get<std::string>("AsProxy.ConnectTo") << std::endl;
  /* if (argc != 5)
     {
     std::cerr << "usage: tcpproxy_server " << std::endl;
     return 1;
     }
  */
}
catch (const std::exception& e) {
  std::cerr << "Exception caught: " << e.what() << '\n';
//  return 1;
    }
 }
