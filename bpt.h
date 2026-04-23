#ifndef BPT_H
#define BPT_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <memory>
#include <mutex>
#include <algorithm>

const int BPT_ORDER = 50;  // B+ tree order
const std::string DATA_FILE = "data.bpt";
const std::string INDEX_FILE = "index.bpt";
const int BLOCK_SIZE = 4096;

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
    std::mutex mtx;

    int allocate_block();
    void write_node(int block_id, const BPTNode& node);
    BPTNode read_node(int block_id);
    void insert_to_leaf(const std::string& key, int value);
    int find_leaf_for_insert(const std::string& key);
    std::vector<int> find_in_tree(const std::string& key);
    void delete_from_leaf(const std::string& key, int value);

public:
    BPTree(const std::string& data_path = DATA_FILE, const std::string& index_path = INDEX_FILE);
    ~BPTree();

    void insert(const std::string& key, int value);
    void remove(const std::string& key, int value);
    std::vector<int> find(const std::string& key);
    void clear();
};

#endif // BPT_H