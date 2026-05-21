#pragma once
#include "database.h"
#include <string>
#include <vector>

struct Note {
    int id;
    std::string title;
    std::string body;
    std::string created_at;
};

class NotesManager {
public:
    NotesManager(Database& db);
    void add(const std::string& title, const std::string& body);
    std::vector<Note> list();
    void update(int id, const std::string& title, const std::string& body);
    void remove(int id);
    void search(const std::string& keyword);  // extra feature

private:
    sqlite3* db;
};