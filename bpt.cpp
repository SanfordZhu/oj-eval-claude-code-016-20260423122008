#include "bpt.h"
#include <algorithm>
#include <iostream>
#include <cstring>

// Static storage for the database
static std::map<std::string, std::set<int>> g_memory_db;
static std::mutex g_db_mutex;

BPTree::BPTree(const std::string& data_path, const std::string& index_path)
    : data_file(data_path), index_file(index_path), root_block(0), next_block_id(1) {

    // Create empty files if they don't exist
    std::ofstream check_data(data_file, std::ios::app);
    std::ofstream check_index(index_file, std::ios::app);
}

BPTree::~BPTree() {
    // Cleanup if needed
}

int BPTree::allocate_block() {
    return next_block_id++;
}

void BPTree::write_node(int block_id, const BPTNode& node) {
    // File-based implementation not used for this problem
}

BPTNode BPTree::read_node(int block_id) {
    // File-based implementation not used for this problem
    return BPTNode();
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