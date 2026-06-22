#include<bits/stdc++.h>
#include <fstream>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <cstdio> 

using namespace std;

template <typename T>
class StatusLogger {
public:
    static void log(string msg, T val) {
        cout << "[LOG] " << msg << ": " << val << endl;
    }
};

// Requirement: Class for buffered reading & indexing
class Indexer {
    string verName;
    unordered_map<string, long long> freqMap;
public:
    Indexer(string name) : verName(name) {}

    void process(string path, int kb) {
        
        FILE* file = fopen(path.c_str(), "rb");
        if (!file) {
            throw runtime_error("ERROR: System cannot access file.");
        }

        
        size_t bufSize = (size_t)kb * 1024;
        unique_ptr<char[]> buffer(new char[bufSize]);
        string word;
        word.reserve(64);

        size_t bytesRead;
        while ((bytesRead = fread(buffer.get(), 1, bufSize, file)) > 0) {
            for (size_t i = 0; i < bytesRead; ++i) {
                unsigned char c = (unsigned char)buffer[i];
                if (isalnum(c)) {
                    word += (char)tolower(c);
                } else if (!word.empty()) {
                    freqMap[word]++;
                    word.clear();
                }
            }
        }
        if (!word.empty()) freqMap[word]++;
        
        fclose(file);
    }

    long long getWordCount(string w) {
        for(auto &c : w) c = tolower((unsigned char)c);
        return freqMap.count(w) ? freqMap[w] : 0;
    }
    const unordered_map<string, long long>& getMap() { 
        return freqMap; 
    }
    string getName() {
        return verName; 
    }
};


class BaseQuery { 
    public: virtual ~BaseQuery() {} 
    virtual void execute() = 0; 
};

class SingleWordQuery : public BaseQuery {
    shared_ptr<Indexer> idx; string word;
public:
    SingleWordQuery(shared_ptr<Indexer> i, string w) : idx(i), word(w) {}
    void execute() override {
        cout << "Version: " << idx->getName() << endl;
        cout << "Count" << ": " << idx->getWordCount(word) << endl;
    }
};

class TopKQuery : public BaseQuery {
    shared_ptr<Indexer> idx; int k;
public:
    TopKQuery(shared_ptr<Indexer> i, int topK) : idx(i), k(topK) {}
    void execute() override {
        cout << "Top-" << k << " words in version "<<idx->getName()<<":" << endl;
        auto m = idx->getMap();
        vector<pair<string, long long>> v(m.begin(), m.end());
        sort(v.begin(), v.end(), [](auto &a, auto &b){
            return a.second == b.second ? a.first < b.first : a.second > b.second;
        });
        for(int i=0; i<k && i<(int)v.size(); ++i) cout << v[i].first << " " << v[i].second << endl;
    }
};

// Requirement: Function Overloading
class Controller {
public:
    void perform(shared_ptr<Indexer> i, string type, string word, int k) {
        if (type == "word") { SingleWordQuery q(i, word); q.execute(); }
        else if (type == "top") { TopKQuery q(i, k); q.execute(); }
    }
    void perform(shared_ptr<Indexer> i1, shared_ptr<Indexer> i2, string word) {
        long long diff = i2->getWordCount(word) - i1->getWordCount(word);
        
        cout << "Difference (" << i2->getName() << " - " << i1->getName() << "): " << diff << endl;
    }
};

int main(int argc, char** argv) {
    try {
        
        
        string f1, f2, v1, v2, type, target;
        int buf = 256, k = 10;

        
        for (int i = 1; i < argc; ++i) {
            string s = argv[i];
            if ((s == "--file" || s == "--file1") && i + 1 < argc) f1 = argv[++i];
            else if (s == "--file2" && i + 1 < argc) f2 = argv[++i];
            else if ((s == "--version" || s == "--version1") && i + 1 < argc) v1 = argv[++i];
            else if (s == "--version2" && i + 1 < argc) v2 = argv[++i];
            else if (s == "--buffer" && i + 1 < argc) buf = stoi(argv[++i]);
            else if (s == "--query" && i + 1 < argc) type = argv[++i];
            else if (s == "--word" && i + 1 < argc) target = argv[++i];
            else if (s == "--top" && i + 1 < argc) k = stoi(argv[++i]);
        }

        if (type.empty()) throw runtime_error("Missing --query argument.");

        auto start = chrono::high_resolution_clock::now();
        Controller ctrl;

        if (type == "diff") {
            auto i1 = make_shared<Indexer>(v1); i1->process(f1, buf);
            auto i2 = make_shared<Indexer>(v2); i2->process(f2, buf);
            ctrl.perform(i1, i2, target);
        } else {
            auto i = make_shared<Indexer>(v1);
            i->process(f1, buf);
            ctrl.perform(i, type, target, k);
        }

        cout<<"Buffer size (KB): "<<buf<<endl;
        auto end = chrono::high_resolution_clock::now();
        cout<<"Execution time (s): " <<chrono::duration<double>(end-start).count()<<endl;

    } catch (const exception& e) {
        cout << "[ERROR] " << e.what() << endl;
        return 1;
    }
    return 0;
}