#ifndef API_HANDLER_FACTORY_H
#define API_HANDLER_FACTORY_H

#include "request_handler_factory.h"
#include "http/path.h"

/*

API Handler factory class

*/

class api_handler_factory : public request_handler_factory
{
	public:
	  api_handler_factory(std::string location, path root_file_path);
	  request_handler_interface* create(std::string location, std::string request_url);
	
	private:

	  std::string location_;
	  path root_file_path_;
};

#endif