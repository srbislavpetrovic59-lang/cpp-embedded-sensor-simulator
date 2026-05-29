#include "rest/RestServer.h"

#include "rest/SensorData.h"

#include "../../third_party/httplib.h"

#include <iostream>
#include <sstream>
#include "database/TelemetryDatabase.h"
#include "utils/TimeUtils.h"

void RestServer::start(int port, TelemetryDatabase& database)
{
    httplib::Server svr;
    svr.set_mount_point(
        "/",
        "C:/Vezbe/EmbeddedSimulator/web"
    );

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
                << g_sensorData.humidity << ", ";

            json << "\"timestamp\": \""
                << getTimestamp() << "\"";
            json << "}";

            res.set_content(
                json.str(),
                "application/json"
            );
        });

    svr.Get("/history",
        [&](const httplib::Request&,
            httplib::Response& res)
        {
            res.set_header(
                "Access-Control-Allow-Origin",
                "*"
            );

            std::string json =
                database.getHistoryJson();

            res.set_content(
                json,
                "application/json"
            );
        });
    std::cout
        << "REST API listening on port "
        << port
        << std::endl;

    svr.listen("0.0.0.0", port);
}