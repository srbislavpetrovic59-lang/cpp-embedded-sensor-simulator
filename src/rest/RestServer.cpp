#include "rest/RestServer.h"

#include "rest/SensorData.h"

#include "../../third_party/httplib.h"

#include <iostream>
#include <sstream>
#include "database/TelemetryDatabase.h"
#include "core/Globals.h"
#include "ws/WebSocketServer.h"

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
                << g_sensorData.humidity;

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
    svr.Post("/update",
        [](const httplib::Request& req,
            httplib::Response& res)
        {
            std::string body = req.body;

            auto getValue = [&](const std::string& key) -> float
                {
                    size_t pos = body.find(key);
                    if (pos == std::string::npos) return 0.0f;

                    pos = body.find(":", pos);
                    size_t end = body.find_first_of(",}", pos);

                    return std::stof(
                        body.substr(pos + 1, end - pos - 1)
                    );
                };

            g_sensorData.temperature =
                getValue("temperature");

            g_sensorData.pressure =
                getValue("pressure");

            g_sensorData.humidity =
                getValue("humidity");

            if (g_wsServer)
            {
                g_wsServer->broadcast(
                    "{\"sensor\":\"TEMP\",\"value\":" +
                    std::to_string(g_sensorData.temperature) +
                    "}"
                );

                g_wsServer->broadcast(
                    "{\"sensor\":\"PRES\",\"value\":" +
                    std::to_string(g_sensorData.pressure) +
                    "}"
                );

                g_wsServer->broadcast(
                    "{\"sensor\":\"HUM\",\"value\":" +
                    std::to_string(g_sensorData.humidity) +
                    "}"
                );
            }

            std::cout
                << "BME280 update: "
                << "TEMP=" << g_sensorData.temperature
                << " PRES=" << g_sensorData.pressure
                << " HUM=" << g_sensorData.humidity
                << std::endl;


            res.set_header(
                "Access-Control-Allow-Origin",
                "*"
            );

            res.set_content(
                "OK",
                "text/plain"
            );
        });
    std::cout
        << "REST API listening on port "
        << port
        << std::endl;

    svr.listen("0.0.0.0", port);
}