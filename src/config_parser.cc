/*
 * An NGINX config file parser.
 *
 * See:
 *   http://wiki.nginx.org/Configuration
 *   http://blog.martinfjordvald.com/2010/07/nginx-primer/
 *
 * How Nginx does it:
 *   http://lxr.nginx.org/source/src/core/ngx_conf_file.c
 */

#include <stack>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>

#include "http/path.h"
#include "config_parser.h"

/**
 * Return string representation of NGINX configuration.
 */
std::string NginxConfig::ToString(int depth) {
  std::string serialized_config;
  for (const auto& statement : statements_) {
    serialized_config.append(statement->ToString(depth));
  }
  return serialized_config;
}

/**
 * Gets the port number from a configuration file
 */
int NginxConfig::getPortNum()
{
  int ret;
  for (auto statement : statements_)
  {
    // If the statement is not a child block.
    if (statement->child_block_.get() == nullptr)
    {
      if (statement->tokens_.size() == 2 && statement->tokens_[0] == "port")
      {
        ret = stoi(statement->tokens_[1]);
        
        // Acceptable range for port numbers is 0 <= ret <= 65535 (0xffff).
        if (ret >= 0 && ret <= 0xffff)
          return ret;
        else
          return -1;
      }
    }
    // Otherwise, if the statement is a child block (examine it recursively).
    else
    {
      ret = statement -> child_block_ -> getPortNum();
      if (ret != -1)
        return ret;
    }
  }
  return -1;
}

/**
 * Return the URL serving path without trailing slashes.
 * Will return an "ERROR" if for some another reason the URL is
 * improper. Improper URL's will be handled by the error handler. 
 */
std::string NginxConfig::formatURL(std::string url)
{
  // Check that the URL begins with a valid character.
  if (url.front() == '/')
  {
    // Ensure that the length is at least two characters before 
    // recursively truncating the trailing slashes.
    if (url.length() > 1 && url.back() == '/')
    {
      // Recursivley format the URL until trailing spaces are removed.
      return formatURL(url.substr(0, url.size()-1));
    }
    else 
    {
      // Return a single slash URL.
      return url;
    }
  }
  else
  {
    // ERROR for invalid URLs.
    BOOST_LOG_TRIVIAL(error) << "Configuration error:\n"
                             << url
                             << " is not a valid endpoint.";
    return "ERROR";
  }
}

/**
 * Determine local path from configuration file.
 */
std::vector<path> NginxConfig::getPaths()
{
  for (const std::shared_ptr<NginxConfigStatement> statement : statements_)
  {
    // Find "locate" statements containing 3 tokens.
    if (statement->tokens_[0] == "location")
    {
      // If the statement is directed towards an EchoHandler
      // with a child block.
      if (statement->tokens_[2] == "EchoHandler" &&
          statement->child_block_.get() != nullptr &&
          statement->tokens_.size() == 3)
      {
        path current_path;
        current_path.type = echo;
        current_path.endpoint = formatURL(statement->tokens_[1]);
        paths.push_back(current_path);
      }
      // If the statement is directed towards an StaticHandler
      // with a child block.
      else if (statement->tokens_[2] == "StaticHandler" &&
               statement->child_block_.get() != nullptr &&
               statement->tokens_.size() == 3)
      {
        for (const std::shared_ptr<NginxConfigStatement> child_statement : statement->child_block_->statements_)
        {
          // Locate "root" statements with corresponding path
          if (child_statement->tokens_.size() == 2)
          {
            path current_path;
            current_path.type = static_;
            current_path.endpoint = formatURL(statement->tokens_[1]);
            current_path.info_map[child_statement->tokens_[0]] = child_statement->tokens_[1];
            paths.push_back(current_path);
          }
        }
      }
      /**
       * This is catch-all else statement for invalid location statements. 
       *
       * - Look for any statments beginning with a "location" token.
       * - Now determine if one of the following conditions holds:
       *    1. The the number of tokens in the statement is not equal to 3
       *    2. The third token is neither "EchoHandler" nor "StaticHandler"
       *    3. The statement does not have a child block
       *
       * If any of these above three statements hold for a "location" statement,
       * then we know that the specified configuration is invalid.
       */
      else
      {
        std::string print_statement = "";
    
        for (std::string current_token : statement->tokens_)
        {
          print_statement += (current_token + " ");
        }

    	// Log error for invalid path
    	BOOST_LOG_TRIVIAL(error) << "Configuration error:\n"
          			 << print_statement
          			 << "is not a valid path.";
      }
    }
  }

  // Determine whether or not the base path has already
  // been initialized.
  bool base_present = false;
  for (path current_path : paths)
  {
    if (current_path.endpoint == "/")
    {
      base_present = true;
    }
  }

  // If it hasn't, create it and append it to paths.
  if (base_present == false)
  {
    path base_path;
    base_path.endpoint = "/";
    base_path.type = not_found;
    paths.push_back(base_path);
  }

  return paths;
}

