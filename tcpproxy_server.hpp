#ifndef TCPPROXY_SERVER_H
#define TCPPROXY_SERVER_H

#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "http_parser.h"

namespace tcp_proxy
{
  
  namespace ip = boost::asio::ip;
  class bridge : public boost::enable_shared_from_this<bridge>
  {
  public:
	 http_parser_settings setting;
	 http_parser *parse = new http_parser;
	
    typedef ip::tcp::socket socket_type;
    typedef boost::shared_ptr<bridge> ptr_type;
    
    bridge(boost::asio::io_service& ios);
    socket_type& downstream_socket();
    socket_type& upstream_socket();
    //specifies where the packets should be reached and connected
    void start(const std::string& upstream_host, unsigned short upstream_port);
	
    //here is where a connection is made to upstream server
    //data is then read from socket with async_read_some when there is data
    //and then callback method is called
    void handle_upstream_connect(const boost::system::error_code& error);

  private:
    void handle_downstream_write(const boost::system::error_code& error);
    
    //callback method when data is read from downstream socket/thats
    //data from client can be intercepted here or logged
    void handle_downstream_read(const boost::system::error_code& error,
				const size_t& bytes_transferred);
   // static int icame(http_parser* read, const char *at, size_t length);
    void handle_upstream_write(const boost::system::error_code& error);
    
    //callback method when data is read from the upstream socket
    void handle_upstream_read(const boost::system::error_code& error,
			      const size_t& bytes_transferred);

    
    //handles closing of threads if opened..code reaches here when theres a system call error
    void close();
    
    //custom method,only called when client accessed a blocked site..it returns a blocked page
    //changes to be made..i dont like the idea that the html file will be read always..
    //i hate disk reads...
    void customclose(const boost::system::error_code& error,const size_t& bytes_transferred );
    
    //method to return posix date time
    std::string format_datetime(boost::posix_time::ptime pt);
	  
    //both tcp sockets
    socket_type downstream_socket_;
    socket_type upstream_socket_;
    //8KB for byte size for transferred data
    enum { max_data_length = 8192 };
    unsigned char downstream_data_[max_data_length];
    unsigned char upstream_data_[max_data_length];
    std::ofstream downstream_log;
    std::ofstream upstream_log;
     
    boost::mutex mutex_;
     
  public:
    //a nested class, members here are basically totally linked with the outer class
    class acceptor
    {
    public:
      //constructor which takes the obvious arguments..takes the io service and endpoint
		//but here it takes two endpoints
		acceptor(boost::asio::io_service& io_service,
			   const std::string& local_host, unsigned short local_port,
				 const std::string& upstream_host, unsigned short upstream_port);
	
		//method which creates a new bridge for an accepted client and gives it a shared pointer(we r safe mehn)
		bool accept_connections();
		
    private:
      //callback for for async_accept.. start a connection
      void handle_accept(const boost::system::error_code& error);
      

      boost::asio::io_service& io_service_; //reference to the io service started from main.cpp..
      ip::address_v4 localhost_address;//ipv4
      ip::tcp::acceptor acceptor_;//boost::asio::ip::tcp::acceptor object for
      ptr_type session_; // typedef boost::shared_ptr<bridge> ptr_type
      unsigned short upstream_port_; //upstream port e.g. what squid listening on
      std::string upstream_host_; //upstream host e.g.ip for squid
    };
  };
}
#endif
