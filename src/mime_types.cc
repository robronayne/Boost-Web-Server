#include "mime_types.h"

struct mapping
{
  const char* extension;
  const char* mime_type;
} mappings[] =
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

std::string extension_to_type(const std::string& extension)
{
  for (mapping m : mappings ) 
  {
    if (m.extension == extension) {
      return m.mime_type;
    }
  }

  return "text/plain";
}