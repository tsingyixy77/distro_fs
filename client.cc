#include <iostream>
#include <asio.hpp>
#include <glog/logging.h>
#include "message.h"

using namespace std;
using namespace asio::ip;
#define info LOG(INFO)
class client {
    public:
        client():socket_(io_){}
        void connect(tcp::endpoint ep){
            info<<"started to connect"<<endl;
            socket_.connect(ep);
        }
        void send(message msg){
            char data[128];
            sprintf(data,"%s",msg.data());
            //info<<"send msg "<<msg<<endl;
            socket_.write_some(asio::buffer(data));
        }
        void receive(){
            char buf[128];
            socket_.read_some(asio::buffer(buf));
            message msg(buf);
            //info<<"received message "<<msg<<endl;
        }
        void close(){
            socket_.close();
        }
    private:
        asio::io_service io_;
        tcp::socket socket_;
};

int main(){
    google::InitGoogleLogging(__FILE__);
    client cc;
    tcp::endpoint ep(tcp::v4(),8081);
    cc.connect(ep);
    message msg(2,"this is a message object");
    cc.send(msg);
    cc.receive();
    cc.send(msg);
    cc.receive();
    cc.close();
    return 0;
}
