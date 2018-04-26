#include <iostream>
#include <fstream>
#include <map>
//#include <glog.h>
#include <asio.hpp>


using namespace asio::ip;
using namespace std;

class FileService {
    public:
        void add(string filename, tcp::endpoint ep){
            if (files_.find(filename) != files_.end()){
                cout<<"the files has exist"<<endl;
            }
            else {
                files_[filename] = ep;
            }
        }
        tcp::endpoint query(string filename){
            if (files_.find(filename) == files_.end()){
                cout<<"the file not exist"<<endl;
            }
            else {
                return files_[filename];
            }
            return tcp::endpoint("0.0.0.0",-1);
        }
    private:
        map<string,tcp::endpoint> files_;
};
class DirectoryService {
    public:
        DirectoryService(string path):path_(path){}
        void write(string filename, string content){
            ofstream os(path_ + "/" + filename, ios::out);
            os<<content;
            os.flush();
        }
        string read(string filename){
            ifstream is(path_ + "/" + filename, ios::in);
            string content;
            is>>content;
            return content;
        }
    private:
        map<string,string> files_;
        string path_;
};
