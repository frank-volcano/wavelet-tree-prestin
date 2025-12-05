#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <utility>
#include "WaveletTree.h"

using namespace std;
using std::move;


struct FrameData {
    
    vector<int> bins;
    unique_ptr<WaveletTree> wt;
    
    int min_bin;
    int max_bin;
    string name;
};

vector<int> read_sequence_from_file(const string& filename) {
    
    ifstream in(filename);
    vector<int> values;

    if (!in) {
        
        cout << "Could not open file: " << filename << endl;
        
        return values;
    }

    string line;
    
    while (getline(in, line)) {
        
        if (line.empty()) continue;
        
        if (line[0] == '#') continue;

        istringstream iss(line);
        
        int x;
        
        while (iss >> x) {
            values.push_back(x);
        }
    }

    return values;
}

size_t count_symbol_in_interval(const WaveletTree& wt, int c, size_t L, size_t R) {

    if (R < L) return 0;
    
    size_t r = wt.rank(c, R);
    
    size_t l = (L == 0 ? 0 : wt.rank(c, L - 1));
    
    if (r < l) return 0;
    
    return r - l;
}

size_t count_region_in_interval(const WaveletTree& wt, int bin_low, int bin_high, int global_min_bin, int global_max_bin, size_t L, size_t R) {

    if (R < L) return 0;

    if (bin_low > bin_high) {
        
        int t = bin_low;
        
        bin_low = bin_high;
        bin_high = t;
    }

    if (bin_low < global_min_bin) bin_low = global_min_bin;
    
    if (bin_high > global_max_bin) bin_high = global_max_bin;

    size_t total = 0;
    
    for (int x = bin_low; x <= bin_high; ++x) {
        total += count_symbol_in_interval(wt, x, L, R);
    }

    return total;
}

void print_menu() {
    
    cout << endl;
    
    cout << "Wavelet Tree MD Occupancy Demo (Multi-frame)" << endl;
    
    cout << "1) Add frame from file" << endl;
    
    cout << "2) Show summary of all frames" << endl;
    
    cout << "3) Per-frame rank(c, index)" << endl;
    
    cout << "4) Per-frame count symbol c in [L, R]" << endl;
    
    cout << "5) Per-frame count region [bin_low, bin_high] in [L, R]" << endl;
    
    cout << "6) Time series: region fraction per frame" << endl;
    
    cout << "0) Quit" << endl;
    
    cout << "Choice: ";

}

int choose_frame_index(const vector<FrameData>& frames) {
    
    if (frames.empty()) {
        
        cout << "No frames loaded." << endl;
        
        return -1;
    }

    cout << "Available frames:" << endl;

    for (size_t i = 0; i < frames.size(); ++i) {
        
        cout << "[" << i << "] " << frames[i].name << " length=" << frames[i].bins.size() << " range=[" << frames[i].min_bin << ", " << frames[i].max_bin << "]" << endl;
    }

    cout << "Enter frame index: ";
    
    int idx;
    
    cin >> idx;

    if (idx < 0 || (size_t)idx >= frames.size()) {
        
        cout << "Invalid frame index." << endl;
        
        return -1;
    }

    return idx;
}

