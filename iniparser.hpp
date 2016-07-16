#ifndef INIPARSER_H
#define INIPARSER_H
#include "smartfilter.hpp"
#include "contentfilter.h"
//#include "tcpproxy_server.hpp"
#include <exception>
#include <string>
#include <vector>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>





namespace ini{



class iniparser
{
public:
	
	std::vector<std::string> category;
	std::vector<unsigned int> sizeperphraseclass;
	std::string localip;
	std::string listeningport;
	std::string forwardip;
	std::string forwardport;
    std::vector<std::string> phrases;
    std::vector<unsigned int> weights;
    filterclass::filter *p = new filterclass::filter();
    contentfilter::contentfilter *contentfilt = new contentfilter::contentfilter();
    iniparser();
    void readconfig();
};
}

#endif // INIPARSER_H
