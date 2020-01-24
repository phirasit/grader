//
// Created by phirasit on 1/23/20.
//

#include "router.hpp"
#include "json/json.h"

using namespace Pistache;

void GraderRestServer::setup_routing() {
  using namespace Rest;
  
  // list of all routing
  Routes::Get(this->router, "/info", Routes::bind(&GraderRestServer::get_grader_info, this));
  
  Routes::NotFound(this->router, Routes::bind(&GraderRestServer::invalid_path, this));
}

static void write_json_response(Http::ResponseWriter& response, const Http::Code& code, const Json::Value& body) {
  static Json::StreamWriterBuilder builder;
  static std::once_flag run_once;
  std::call_once(run_once, [] () {
    builder["indentation"] = "";
  });
  response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
  response.send(code, Json::writeString(builder, body));
}

void GraderRestServer::get_grader_info(const Http::Request &request, Http::ResponseWriter response) {
  Json::Value body;
  body["status"] = std::to_string(this->pool->get_status());
  body["number-graders"] = this->pool->get_num_graders();
  body["number-of-queued-submissions"] = this->pool->get_waiting_pool_size();
  
  body["graders-status"] = Json::arrayValue;
  for (size_t i = 0; i < this->pool->get_num_graders(); ++i) {
    body["graders-status"].append(std::to_string(this->pool->get_grader_status(i)));
  }
  write_json_response(response, Http::Code::Ok, body);
}

void GraderRestServer::invalid_path(const Http::Request &request, Http::ResponseWriter response) {
  Json::Value body;
  body["error"] = "path does not exist";
  body["path"] = request.resource();
  write_json_response(response, Http::Code::Not_Found, body);
}

