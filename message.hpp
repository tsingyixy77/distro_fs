#include <iostream>
#include <msgpack.hpp>

using namespace std;
namespace mp=msgpack;
class message {
    private:
        int type_;
        string filename_;
        char* data_;
        void encode(){
            mp::sbuffer buf;
            mp::pack(buf,*this);
            data_ = new char[buf.size()+5];
            sprintf(data_,"%4d%s",buf.size(),buf.data());
        }
        void decode(){
            char header[5];
            strncpy(header,data_,4);
            auto size = atoi(header);
            mp::object_handle hdl = 
                mp::unpack(data_+4,size);
            *this = hdl.get().as<message>();
        }
    public:
        message():data_(NULL){}
        message(int type,string filename)
            :type_(type),filename_(filename),data_(NULL){}
        message(char* data):data_(data){
            decode();
        }

        const int type(){return type_;}
        const string file(){return filename_;}
        char* data(){
            if (!data_)
                encode();
            return data_;
        }

        MSGPACK_DEFINE(type_,filename_);

};

/*
int main(){
    message m1(2,"/nfs/sjord9.home/hutan/thk/aaa");

    char* data = m1.data();

    message m2(data);
    cout<<m2.type()<<endl;
    cout<<m2.file()<<endl;

    return 0;
}
*/
