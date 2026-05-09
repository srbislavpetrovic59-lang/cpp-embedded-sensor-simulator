#include "rest/RestServer.h"

#include "rest/SensorData.h"

#include "../../third_party/httplib.h"

#include <iostream>
#include <sstream>

void RestServer::start(int port)
{
    httplib::Server svr;

    svr.Get("/status",
        [](const httplib::Request&,
            httplib::Response& res)
        {
            res.set_header(
                "Access-Control-Allow-Origin",
                "*"
            );
            res.set_content(
                "REST server running",
                "text/plain"
            );
        });

    svr.Get("/latest",
        [](const httplib::Request&,
            httplib::Response& res)
        {
            res.set_header(
                "Access-Control-Allow-Origin",
                "*"
            );
            std::ostringstream json;

            json << "{";
            json << "\"temperature\": "
                << g_sensorData.temperature << ",";

            json << "\"pressure\": "
                << g_sensorData.pressure << ",";

            json << "\"humidity\": "
                << g_sensorData.humidity;

            json << "}";

            res.set_content(
                json.str(),
                "application/json"
            );
        });

    std::cout
        << "REST API listening on port "
        << port
        << std::endl;

    svr.listen("0.0.0.0", port);
}