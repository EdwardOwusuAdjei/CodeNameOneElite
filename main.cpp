#include <exception>
#include <string>
#include "iniparser.hpp"
#include "tcpproxy_server.hpp"



//argc and argv currently unused variables
//but will be use later to get commandline
//options
int main(int argc, char* argv[])
{
  ini::iniparser* config = new ini::iniparser();
  config->readconfig();
  const unsigned short local_port   = static_cast<unsigned short>(::atoi(config->listeningport.c_str()));
  const unsigned short forward_port = static_cast<unsigned short>(::atoi(config->forwardport.c_str()));
  const std::string local_host      = config->localip;
  const std::string forward_host    = config->forwardip;

  boost::asio::io_service ios;//boost io service which which does the talking to i/o of the operating system

  try
    {
      tcp_proxy::bridge::acceptor acceptor(ios,
                                           local_host, local_port,
                                           forward_host, forward_port);
 
      acceptor.accept_connections();

      ios.run();
    }
  catch(std::exception& e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }

}
