

#include <iostream>
#include <map>
#include <set>
#include <string>

#include <boost/asio.hpp>

using namespace std;





int main(int argc, char* argv[])
{
  cout << __FILE__ << endl;

  // key is url, value is set of urls contained by key url
  map< string, set<string> > forward_links;

  // key is url, value is set of urls containing key url
  map< string, set<string> > reverse_links;



  for (int i = 0; i < argc; ++i)
  {
    cout << argv[i] << endl;
  }



  return 0;
}
