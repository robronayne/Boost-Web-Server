/**
 * mime_types.h
 * ~~~~~~~~~~~~
 *
 * Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef MIME_TYPES_H
#define MIME_TYPES_H

#include <string>

/**
 * Define a mapping structure from extension to MIME type.
 */
struct mapping
{
  const char* extension;
  const char* mime_type;
};

/*
 * Convert a file extension into a MIME type.
 */
std::string extension_to_type(const std::string& extension);

#endif 