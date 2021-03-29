//  "Copyright [year] <Copyright Owner>"
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
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>
#include <string>
#include <ostream>
#include <strstream>
#include <fstream>

#ifndef INCLUDE_SERVER_HPP_
#define INCLUDE_SERVER_HPP_

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// This is the C++11 equivalent of a generic lambda.
// The function object is used to send an HTTP message.
template<class Stream>
struct send_lambda
{
  Stream& stream_;
  bool& close_;
  beast::error_code& ec_;

  explicit
  send_lambda(
      Stream& stream,
      bool& close,
      beast::error_code& ec)
      : stream_(stream)
      , close_(close)
      , ec_(ec)
  {
  }

  template<bool isRequest, class Body, class Fields>
  void
  operator()(http::message<isRequest, Body, Fields>&& msg) const
  {
    // Determine if we should close the connection after
    close_ = msg.need_eof();

    // We need the serializer here because the serializer requires
    // a non-const file_body, and the message oriented version of
    // http::write only works with const messages.
    http::serializer<isRequest, Body, Fields> sr{msg};
    http::write(stream_, sr, ec_);
  }
};


class Server
{
 public:
  Server();
  template< class Body, class Allocator, class Send>
  void handle_request(
      http::request<Body, http::basic_fields<Allocator>>&& req,
      Send&& send);
  void fail(beast::error_code ec, char const* what);
  void do_session(tcp::socket& socket);
  void update_collections();
  void sort_collection();
  std::string create_responce(std::string id);

 private:
  std::timed_mutex _lock;
  nlohmann::json _js;
  std::vector<std::pair<std::pair<std::string, std::string>,  uint32_t>> _vec;
};

#endif  // INCLUDE_SERVER_HPP_
