//
// Created by phirasit on 1/23/20.
//

#ifndef GRADER_ROUTER_HPP
#define GRADER_ROUTER_HPP

#include "pistache/router.h"
#include "pistache/http.h"
#include "pistache/endpoint.h"
#include "grader/grader_pool.hpp"

using namespace Pistache;

class GraderRestServer : public Http::Endpoint {
protected:
    Rest::Router router;
    GraderPool* const pool;
    
    void setup_routing();
    
    // list of all resource functions
    void get_grader_info(const Http::Request&, Http::ResponseWriter);
    void invalid_path(const Http::Request&, Http::ResponseWriter);
    
public:
    explicit GraderRestServer(Address addr, GraderPool* main_pool) :
      Http::Endpoint(addr), pool(main_pool)
    {
      this->setup_routing();
    }
    ~GraderRestServer() = default;
    
    void start() {
      this->setHandler(router.handler());
      this->serveThreaded();
    }
};

#endif //GRADER_ROUTER_HPP
