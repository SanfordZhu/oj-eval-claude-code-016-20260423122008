#ifndef BPT_H
#define BPT_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <memory>
#include <mutex>

const int BPT_ORDER = 100;  // B+ tree order (reduced for simplicity)
const std::string DATA_FILE = "data.bpt";
const std::string INDEX_FILE = "index.bpt";

struct BPTNode {
    bool is_leaf;
    std::vector<std::string> keys;
    std::vector<int> values;  // Only used for leaf nodes
    std::vector<int> children; // Block indices for child nodes
    int next_leaf; // Block index of next leaf node (-1 if last)
    int block_id;  // Block ID of this node

    BPTNode() : is_leaf(true), next_leaf(-1), block_id(-1) {}
};

class BPTree {
private:
    std::string data_file;
    std::string index_file;
    int root_block;
    int next_block_id;
    std::map<std::string, std::set<int>> memory_index; // In-memory index for speed
    std::mutex mtx;
    bool use_memory_only;

    int allocate_block();
    void write_node(int block_id, const BPTNode& node);
    BPTNode read_node(int block_id);
    void insert_to_leaf(const std::string& key, int value);
    void split_and_insert_leaf(const std::string& key, int value);
    void insert_to_parent(int left_block, const std::string& key, int right_block);

public:
    BPTree(const std::string& data_path = DATA_FILE, const std::string& index_path = INDEX_FILE);
    ~BPTree();

    void insert(const std::string& key, int value);
    void remove(const std::string& key, int value);
    std::vector<int> find(const std::string& key);
    void clear();
};

#endif // BPT_H