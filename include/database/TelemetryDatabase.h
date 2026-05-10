#pragma once

#include <string>

struct sqlite3;

class TelemetryDatabase
{
public:
    bool open(const std::string& filename);

    void insert(
        const std::string& type,
        float value);

private:
    sqlite3* db = nullptr;

    void createTable();
};