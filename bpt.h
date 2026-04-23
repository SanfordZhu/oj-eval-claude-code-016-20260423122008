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

const std::string DATA_FILE = "data.db";
const std::string INDEX_FILE = "index.db";

class BPTree {
private:
    std::string data_file;
    std::string index_file;
    std::mutex mtx;

    void load_from_file();
    void save_to_file();

public:
    BPTree(const std::string& data_path = DATA_FILE, const std::string& index_path = INDEX_FILE);
    ~BPTree();

    void insert(const std::string& key, int value);
    void remove(const std::string& key, int value);
    std::vector<int> find(const std::string& key);
    void clear();
};

#endif // BPT_H