#include "request_handler_factory/api_handler_factory.h"
#include "request_handler/api_handler.h"

api_handler_factory::api_handler_factory(std::string location, path root_file_path, entity_manager* entity_manager)
  : location_(location), root_file_path_(root_file_path), e_manager(entity_manager) {}

request_handler_interface* api_handler_factory::create(std::string location, std::string request_url)
{
	return new api_handler(location, root_file_path_.info_map["data_path"], request_url, e_manager);
}