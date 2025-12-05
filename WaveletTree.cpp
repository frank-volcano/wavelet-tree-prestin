#include "WaveletTree.h"
#include <algorithm>

using namespace std;

WaveletTree::WaveletTree(const vector<symbol_t>& data) {
    if (data.empty()) return;

    auto p = minmax_element(data.begin(), data.end());
    
    symbol_t low = *p.first;
    symbol_t high = *p.second;
    
    n = data.size();

    root = build(data, low, high);
}

unique_ptr<WaveletTree::Node>

WaveletTree::build(const vector<symbol_t>& data, symbol_t low, symbol_t high) {

    if (data.empty() || low > high) return nullptr;

    auto node = make_unique<Node>();
    node->low = low;
    node->high = high;

    if (low == high || data.size() == 1) {
        
        return node;
    }

    symbol_t mid = low + (high - low) / 2;

    node->prefix_zeros.resize(data.size());
    
    vector<symbol_t> left_data;
    vector<symbol_t> right_data;
    
    left_data.reserve(data.size());
    
    right_data.reserve(data.size());

    size_t zero_count = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        
        symbol_t c = data[i];
        
        if (c <= mid) {
            
            left_data.push_back(c);
            
            zero_count++;
        
        } else {
            
            right_data.push_back(c);
        }
        
        node->prefix_zeros[i] = zero_count;
    }

    if (!left_data.empty()) {
        
        node->left = build(left_data, low, mid);
    }

    if (!right_data.empty()) {
        
        node->right = build(right_data, mid + 1, high);
    }

    return node;
}

size_t WaveletTree::rank(symbol_t c, size_t index) const {
    if (!root || index >= n) return 0;
    return rank_recursive(root.get(), c, index);
}

size_t WaveletTree::rank_recursive(const Node* node,
                                   symbol_t c,
                                   size_t index) const {

    if (!node || c < node->low || c > node->high) {
        return 0;
    }

    if ((node->low == node->high) ||
        (!node->left && !node->right)) {
        return index + 1;
    }

    symbol_t mid = node->low + (node->high - node->low) / 2;

    size_t zeros = node->prefix_zeros[index];
    size_t ones = (index + 1) - zeros;

    if (c <= mid) {
        if (!node->left || zeros == 0) return 0;
        return rank_recursive(node->left.get(), c, zeros - 1);
    } else {
        if (!node->right || ones == 0) return 0;
        return rank_recursive(node->right.get(), c, ones - 1);
    }
}
