#ifndef WAVELET_TREE_H
#define WAVELET_TREE_H

#include <vector>
#include <memory>
#include <cstddef>

using namespace std;

class WaveletTree {
public:
    using symbol_t = int;

    WaveletTree() = default;
    
    explicit WaveletTree(const vector<symbol_t>& data);

    size_t size() const { return n; }

    size_t rank(symbol_t c, size_t index) const;

private:
    struct Node {
        
        symbol_t low;
        
        symbol_t high;
        
        vector<size_t> prefix_zeros;
        
        unique_ptr<Node> left;
        
        unique_ptr<Node> right;
    };

    unique_ptr<Node> root;
    
    size_t n = 0;

    unique_ptr<Node> build(const vector<symbol_t>& data, symbol_t low, symbol_t high);

    size_t rank_recursive(const Node* node, symbol_t c, size_t index) const;
};

#endif
