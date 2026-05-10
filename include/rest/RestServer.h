#pragma once
#include "database/TelemetryDatabase.h"

class RestServer
{
public:
    void start(int port, TelemetryDatabase& database);
};