int main() {

    vector<FrameData> frames;

    while (true) {
        print_menu();
        
        int choice;
        
        cin >> choice;

        if (!cin) {
            
            cout << "Input error, exiting now! :)" << endl;
            
            break;
        }

        if (choice == 0) {
            
            cout << "Goodbye! :)" << endl;
            
            break;
        }


        if (choice == 1) {
            
            cout << "Enter frame file path: ";
            
            string path;
            
            cin >> path;

            vector<int> seq = read_sequence_from_file(path);

            if (seq.empty()) {
                
                cout << "No data loaded." << endl;
                
                continue;
            }

            auto mm = minmax_element(seq.begin(), seq.end());

            FrameData f;
            
            f.name = path;
            f.bins = seq;
            f.min_bin = *mm.first;
            f.max_bin = *mm.second;
            f.wt = make_unique<WaveletTree>(f.bins);

            cout << "Loaded frame \"" << path << "\" length=" << f.bins.size() << " range=[" << f.min_bin << ", " << f.max_bin << "]" << endl;

            frames.push_back(move(f));
        }


        else if (choice == 2) {
            
            if (frames.empty()) {
                
                cout << "No frames loaded." << endl;
                
                continue;
            }

            cout << "Total frames: " << frames.size() << endl;

            for (size_t i = 0; i < frames.size(); ++i) {
                
                FrameData& f = frames[i];
                
                cout << "[" << i << "] " << f.name << " length=" << f.bins.size() << " range=[" << f.min_bin << ", " << f.max_bin << "]" << endl;

                cout << "First up to 10 bins: ";
                
                size_t limit = (f.bins.size() < 10 ? f.bins.size() : 10);
                
                for (size_t j = 0; j < limit; ++j) {
                    
                    cout << f.bins[j] << " ";
                }
                
                cout << endl << endl;
            }
        }

    
        else if (choice == 3) {
            int idx = choose_frame_index(frames);
            
            if (idx < 0) continue;

            FrameData& f = frames[idx];

            int c;
            size_t i;
            
            cout << "Enter symbol c: ";
            
            cin >> c;
            
            cout << "Enter index i: ";
            
            cin >> i;

            if (i >= f.bins.size()) {
                
                cout << "Index out of range." << endl;
                
                continue;
            }

            size_t r = f.wt->rank(c, i);
            
            cout << "rank(" << c << ", " << i << ") = " << r << endl;
        }

        
        else if (choice == 4) {
            
            int idx = choose_frame_index(frames);
            
            if (idx < 0) continue;

            FrameData& f = frames[idx];

            int c;
            size_t L, R;
            
            cout << "Enter symbol c: ";
            
            cin >> c;
            
            cout << "Enter L: ";
            
            cin >> L;
            
            cout << "Enter R: ";
            
            cin >> R;

            if (L > R || R >= f.bins.size()) {
                
                cout << "Invalid range." << endl;
                
                continue;
            }

            size_t count = count_symbol_in_interval(*(f.wt), c, L, R);
            
            cout << "Count = " << count << endl;
        }

        
        else if (choice == 5) {
            
            int idx = choose_frame_index(frames);
            
            if (idx < 0) continue;

            FrameData& f = frames[idx];

            int lo, hi;
            size_t L, R;

            cout << "Enter bin_low: ";
            
            cin >> lo;
            
            cout << "Enter bin_high: ";
            
            cin >> hi;
            
            cout << "Enter L: ";
            
            cin >> L;
            
            cout << "Enter R: ";
            
            cin >> R;

            if (L > R || R >= f.bins.size()) {
                
                cout << "Invalid range." << endl;
                
                continue;
            }

            size_t count = count_region_in_interval(*(f.wt), lo, hi, f.min_bin, f.max_bin, L, R);

            cout << "Region count = " << count << endl;

            double fraction = (R-L+1 > 0) ? (double)count / (double)(R-L+1) : 0.0;

            cout << "Fraction = " << fraction << endl;
        }

        
        else if (choice == 6) {
            
            if (frames.empty()) {
                
                cout << "No frames loaded." << endl;
                
                continue;
            }

            int lo, hi;
            
            cout << "Enter bin_low: ";
            
            cin >> lo;
            
            cout << "Enter bin_high: ";
            
            cin >> hi;

            cout << "Region time-series:" << endl;

            for (size_t fidx = 0; fidx < frames.size(); ++fidx) {
                
                FrameData& f = frames[fidx];
                
                size_t L = 0;
                
                size_t R = f.bins.size() - 1;

                size_t count = count_region_in_interval(*(f.wt), lo, hi, f.min_bin, f.max_bin, L, R);

                double fraction = (R-L+1 > 0) ? (double)count / (double)(R-L+1) : 0.0;

                cout << "[" << fidx << "] " << f.name << " fraction=" << fraction << endl;
            }
        }

        else {
            
            cout << "Unknown choice." << endl;
        }
    }

    return 0;
}
