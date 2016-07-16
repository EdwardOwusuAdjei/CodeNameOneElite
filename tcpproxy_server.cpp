#include "tcpproxy_server.hpp"
#include "smartfilter.hpp"
#include "contentfilter.h"
#include <cstdlib>
#include <cstddef>



#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/asio/basic_streambuf.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/date_time/posix_time/posix_time_io.hpp>




using namespace boost;

namespace tcp_proxy
{
  
  namespace ip = boost::asio::ip;

  typedef ip::tcp::socket socket_type;
  typedef boost::shared_ptr<bridge> ptr_type;

  bridge::bridge(boost::asio::io_service& ios)
    : downstream_socket_(ios),
      upstream_socket_(ios)
  {}

  socket_type& bridge::downstream_socket()
  {
    return downstream_socket_;
  }

  socket_type& bridge::upstream_socket()
  {
    return upstream_socket_;
  }

  void bridge::start(const std::string& upstream_host, unsigned short upstream_port)
  {
    upstream_socket_.async_connect(
      ip::tcp::endpoint(
        boost::asio::ip::address::from_string(upstream_host),
        upstream_port),
      boost::bind(&bridge::handle_upstream_connect,
                  shared_from_this(),
                  boost::asio::placeholders::error));

  }

  void bridge::handle_upstream_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      
     
      
      // settings.on_chunk_complete = ;
     
      //std::string datetime = format_datetime(boost::posix_time::microsec_clock::universal_time());
      //file name = address of client connecting to us and address of client receiving data
//        std::string upstream_file_name   = "us_" + upstream_socket_  .remote_endpoint().address().to_string()+ "_" + datetime + ".log";
      //      std::string downstream_file_name = "ds_" + downstream_socket_.remote_endpoint().address().to_string()+ "_" + datetime + ".log";
      //opens/create a file and associates it with the stream,wrting of data into such files will be done
      //in callback functions
      //upstream_log.open(upstream_file_name.c_str(),std::ios::binary);
      //downstream_log.open(downstream_file_name.c_str(),std::ios::binary); 

