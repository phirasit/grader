//
// Created by phirasit on 1/23/20.
//

#include "router.hpp"

#include "basen.hpp"
#include "file.hpp"
#include "json/json.h"
#include "submission.hpp"

#include <fstream>
#include <optional>

using namespace Pistache;

/////// Routing //////

void GraderRestServer::setup_routing() {
  using namespace Rest;
  
  // list of all routing
  Routes::Get(this->router, "/info", Routes::bind(&GraderRestServer::get_grader_info, this));
  Routes::Post(this->router, "/submit", Routes::bind(&GraderRestServer::add_submission, this));
  Routes::Get(this->router, "/get_result", Routes::bind(&GraderRestServer::get_result, this));
  
  Routes::NotFound(this->router, Routes::bind(&GraderRestServer::invalid_path, this));
}

//////// Utility Functions ////////

static std::optional<Json::Value> read_json_request(const Http::Request& request) {
  static Json::CharReaderBuilder reader_builder;
  static Json::CharReader* reader = reader_builder.newCharReader();
  
  const std::string& body = request.body();
  Json::Value json_body;
  std::string errors;
  
  return reader->parse(body.c_str(), body.c_str() + body.length(), &json_body, &errors)
    ? std::make_optional(json_body)
    : std::nullopt;
}

static void write_json_response(Http::ResponseWriter& response, const Http::Code& code, Json::Value& body) {
  static Json::StreamWriterBuilder builder;
  static std::once_flag run_once;
  std::call_once(run_once, [] () {
    builder["indentation"] = "";
  });
  body["success"] = (code == Http::Code::Ok);
  response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
  response.send(code, Json::writeString(builder, body));
}

////// Middleware ////////

#define GET_REQUEST_JSON_BODY(request, response, json_body) \
  const std::optional<Json::Value> json_body_parse_result = read_json_request(request); \
  if (!json_body_parse_result.has_value()) { \
    Json::Value body; \
    body["error"] = "invalid json request"; \
    write_json_response(response, Http::Code::Bad_Request, body); \
    return; \
  } \
  const Json::Value json_body = json_body_parse_result.value();


#define ERROR(response, code, msg, body) \
  body["error"] = msg; \
  write_json_response(response, code, body);\
  return;
  
/////// API /////////

void GraderRestServer::get_grader_info(const Http::Request &request, Http::ResponseWriter response) {
  Json::Value body;
  body["status"] = to_string(this->pool->get_status());
  body["number-graders"] = this->pool->get_num_graders();
  body["number-of-queued-submissions"] = this->pool->get_waiting_pool_size();
  
  body["graders-status"] = Json::arrayValue;
  for (size_t i = 0; i < this->pool->get_num_graders(); ++i) {
    body["graders-status"].append(std::to_string(this->pool->get_grader_status(i)));
  }
  write_json_response(response, Http::Code::Ok, body);
}

void GraderRestServer::add_submission(const Http::Request &request, Http::ResponseWriter response) {
  GET_REQUEST_JSON_BODY(request, response, json_body)
  Json::Value body;
  
  // create a file
  if (!json_body["file-data"]) {
    ERROR(response, Http::Code::Bad_Request, "missing field `file-data`", body);
  }
  
  // decode base64 to file
  const std::string& file_content_base64 = json_body["file-data"].asString();
  std::string file_content;
  bn::decode_b64(file_content_base64.begin(), file_content_base64.end(), std::back_inserter(file_content));
  const file::File& filename = file::get_path("/submissions/" + json_body["id"].asString());
  
  std::ofstream output_file (filename);
  output_file << file_content;
  output_file.close();
  
  // parse a submission
  const std::optional<Submission>& parse_result = Submission::from_json(json_body, filename);
  
  // invalid submission
  if (!parse_result.has_value()) {
    ERROR(response, Http::Code::Bad_Request, "invalid submission format", body);
  }
  // add new submission
  const Submission& submission = parse_result.value();
  
  this->pool->add_submission(submission);
  
  body["submission-id"] = submission.get_submission_id();
  body["task-id"] = submission.get_task_id();
  write_json_response(response, Http::Code::Ok, body);
}

void GraderRestServer::get_result(const Http::Request &request, Http::ResponseWriter response) {
  GET_REQUEST_JSON_BODY(request, response, json_body)
  
  Json::Value body;
  if (!json_body["id"]) {
    ERROR(response, Http::Code::Bad_Request, "missing `id` field", body);
  }
  
  const SubmissionID& submission_id = json_body["id"].asString();
  const SUBMISSION_STATUS status = this->pool->get_submission_status(submission_id);
  body["submission-id"] = submission_id;
  body["status"] = to_string(status);
  
  // if there is a result file
  if (status == SUBMISSION_STATUS_GRADED) {
    // read result as base64 string
    std::ifstream result_file (this->pool->get_result_file(submission_id));
    if (!result_file.is_open()) {
      ERROR(response, Http::Code::Bad_Request, "cannot open result file", body);
    }
    std::string file_base64;
    bn::encode_b64(
      std::istreambuf_iterator<char>(result_file),
      std::istreambuf_iterator<char>(),
      std::back_inserter(file_base64));
    body["result"] = file_base64;
  }
  
  write_json_response(response, Http::Code::Ok, body);
}

void GraderRestServer::invalid_path(const Http::Request &request, Http::ResponseWriter response) {
  Json::Value body;
  body["path"] = request.resource();
  ERROR(response, Http::Code::Not_Found, "path does not exist", body);
}

