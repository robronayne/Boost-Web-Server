#ifndef MAPPING_H
#define MAPPING_H

#include "http/mime_types.h"

// Intialize mapping for all implemented types.
mapping mappings[] =
{
  { "txt", "text/plain"},
  { "gif", "image/gif" },
  { "htm", "text/html" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "jpeg", "image/jpeg" },
  { "png", "image/png" },
  { "pdf", "application/pdf"},
  { "zip", "application/zip" }
};

/*
 * Return the MIME type associated with a given extension,
 * otherwise return the plain text typing.
 */
std::string extension_to_type(const std::string& extension)
{
  for (mapping map : mappings ) 
  {
    if (map.extension == extension) {
      return map.mime_type;
    }
  }

  // Default to plain text if unmatched.
  return "text/plain";
}

#endif