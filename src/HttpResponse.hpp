#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP




struct HttpResponse
{
  unsigned int status_code;
  std::string status_message;
  std::string http_version;
  std::map<std::string, std::string> header;
  std::string body;


  void clear()
  {
    status_code = 0;
    status_message = "";
    http_version = "";
    header.clear();
    body = "";
  }
};





std::ostream& operator<<(std::ostream& os, const HttpResponse& response)
{
  os << response.http_version << " " << response.status_code << " " << response.status_message << std::endl;

  for (std::map<std::string, std::string>::const_iterator i = response.header.begin(); i != response.header.end(); ++i)
  {
    os << i->first << ": " << i->second << std::endl;
  }

  os << response.body << std::endl;
  return os;
}




#endif//HTTP_RESPONSE_HPP
