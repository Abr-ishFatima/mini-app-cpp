#include "database.h"
#include <stdexcept>

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK)
        throw std::runtime_error("Cannot open database");

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            body TEXT,
            created_at TEXT DEFAULT (datetime('now'))
        );
    )";
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
}

Database::~Database() {
    sqlite3_close(db);
}