#ifndef SMARTFILTER_H
#define SMARTFILTER_H
/*
 *Development of a smart algorithm
 *if blocked site due to content or
 *bad url exist, Block.. if not in the list..
 *send the learner to visit unknown site
 *and parse html for bad keywords
 */


/*
 *Implementation needs a vector
 *Reason: Quick lookup,Slow insertion
 *and deletion..we dont mind slow
 *insertion and deletion
 */


#include <future>

#include <boost/foreach.hpp>
#include <sstream>
#include <string>
#include <exception>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
//object of this class should be made once
//due to "read from disk once" <-- comment need checking
namespace filterclass
{
/*read in all url into vector in ram for quick lookup*/
  class filter
  {
  public:
    int loadfile();
    int fastlookup(std::string& httpheader);

  private:
    static void writebad(std::string& unknown);
  };
}
#endif // SMARTFILTER_H
