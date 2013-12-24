

#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <string>



#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "HttpGetter.hpp"


using namespace std;
using boost::property_tree::ptree;




int main(int argc, char* argv[])
{


  // key is url, value is set of urls contained by key url
  map< string, set<string> > forward_links;

  // key is url, value is set of urls containing key url
  map< string, set<string> > reverse_links;

  try
  {
    if (argc != 3)
    {
      std::cout << "Usage: async_client <server> <path>\n";
      std::cout << "Example:\n";
      std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
      return 1;
    }


    boost::asio::io_service io_service;
    HttpGetter c(io_service, argv[1], argv[2]);
    io_service.run();

    ptree pt;

    istringstream json_stream(c.result());
    read_json(json_stream, pt);

    pt = pt.get_child("query");
    pt = pt.get_child("pages");

    for (ptree::iterator i = pt.begin(); i != pt.end(); ++i)
    {
      ptree links =  i->second.get_child("links");

      for (ptree::iterator l = links.begin(); l != links.end(); ++l)
      {
        cout << l->second.get_child("title").get_value<string>() << endl;
      }
    }


  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
