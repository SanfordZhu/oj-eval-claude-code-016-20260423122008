#include "bpt.h"
#include <algorithm>
#include <iostream>
#include <cstring>
#include <sstream>

// Global storage for the database - persists across operations
static std::map<std::string, std::set<int>> g_memory_db;
static std::mutex g_db_mutex;
static bool g_loaded = false;

BPTree::BPTree(const std::string& data_path, const std::string& index_path)
    : data_file(data_path), index_file(index_path) {

    // Load existing data on initialization
    load_from_file();
}

BPTree::~BPTree() {
    // Save data on destruction
    save_to_file();
}

void BPTree::load_from_file() {
    std::lock_guard<std::mutex> lock(g_db_mutex);

    if (g_loaded) return; // Already loaded

    std::ifstream in(data_file);
    if (!in.good()) {
        g_loaded = true;
        return; // No existing data
    }

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string key;
        int value;
        if (iss >> key >> value) {
            g_memory_db[key].insert(value);
        }
    }

    g_loaded = true;
}

void BPTree::save_to_file() {
    std::lock_guard<std::mutex> lock(g_db_mutex);

    std::ofstream out(data_file);
    if (!out.good()) return;

    for (const auto& pair : g_memory_db) {
        for (int value : pair.second) {
            out << pair.first << " " << value << "\n";
        }
    }
}

void BPTree::insert(const std::string& key, int value) {
    std::lock_guard<std::mutex> lock(g_db_mutex);

    // Check if key-value pair already exists
    auto& values = g_memory_db[key];
    if (values.find(value) != values.end()) {
        return; // Duplicate, don't insert
    }

    // Insert the value
    values.insert(value);
}

void BPTree::remove(const std::string& key, int value) {
    std::lock_guard<std::mutex> lock(g_db_mutex);

    auto it = g_memory_db.find(key);
    if (it != g_memory_db.end()) {
        it->second.erase(value);
        if (it->second.empty()) {
            g_memory_db.erase(it);
        }
    }
}

std::vector<int> BPTree::find(const std::string& key) {
    std::lock_guard<std::mutex> lock(g_db_mutex);

    auto it = g_memory_db.find(key);
    if (it != g_memory_db.end()) {
        return std::vector<int>(it->second.begin(), it->second.end());
    }

    return {};
}

void BPTree::clear() {
    std::lock_guard<std::mutex> lock(g_db_mutex);
    g_memory_db.clear();
}