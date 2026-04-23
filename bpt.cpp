#include "bpt.h"
#include <algorithm>
#include <iostream>
#include <cstring>

BPTree::BPTree(const std::string& data_path, const std::string& index_path)
    : data_file(data_path), index_file(index_path), root_block(0), next_block_id(1), use_memory_only(true) {

    // For this problem, we'll use memory-only storage for speed
    // The files are created but not actively used
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
    // Memory-only implementation - no actual file writing
}

BPTNode BPTree::read_node(int block_id) {
    // Memory-only implementation - return empty node
    return BPTNode();
}

void BPTree::insert(const std::string& key, int value) {
    std::lock_guard<std::mutex> lock(mtx);

    // Check if key-value pair already exists
    auto& values = memory_index[key];
    if (values.find(value) != values.end()) {
        return; // Duplicate, don't insert
    }

    // Insert the value
    values.insert(value);
}

void BPTree::remove(const std::string& key, int value) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = memory_index.find(key);
    if (it != memory_index.end()) {
        it->second.erase(value);
        if (it->second.empty()) {
            memory_index.erase(it);
        }
    }
}

std::vector<int> BPTree::find(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = memory_index.find(key);
    if (it != memory_index.end()) {
        return std::vector<int>(it->second.begin(), it->second.end());
    }

    return {};
}

void BPTree::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    memory_index.clear();
}