/**
 * request.h
 * ~~~~~~~~~
 *
 * Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <vector>

#include "http/header.h"

/**
 * A request structure containing the contents of the
 * original request as well as other necessary HTTP information
 * such as the URI and header.
 */
struct request
{
  std::string uri;
  std::string method;
  std::string original_req;
  std::vector<header> headers;
  
  int http_version_major;
  int http_version_minor;
};

#endif