/**
 * Return string represenation of a configuration to a depth.
 */
std::string NginxConfigStatement::ToString(int depth) {
  std::string serialized_statement;
  for (int i = 0; i < depth; ++i) {
    serialized_statement.append("  ");
  }
  for (unsigned int i = 0; i < tokens_.size(); ++i) {
    if (i != 0) {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }
  if (child_block_.get() != nullptr) {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->ToString(depth + 1));
    for (int i = 0; i < depth; ++i) {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  } else {
    serialized_statement.append(";");
  }
  serialized_statement.append("\n");
  return serialized_statement;
}

/**
 * Return the string associated with a given token type.
 */
const char* NginxConfigParser::TokenTypeAsString(TokenType type) 
{
  switch (type) 
  {
    case TOKEN_TYPE_START:         return "TOKEN_TYPE_START";
    case TOKEN_TYPE_NORMAL:        return "TOKEN_TYPE_NORMAL";
    case TOKEN_TYPE_START_BLOCK:   return "TOKEN_TYPE_START_BLOCK";
    case TOKEN_TYPE_END_BLOCK:     return "TOKEN_TYPE_END_BLOCK";
    case TOKEN_TYPE_COMMENT:       return "TOKEN_TYPE_COMMENT";
    case TOKEN_TYPE_STATEMENT_END: return "TOKEN_TYPE_STATEMENT_END";
    case TOKEN_TYPE_EOF:           return "TOKEN_TYPE_EOF";
    case TOKEN_TYPE_ERROR:         return "TOKEN_TYPE_ERROR";
    default:                       return "Unknown token type";
  }
}

/**
 * Parse individual tokens from input stream.
 */
NginxConfigParser::TokenType NginxConfigParser::ParseToken(std::istream* input, std::string* value) 
{
  TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
  TokenParserState prev_state = TOKEN_STATE_INITIAL_WHITESPACE;
  while (input->good()) {
    const char c = input->get();
    if (!input->good()) {
      break;
    }
    switch (state) {
      case TOKEN_STATE_INITIAL_WHITESPACE:
        switch (c) {
          case '{':
            *value = c;
            return TOKEN_TYPE_START_BLOCK;
          case '}':
            *value = c;
            return TOKEN_TYPE_END_BLOCK;
          case '#':
            *value = c;
            state = TOKEN_STATE_TOKEN_TYPE_COMMENT;
            continue;
          case '"':
            *value = c;
            state = TOKEN_STATE_DOUBLE_QUOTE;
            continue;
          case '\'':
            *value = c;
            state = TOKEN_STATE_SINGLE_QUOTE;
            continue;
          case ';':
            *value = c;
            return TOKEN_TYPE_STATEMENT_END;
          case ' ':
          case '\t':
          case '\n':
          case '\r':
            continue;
          default:
            *value += c;
            state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
            continue;
        }
      case TOKEN_STATE_SINGLE_QUOTE:
        *value += c;
        if (c == '\'') {
          // The end of a quoted token should be followed by whitespace.
          const char next_c = input->get();
          if (next_c == ' ' || next_c == '\t' || next_c == '\n' || next_c == '\t' ||
              next_c == ';' || next_c == '{' || next_c == '}') {
            input->unget();
          } else {
            return TOKEN_TYPE_ERROR;
          }
          return TOKEN_TYPE_NORMAL;
        }
        // Allow for backslash-escaping within strings.
        else if (c == '\\') {
          value->pop_back();
          prev_state = TOKEN_STATE_SINGLE_QUOTE;
          state = TOKEN_STATE_QUOTED_STRING;
        }
        continue;
      case TOKEN_STATE_DOUBLE_QUOTE:
        *value += c;
        if (c == '"') {
          // The end of a quoted token should be followed by whitespace.
          const char next_c = input->get();
          if (next_c == ' ' || next_c == '\t' || next_c == '\n' || next_c == '\t' ||
              next_c == ';' || next_c == '{' || next_c == '}') {
            input->unget();
          } else {
            return TOKEN_TYPE_ERROR;
          }
          return TOKEN_TYPE_NORMAL;
        }
        // Allow for backslash-escaping within strings.
        else if (c == '\\') {
          value->pop_back();
          prev_state = TOKEN_STATE_DOUBLE_QUOTE;
          state = TOKEN_STATE_QUOTED_STRING;
        }
        continue;
      case TOKEN_STATE_QUOTED_STRING:
        *value += c;
        if (prev_state == TOKEN_STATE_SINGLE_QUOTE) {
          state = TOKEN_STATE_SINGLE_QUOTE;
        }
        else if (prev_state == TOKEN_STATE_DOUBLE_QUOTE) {
          state = TOKEN_STATE_DOUBLE_QUOTE;
        }
      case TOKEN_STATE_TOKEN_TYPE_COMMENT:
        if (c == '\n' || c == '\r') {
          return TOKEN_TYPE_COMMENT;
        }
        *value += c;
        continue;
      case TOKEN_STATE_TOKEN_TYPE_NORMAL:
        if (c == ' ' || c == '\t' || c == '\n' || c == '\t' ||
            c == ';' || c == '{' || c == '}') {
          input->unget();
          return TOKEN_TYPE_NORMAL;
        }
        *value += c;
        continue;
    }
  }

  // If we get here, we reached the end of the file.
  if (state == TOKEN_STATE_SINGLE_QUOTE ||
      state == TOKEN_STATE_DOUBLE_QUOTE) {
    return TOKEN_TYPE_ERROR;
  }

  return TOKEN_TYPE_EOF;
}

/**
 * Parse NGINX configuration from byte stream.
 */
bool NginxConfigParser::Parse(std::istream* config_file, NginxConfig* config) 
{
  // Generate configuration stack and push on the NGINX configuration.
  std::stack<NginxConfig*> config_stack;
  config_stack.push(config);

  // Initialize the last token as the start of the file.
  TokenType last_token_type = TOKEN_TYPE_START;
  TokenType token_type;

  // Keeps track of start and end blocks to make sure they are all closed.
  int token_block = 0;
  while (true) {
    std::string token;
    token_type = ParseToken(config_file, &token);
    BOOST_LOG_TRIVIAL(info) << TokenTypeAsString(token_type) 
                            << ": "
                            << token.c_str();
    if (token_type == TOKEN_TYPE_ERROR) {
      break;
    }

    if (token_type == TOKEN_TYPE_COMMENT) {
      // Skip comments.
      continue;
    }

    if (token_type == TOKEN_TYPE_START) {
      // Error.
      break;
    } else if (token_type == TOKEN_TYPE_NORMAL) {
      if (last_token_type == TOKEN_TYPE_START ||
          last_token_type == TOKEN_TYPE_STATEMENT_END ||
          last_token_type == TOKEN_TYPE_START_BLOCK ||
          last_token_type == TOKEN_TYPE_END_BLOCK ||
          last_token_type == TOKEN_TYPE_NORMAL) {
        if (last_token_type != TOKEN_TYPE_NORMAL) {
          config_stack.top()->statements_.emplace_back(
              new NginxConfigStatement);
        }
        config_stack.top()->statements_.back().get()->tokens_.push_back(
            token);
      } else {
        // Error.
        break;
      }
    } else if (token_type == TOKEN_TYPE_STATEMENT_END) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
    } else if (token_type == TOKEN_TYPE_START_BLOCK) {
      token_block++;
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
      NginxConfig* const new_config = new NginxConfig;
      config_stack.top()->statements_.back().get()->child_block_.reset(
          new_config);
      config_stack.push(new_config);
    } else if (token_type == TOKEN_TYPE_END_BLOCK) {
      token_block--;
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_START_BLOCK &&
          last_token_type != TOKEN_TYPE_END_BLOCK) {
        // Allow for empty blocks and nested blocks.
        // Error.
        break;
      }
      config_stack.pop();
    } else if (token_type == TOKEN_TYPE_EOF) {
      if ((last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK) ||
          token_block != 0) {
        // Error.
        break;
      }
      return true;
    } else {
      // Error. Unknown token.
      break;
    }
    last_token_type = token_type;
  }

  BOOST_LOG_TRIVIAL(error) << "Bad transition from " 
                           << TokenTypeAsString(last_token_type)
                           << TokenTypeAsString(token_type);
  return false;
}

/**
 * Parse configuration from specified file.
 */
bool NginxConfigParser::Parse(const char* file_name, NginxConfig* config) 
{
  // Open configuration file.
  std::ifstream config_file;
  config_file.open(file_name);
  
  // Check if the configuration file was opened successfully.
  if (!config_file.good()) {
    BOOST_LOG_TRIVIAL(error) << "Failed to open config file: " << file_name;
    return false;
  }

  const bool return_value = Parse(dynamic_cast<std::istream*>(&config_file), config);
  config_file.close();
  return return_value;
}
