#include "notes.h"
#include <iostream>

NotesManager::NotesManager(Database& db) : db(db.get()) {}

void NotesManager::add(const std::string& title, const std::string& body) {
    std::string sql = "INSERT INTO notes (title, body) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, body.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Note added.\n";
}

std::vector<Note> NotesManager::list() {
    std::vector<Note> notes;
    const char* sql = "SELECT id, title, body, created_at FROM notes ORDER BY id;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Note n;
        n.id = sqlite3_column_int(stmt, 0);
        n.title = (const char*)sqlite3_column_text(stmt, 1);
        n.body = sqlite3_column_text(stmt, 2) ? (const char*)sqlite3_column_text(stmt, 2) : "";
        n.created_at = (const char*)sqlite3_column_text(stmt, 3);
        notes.push_back(n);
    }
    sqlite3_finalize(stmt);
    return notes;
}

void NotesManager::update(int id, const std::string& title, const std::string& body) {
    std::string sql = "UPDATE notes SET title=?, body=? WHERE id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, body.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Note updated.\n";
}

void NotesManager::remove(int id) {
    // Edge case: check if note exists before deleting
    std::string check_sql = "SELECT id FROM notes WHERE id=?;";
    sqlite3_stmt* check;
    sqlite3_prepare_v2(db, check_sql.c_str(), -1, &check, nullptr);
    sqlite3_bind_int(check, 1, id);
    if (sqlite3_step(check) != SQLITE_ROW) {
        std::cout << "Error: Note with ID " << id << " does not exist.\n";
        sqlite3_finalize(check);
        return;
    }
    sqlite3_finalize(check);

    std::string sql = "DELETE FROM notes WHERE id=?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Note deleted.\n";
}

// Extra feature: keyword search
void NotesManager::search(const std::string& keyword) {
    std::string sql = "SELECT id, title, body, created_at FROM notes WHERE title LIKE ? OR body LIKE ?;";
    std::string pattern = "%" + keyword + "%";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "[" << sqlite3_column_int(stmt, 0) << "] "
                  << sqlite3_column_text(stmt, 1) << "\n"
                  << sqlite3_column_text(stmt, 2) << "\n"
                  << "Created: " << sqlite3_column_text(stmt, 3) << "\n\n";
    }
    sqlite3_finalize(stmt);
}