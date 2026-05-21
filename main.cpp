#include <iostream>
#include "database.h"
#include "notes.h"

void print_usage() {
    std::cout << "Usage:\n"
              << "  notes add <title> <body>\n"
              << "  notes list\n"
              << "  notes update <id> <title> <body>\n"
              << "  notes delete <id>\n"
              << "  notes search <keyword>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { print_usage(); return 1; }

    Database db("notes.db");
    NotesManager mgr(db);

    std::string cmd = argv[1];

    if (cmd == "add" && argc == 4) {
        mgr.add(argv[2], argv[3]);
    } else if (cmd == "list") {
        auto notes = mgr.list();
        if (notes.empty()) { std::cout << "No notes yet.\n"; return 0; }
        for (auto& n : notes)
            std::cout << "[" << n.id << "] " << n.title << "\n"
                      << n.body << "\n"
                      << "Created: " << n.created_at << "\n\n";
    } else if (cmd == "update" && argc == 5) {
        mgr.update(std::stoi(argv[2]), argv[3], argv[4]);
    } else if (cmd == "delete" && argc == 3) {
        mgr.remove(std::stoi(argv[2]));
    } else if (cmd == "search" && argc == 3) {
        mgr.search(argv[2]);
    } else {
        print_usage();
    }

    return 0;
}