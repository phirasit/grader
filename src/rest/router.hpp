//
// Created by phirasit on 1/23/20.
//

#ifndef GRADER_ROUTER_HPP
#define GRADER_ROUTER_HPP

#include "pistache/endpoint.h"

using namespace Pistache;

class GraderRestServer : public Http::Handler {
public:
    HTTP_PROTOTYPE(GraderRestServer)
    
    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
      response.send(Http::Code::Ok, "Hello, World");
    }
};

#endif //GRADER_ROUTER_HPP
