#include "request_handler/api_handler.h"
#include "entity.h"
#include <iostream>
#include <sstream>
#include <exception>
#include <vector>
#include <boost/beast/core/buffers_to_string.hpp>

// helper function to setup response in case of errors
void api_handler::fail(http::response<http::dynamic_body>& res, http::status fail_type)
{
  res.result(fail_type);
  beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/html");
}

/*
Idea credit: team the-burners
Code credit: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
Takes request uri and splits fields
Example, /api/Shoes/1 -> ["api", "Shoes", "1"]
*/
void split_request(std::string req_target, std::vector<std::string>& fields)
{
  
  // Start after the initial slash
  unsigned int start = 1U;
  std::size_t end = req_target.find("/", 1);

  // keep finding the next slash and appending the substrings
  while (end != std::string::npos)
  {
      fields.push_back(req_target.substr(start, end - start));
      start = end + 1;
      end = req_target.find("/", start);
  }

  // append the final substring
  fields.push_back(req_target.substr(start));
}

/*
Helper function to "prepare" id struct using parsed fields
Example, ["api", "Shoes", "1"]
will set id.id_ = 1 and id.type = /api/Shoes
Required for GET, PUT and DELETE requests
*/
bool prepare_id(ent_id& id, std::vector<std::string>& fields)
{

  /*
  we need at least 2 (ideally at least 3) fields
  The location (e.g. "api") and the entity type (e.g. "Shoes")
  And the id
  */
  if (fields.size() < 2) {
    return false;
  }

  // try to convert the last field into an integer
  try {
    id.id_ = std::stoi(fields.back(), nullptr);
  }

  /*
  an exception will be thrown if the conversion is unsuccessful
  we fail in that case
  */
  catch (std::exception& e) {
    return false;
  }

  // build the type string using all but the last field
  for (int i = 0; i < fields.size() - 1; i++)
  {
    id.type += "/" + fields[i];
  }

  return true;
}

api_handler::api_handler(std::string location, std::string root_file_path, std::string request_url)
 : location_(location),
   root_(root_file_path),
   request_url_(request_url),
   e_manager(root_file_path) {}

http::status api_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  http::verb req_method = req.method();
  std::string req_target = req.target().to_string();
  std::vector<std::string> uri_fields;
  split_request(req_target, uri_fields);

  switch(req_method) {

    case http::verb::get:
    {
      // if it's a request for a list of ID's return that
      if (e_manager.type_exists(req_target)) {
        std::string reply_body = e_manager.get_ids_by_type(req_target);
        beast::ostream(res.body()) << reply_body;
        res.result(http::status::ok);
        res.content_length(res.body().size());
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
        break;
      }

      // otherwise we must retrieve the requested entity (if it exists)

      // checking request validity  
      ent_id id;
      if (!prepare_id(id, uri_fields)) {
        fail(res, http::status::bad_request);
        break;
      }

      // get entity data and populate response
      const entity* ent = e_manager.get(id);
      if (ent != nullptr) {
        res.result(http::status::ok);
        std::string reply_body = ent->get_data();
        beast::ostream(res.body()) << reply_body;
        res.content_length(res.body().size());
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
      }
      else {
        fail(res, http::status::bad_request);
      }

    break;

    }

    case http::verb::put:
    {
      // check request validity
      ent_id id;
      if (!prepare_id(id, uri_fields)) {
        fail(res, http::status::bad_request);
        break;
      }

      std::string new_data = boost::beast::buffers_to_string(req.body().data());

      // attempt to update entity file
      mu.lock();
      bool updated = e_manager.update(id, new_data);
      mu.unlock();

      if (updated) {
        res.result(http::status::ok);
        std::string reply_body = req_target + " updated successfully.";
        beast::ostream(res.body()) << reply_body;
        res.content_length(res.body().size());
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
      }
      else {
        entity new_entity;

        size_t type_start = req_target.find("/", 1);
        std::string type_;
        if (type_start == std::string::npos) {
          fail(res, http::status::bad_request);
          break;
        }

        type_ = req_target.substr(type_start + 1);
        new_entity.data = boost::beast::buffers_to_string(req.body().data());
        new_entity.path = root_ + "/" + type_ + "/";
        new_entity.type = req_target;

        mu.lock();
        bool inserted = e_manager.insert_with_id(new_entity);
        mu.unlock();

        if (inserted) {
          res.result(http::status::ok);
          std::string reply_body = "{\"id\": " + std::to_string(new_entity.id) + "}";
          beast::ostream(res.body()) << reply_body;
          res.content_length(res.body().size());
          res.set(http::field::content_type, "application/json");
          res.keep_alive(req.keep_alive());
        } else {
          fail(res, http::status::bad_request);
        }
      }
    break;
    }

    case http::verb::delete_:
    {
      // check request validity
      ent_id id;
      if (!prepare_id(id, uri_fields)) {
        fail(res, http::status::bad_request);
        break;
      }

      // attempt to delete entity and entity file
      mu.lock();
      bool removed = e_manager.remove(id);
      mu.unlock();

      if (removed) {
        res.result(http::status::ok);
        std::string reply_body = req_target + " deleted successfully.";
        beast::ostream(res.body()) << reply_body;
        res.content_length(res.body().size());
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
      }
      else {
        fail(res, http::status::bad_request);
      }
    
    break;
    }

    case http::verb::post:
    {
      // object for the new entity to be inserted
      entity new_entity;

      /*
      here we will need a different definition of type because we must create a file
      over here, for example if we have POST /api/Shoes, then type is Shoes
      so that the path can be root_/Shoes/ID
      */
      size_t type_start = req_target.find("/", 1);
      std::string type_;
      if (type_start == std::string::npos) {
        fail(res, http::status::bad_request);
        break;
      }

      type_ = req_target.substr(type_start + 1);
      new_entity.data = boost::beast::buffers_to_string(req.body().data());
      new_entity.path = root_ + "/" + type_ + "/"; // id will be added later
      new_entity.type = req_target;

      // attempt to insert the newly created entity
      mu.lock();
      bool inserted = e_manager.insert(new_entity);
      mu.unlock();
      
      if (inserted) {
        res.result(http::status::ok);
        std::string reply_body = "{\"id\": " + std::to_string(new_entity.id) + "}";
        beast::ostream(res.body()) << reply_body;
        res.content_length(res.body().size());
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req.keep_alive());
      }
      else {
        fail(res, http::status::bad_request);
      }
    break;
    }
    default:
      fail(res, http::status::bad_request);
    break;
  }

  log_message_info(res.result(), req_target);

  return res.result();
}

void api_handler::log_message_info(http::status code, std::string req_path)
{
  int res_code;
  switch(code)
  {
    case http::status::ok :
      res_code = 200;
      break;
    case http::status::bad_request :
      res_code = 400;
      break;
  }
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << req_path
                          << " "
                          << "matched_handler: api handler";
}