      upstream_socket_.async_read_some(
        boost::asio::buffer(upstream_data_,max_data_length),
        boost::bind(&bridge::handle_upstream_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

      downstream_socket_.async_read_some(
        boost::asio::buffer(downstream_data_,max_data_length),
        boost::bind(&bridge::handle_downstream_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

       
    }
    else
      close();//close function calls the mutex from boost to close an open thread
     
  } 

  void bridge::handle_downstream_write(const boost::system::error_code& error)
  {
    if (!error)
    {
      upstream_socket_.async_read_some(
        boost::asio::buffer(upstream_data_,max_data_length),
        boost::bind(&bridge::handle_upstream_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
    else
      close();
  }

  void bridge::handle_downstream_read(const boost::system::error_code& error,
                                      const size_t& bytes_transferred)
  {


    if(!error)
    {
        filterclass::filter *p = new filterclass::filter();
      //if (downstream_log)
      // downstream_log.write(reinterpret_cast<char*>(&downstream_data_[0]),static_cast<std::streamsize>(bytes_transferred));
        
      std::string parsedreq;
      std::string ddd = boost::lexical_cast<std::string>(&downstream_data_[0]);
      std::istringstream header ;
      header.str(ddd);
      // int i = 0;
      while (std::getline(header,parsedreq))
      {
        const int retval = 1;
        if (parsedreq.find("Host:") != std::string::npos) {
          trim_left_if(parsedreq,is_any_of("Host:"));
          trim(parsedreq);
          int b = p->fastlookup(parsedreq);
          if(retval == b)
          {
            delete p;
            customclose(error,bytes_transferred);
          }
          else
          {
            break;
             
          }
        }
      }
       delete p;
      //writes data to upstream socket now
      async_write(upstream_socket_,
                  boost::asio::buffer(downstream_data_,bytes_transferred),
                  boost::bind(&bridge::handle_upstream_write,
                              shared_from_this(),
                              boost::asio::placeholders::error));

       
       
    }
      
    else{
      close();
    }
  }

  static int icame(http_parser *read, const char *at, size_t length){
      std::cout<<length<<std::endl;
      std::cout<<at<<std::endl;
      //htmlbody.clear();
    return 1;  
  }
  // static int chunckcallback(http_parser *read){
  //   std::cout<<"Chunk completed";
  //   return 1;
  // }
  
  void bridge::handle_upstream_write(const boost::system::error_code& error)
  {
    if (!error)
    {
       

      downstream_socket_.async_read_some(
        boost::asio::buffer(downstream_data_,max_data_length),
        boost::bind(&bridge::handle_downstream_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
      
    
    }
    else
      close();
  }

  void bridge::handle_upstream_read(const boost::system::error_code& error,
                                    const size_t& bytes_transferred)
  {

    if (!error)
    {
     contentfilter::contentfilter *contentfilt = new contentfilter::contentfilter();

      std::string hh = boost::lexical_cast<std::string>(&upstream_data_[0]);
      //memset(&setting, 0, sizeof(setting));
      //setting.on_body = icame;
      //setting.on_chunk_complete = chunckcallback; 
      //http_parser_init(parse,HTTP_RESPONSE);
      //http_parser_execute(parse,&setting,hh.c_str(),strlen(hh.c_str()));
      
      // smartfilter sf = new smartfilter();
      //sf.
      std::cout<<&upstream_data_[0]<<std::endl;
      /* if (hh.find("W3Schools") != std::string::npos) {
        //  close();
        customclose(error,bytes_transferred);
      }
      */
      //this line above worked in blocking w3schools..ineffectively
      bool passedweight = false;
      const unsigned int typevar = 2;
      passedweight = contentfilt->loadallphrases(hh);
      if(passedweight)
      {
          std::cout<<"I caught a bastard"<<std::endl;
          delete contentfilt;
          customclose(error,typevar);
      }
      else
          delete contentfilt;
          async_write(downstream_socket_,
                  boost::asio::buffer(upstream_data_,bytes_transferred),
                  boost::bind(&bridge::handle_downstream_write,
                              shared_from_this(),
                              boost::asio::placeholders::error));

    }
    else
      close();
  }

  void bridge::close()
  {
    boost::mutex::scoped_lock lock(mutex_);//unused
      
    if (downstream_socket_.is_open())
    {
      downstream_socket_.close();
    }
     
    if (upstream_socket_.is_open())
    {
      upstream_socket_.close();
    }
  }

  void bridge::customclose(const boost::system::error_code& error,const size_t& type )
  {

    if(!error)
    {

      std::string full_path = "blocked.html";
           
      std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
      if (!is)
      {
        //handle error

      };

      std::string content((std::istreambuf_iterator<char>(is)),
                          std::istreambuf_iterator<char>());

      boost::asio::streambuf b;
      std::ostream redirectresponse(&b);
      //1 means early close and 2 means phrase close
      if(type == 1)
      {
      redirectresponse << "HTTP/1.1 200 OK\r\n Content-Length: " << content.length() << "\r\n\r\n" << content;
      }
      else
      {
      redirectresponse <<"\r\n\r\n"<< content;
      }
      //write to socket of client who has been blocked with clocked page
      async_write(downstream_socket_,
                  boost::asio::buffer(b.data(),content.size()),
                  boost::bind(&bridge::handle_downstream_write,
                              shared_from_this(),
                              boost::asio::placeholders::error));
          
           
      //uncomment this line to redirect to squid but properly change the redirect
      //response or else squid might try and access http://200 OK #bug but could change clients page ;)
      //or still use a proper redirect http code
      /*
        async_write(downstream_socket_,
        boost::asio::buffer(b.data(),max_data_length),
        boost::bind(&bridge::handle_downstream_write,
        shared_from_this(),
        boost::asio::placeholders::error));
      */

      
      boost::mutex::scoped_lock lock(mutex_);
         
      if (downstream_socket_.is_open())
      {
        downstream_socket_.close();
      }
         
      if (upstream_socket_.is_open())
      {
        upstream_socket_.close();
      }
         
    }
  }

  /* std::string bridge::format_datetime(boost::posix_time::ptime pt)
     {
     std::string s;
     std::ostringstream datetime_ss;
     boost::posix_time::time_facet * p_time_output = new boost::posix_time::time_facet;
     std::locale special_locale (std::locale(""), p_time_output);
     datetime_ss.imbue (special_locale);
     (*p_time_output).format("%Y%m%d%H%M%S.%f");
     datetime_ss << pt;
     s = datetime_ss.str().c_str();

     return s;
     }
  */
  bridge::acceptor::acceptor(boost::asio::io_service& io_service,
                             const std::string& local_host, unsigned short local_port,
                             const std::string& upstream_host, unsigned short upstream_port)
    : io_service_(io_service),
      localhost_address(boost::asio::ip::address_v4::from_string(local_host)),
      acceptor_(io_service_,ip::tcp::endpoint(localhost_address,local_port)),
      upstream_port_(upstream_port),
      upstream_host_(upstream_host)
  {}

    
  bool bridge::acceptor::accept_connections()
  {
    
    try
    {
      session_ = boost::shared_ptr<bridge>(new bridge(io_service_));
      
      acceptor_.async_accept(session_->downstream_socket(),
                             boost::bind(&acceptor::handle_accept,
                                         this,
                                         boost::asio::placeholders::error));
    }
    catch(std::exception& e)
    {
      std::cerr << "acceptor exception: " << e.what() << std::endl;
      return false;
    }

    return true;
  }

  void bridge::acceptor::handle_accept(const boost::system::error_code& error)
  {
    if (!error)
    {
     
              
      session_->start(upstream_host_,upstream_port_);
        
      if (!accept_connections())
      {
        std::cerr << "Failure during call to accept." << std::endl;
      }
    }
    else
    {
      std::cerr << "Error: " << error.message() << std::endl;
    }
  }
}

