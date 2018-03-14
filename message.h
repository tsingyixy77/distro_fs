#include <iostream>
#include <msgpack.hpp>
#include <glog/logging.h>

using namespace std;
namespace mp=msgpack;
const int max_message_length = 512;
class message {
    private:
        int type_;
        string filename_;
		int length_;
        char data_[512];
        void encode(){
            mp::sbuffer buf;
            mp::pack(buf,*this);
            sprintf(data_,"%4d%s",buf.size(),buf.data());
			length_ = buf.size()+4;
        }
        void decode(){
            char header[5];
            strncpy(header,data_,4);
            auto size = atoi(header);
			cout<<size<<" bytes"<<endl;
            mp::object_handle hdl = 
                mp::unpack(data_+4,size);
            *this = hdl.get().as<message>();
			length_ = size + 4;
        }
    public:
        message():length_(0){}
        message(int type,string filename)
            :type_(type),filename_(filename),length_(0){}
        message(char* data){
			sprintf(data_,"%s",data);
            decode();
        }

        const int type(){return type_;}
        const string file(){return filename_;}
        char* data(){
			encode();
            return data_;
        }
		int length(){
			return length_;
		}

        MSGPACK_DEFINE(type_,filename_);

};

/*
int main(){
	google::InitGoogleLogging("test");
    message m1(2,"/nfs/sjord9.home/hutan/thk/aaa");

	LOG(INFO)<<"encode message ..."<<endl;
    char* data = m1.data();

    message m2(data);
	LOG(INFO)<<"decode message ..."<<endl;
    cout<<m2.type()<<endl;
    cout<<m2.file()<<endl;

    return 0;
}
*/
