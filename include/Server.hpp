//
// Created by mrbgn on 3/22/21.
//

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <nlohmann/json.hpp>
#include <string>

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

class Server
{
 public:
  Server(std::string address, unsigned port);
};

#endif  // SERVER_SERVER_HPP
