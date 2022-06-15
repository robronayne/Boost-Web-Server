//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <csignal>
#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sources/logger.hpp>

#include "server.h"
#include "session.h"

using boost::asio::ip::tcp;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

// Signal handler for interrupt
void signal_logger(int signal)
{
  BOOST_LOG_TRIVIAL(error) << "Received signal "
                           << signal
                           << ", shutting down";

  exit(signal);
}

// Initialize logging parameters
void init_log() {
  // Add file log parameters
  logging::add_file_log
    (
        keywords::file_name = "log/SYSLOG_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%][%ThreadID%][%Severity%]: %Message%",
        keywords::auto_flush = true
    );

  // Add console log parameters
  logging::add_console_log
    (
        std::cout,
        keywords::format = "[%TimeStamp%][%ThreadID%][%Severity%]: %Message%",
        keywords::auto_flush = true

    );

  // Filter out message with severity level lower than info
  logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

int main(int argc, char* argv[])
{
  // Initialize all logging
  signal(SIGINT, signal_logger);
  init_log();
  logging::add_common_attributes();

  try
  {
    if (argc != 2)
    {
      BOOST_LOG_TRIVIAL(error) << "Usage: async_tcp_echo_server <config>\n";
      return 1;
    }

    NginxConfigParser config_parser;
    NginxConfig config;

    BOOST_LOG_TRIVIAL(info) << "Parsing config file: " + std::string(argv[1]);

    if (!config_parser.Parse(argv[1], &config))
    {
      BOOST_LOG_TRIVIAL(error) << "Invalid config file";
      return -1;
    }

    BOOST_LOG_TRIVIAL(info) << "Parsing complete";
    
    int port = config.getPortNum();
    if (port == -1)
    {
      BOOST_LOG_TRIVIAL(error) << "Invalid port number";
      return -1;
    }

    boost::asio::io_service io_service;

    session s(io_service);

    server serv(s, io_service, port);
    serv.set_paths(config.getPaths());

    BOOST_LOG_TRIVIAL(info) << "Starting server on port: " << port;

    serv.start_accept();

    BOOST_LOG_TRIVIAL(info) << "Now accepting connections";

    io_service.run();
  }
  catch (std::exception& e)
  {
    BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what() << "\n";
  }

  return 0;
}
