#include "database/TelemetryDatabase.h"

#include <iostream>
#include <sstream>
#include "sqlite/sqlite3.h"

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

std::string TelemetryDatabase::getLatestTelemetryJson()
{
    std::stringstream ss;

    ss << "{";

    sqlite3_stmt* stmt;

    const char* sql =
        "SELECT sensor_type, value "
        "FROM telemetry "
        "ORDER BY id DESC LIMIT 3;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        bool first = true;

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string sensor =
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            double value = sqlite3_column_double(stmt, 1);

            if (!first)
                ss << ",";

            ss << "\"" << sensor << "\":" << value;

            first = false;
        }
    }

    sqlite3_finalize(stmt);

    ss << "}";

    return ss.str();
}

std::string TelemetryDatabase::getHistoryJson()
{
    sqlite3_stmt* stmt;

    std::stringstream json;

    json << "[";

    const char* sql =
        "SELECT type, value, timestamp "
        "FROM telemetry "
        "ORDER BY id DESC LIMIT 30;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        bool first = true;

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string sensor =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 0));

            double value =
                sqlite3_column_double(stmt, 1);

            std::string timestamp =
                reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 2));

            if (!first)
                json << ",";

            json << "{";
            json << "\"sensor\":\"" << sensor << "\",";
            json << "\"value\":" << value << ",";
            json << "\"timestamp\":\"" << timestamp << "\"";
            json << "}";

            first = false;
        }
    }

    sqlite3_finalize(stmt);

    json << "]";

    return json.str();
}

