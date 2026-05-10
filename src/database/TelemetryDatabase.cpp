#include "database/TelemetryDatabase.h"

#include <iostream>
#include <sstream>
#include "../../third_party/sqlite/sqlite3.h"

bool TelemetryDatabase::open(
    const std::string& filename)
{
    int result =
        sqlite3_open(filename.c_str(), &db);

    if (result != SQLITE_OK)
    {
        std::cout
            << "Failed to open database\n";

        return false;
    }

    std::cout
        << "Database opened\n";

    createTable();

    return true;
}

void TelemetryDatabase::createTable()
{
    const char* sql =
        "CREATE TABLE IF NOT EXISTS telemetry ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "type TEXT,"
        "value REAL"
        ");";

    char* errMsg = nullptr;

    sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errMsg);
}

void TelemetryDatabase::insert(
    const std::string& type,
    float value)
{
    std::ostringstream sql;

    sql
        << "INSERT INTO telemetry(type, value) VALUES('"
        << type
        << "', "
        << value
        << ");";

    char* errMsg = nullptr;

    sqlite3_exec(
        db,
        sql.str().c_str(),
        nullptr,
        nullptr,
        &errMsg);
}