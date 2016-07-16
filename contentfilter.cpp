#include "contentfilter.h"
namespace contentfilter{
std::vector<std::string> phrase;
std::vector<unsigned int> weight;
std::vector<unsigned int> sizes ;
std::vector<std::string> weightedlist;
std::stringstream ss;



contentfilter::contentfilter()
{

}

void contentfilter::loadmystuff(std::vector<std::string> &ph,std::vector<unsigned int> &we,std::vector<unsigned int> &si)
{
    phrase = ph;
    weight = we;
    sizes = si;
}


/*bool contentfilter::verifyhtml(std::string &webcontent)
{
  boost::regex htmltagregex("<html\b[^>]*>(.*?)</html>");

  boost::sregex_iterator m1(webcontent.begin(), webcontent.end(),htmltagregex);
  boost::sregex_iterator m2;
  while (m1 != m2)
    {
      ss << *m1++;
    }

  ss >> content;


  if(boost::regex_match(webcontent, htmltagregex))
    {
      return true;
    }
  else
    {
      return false;
    }


}
*/
inline unsigned int contentfilter::counter(std::string& content,const std::string &lookingfor)
{

  //returns count of non-overlapping occurences of sub in str
  if(lookingfor.length() == 0) return 0;
  int count = 0;
  for(size_t offset = content.find(lookingfor);offset!=std::string::npos;
      offset = content.find(lookingfor,offset+lookingfor.length()))
    {
       count++;
    }
  return count;
}

bool contentfilter::loadallphrases(std::string &body)
{
    long totaloccurances = 0;
    unsigned int totalweigthperline = 0;
    int totalweightperphrase = 0;
    std::cout<<"Responded fine"<<std::endl;
  //unsigned int isbadweight = 0;
    std::cout<<phrase.size()<<std::endl;
    BOOST_FOREACH( std::string& i, phrase )
    {
      std::cout<<"Words are = "<< i <<std::endl;
    }
  BOOST_FOREACH( unsigned int& i, weight )
    {
      std::cout<<"Weight is = "<< i <<std::endl;
    }
  /*for(auto const& y : sizeperphraseclass)
    {
    std::cout<<y<<std::endl;
    }

    for (auto const& x : weightedphraselist)
    {
    std::cout << x.first  // string (key)
    << ':'
    << x.second // string's value
    << std::endl ;
    }
  */

  /*typedef std::map<std::string, std::map<std::string, std::string>>::iterator it_type;
    for(it_type iterator = m.begin(); iterator != m.end(); iterator++) {
    // iterator->first = key
    // iterator->second = value
    // Repeat if you also want to iterate through the second map.
    }
    for (auto const& x : symbolTable)
    {
    std::cout << x.first  // string (key)
    << ':'
    << x.second // string's value
    << std::endl ;
    }
  */

  unsigned int count = 0;
  unsigned int controlvar = 0;
  for(auto const &i : phrase) {
    totaloccurances = counter(body,i);
    std::cout<<"ocurrences were "<<totaloccurances<<std::endl;
    totalweigthperline +=  weight[count] * totaloccurances;
    std::cout<<"Weight was"<<totalweigthperline<<std::endl;
    if(totalweigthperline >   weight[count])
      {
        std::cout<<"Weight was"<<totalweightperphrase<<std::endl;
        return true;
      }
    count++;

    if(count == sizes[controlvar])
      {
        totalweigthperline = 0;
        controlvar++;
      }
  
  }
  return false;

}
}
