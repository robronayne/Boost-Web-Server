#include <cstddef>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/beast.hpp>
#include <iostream>

#include "profile_manager.h"
#include "request_handler/authentication_handler.h"

/**
 * Skeleton code for authentication handler.
 *
 * Generate an authentication handler with location.
 */
authentication_handler::authentication_handler(std::string location, std::string request_url, std::string data_path,
                                               std::string root, std::string login, std::string logout, std::string signup,
                                               user_profile profile)
  : location_(location), request_url_(request_url), data_path_(data_path), root_(root), login_(login), logout_(logout),
    signup_(signup), profile_(profile) {

}

/**
 * Construct an HTTP structured reply for the received authentication request.
 */
http::status authentication_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  /**
   * Direct workflow to either login or logout depending upon the endpoint specified.
   *
   * If login is specified and the user is not logged in, return a static HTML webpage with
   * a login button. Upon clicking, redirect the user to the Google OAuth page. Otherwise,
   * if the user is already logged in, send back another static HTML with an HTTP response
   * indicating such.
   *
   * Finally, upon logout, return a HTTP reponse with 200 OK if the action was sucecssful,
   * and 400 Bad Request upon failure.
   *
   */
  std::string input = req.target().to_string();
  // check for login sub-handler
  if (location_ + login_ == input)
  {
    res.result(http::status::ok);
    std::string login_page;

    // read in login page
    utility.read_file(root_ + "/" + "login.html", login_page);
    beast::ostream(res.body()) << login_page;
    
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");
  }
  // check for login-submit sub-handler
  else if (location_ + login_ + "-submit" == input)
  {
    std::string data = boost::beast::buffers_to_string(req.body().data());
    
    // set cookie for our webserver
    profile_manager manager(data_path_ + "/");
    std::string new_cookie = manager.update_cookie(data);
    // check that cookie is created
    if (new_cookie != "")
    {
      res.set(http::field::set_cookie, "ngineers=" + new_cookie + "; Path=/");
      res.result(http::status::ok);
    }
    else
    {
      res.result(http::status::bad_request);
      beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
      res.content_length((res.body().size()));
      res.set(http::field::content_type, "text/html");
    }
    
  }
  // check for logout sub-handler
  else if (location_ + logout_ == input)
  {
    std::string data = boost::beast::buffers_to_string(req.body().data());
    // set cookie for our webserver
    res.set(http::field::set_cookie, "ngineers=; Path=/; Max-Age=0");
    res.result(http::status::ok);
  }
  // check for signup sub-handler
  else if (location_ + signup_ == input)
  {
    res.result(http::status::ok);
    std::string signup_page;

    // read in signup page
    utility.read_file(root_ + "/" + "signup.html", signup_page);
    beast::ostream(res.body()) << signup_page;
    
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");
  }
  // check for signup-submit sub-handler
  else if (location_ + signup_ + "-submit" == input)
  {
    std::string data = boost::beast::buffers_to_string(req.body().data());
    
    profile_manager manager(data_path_ + "/");
    // check for succeessful signup
    if(!manager.signup_string(data))
    {
      res.result(http::status::bad_request);
      beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
      res.content_length((res.body().size()));
      res.set(http::field::content_type, "text/html");
    }
    else
    {
      beast::ostream(res.body()) << "OK";
      res.content_length((res.body().size()));
      res.result(http::status::ok);
    }
  }
  // if no matching sub-handler found, return 404
  else
  {
    res.result(http::status::not_found);
    beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");

    log_message_info("404");
  }

   return res.result();
}

user_profile authentication_handler::get_profile()
{
   return profile_;
}

void authentication_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << request_url_
                          << " "
                          << "matched_handler: authentication handler";
}