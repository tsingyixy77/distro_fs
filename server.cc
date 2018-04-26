#include <iostream>
#include <asio.hpp>
#include <string>
#include <vector>
#include <glog/logging.h>
#include <map>
#include "message.h"

using namespace asio::ip;
using namespace std;
#define info LOG(INFO)
class session : public std::enable_shared_from_this<session> {
    public:
        session(tcp::socket socket):socket_(std::move(socket)){
            info<<"starting to process....."<<endl;
        }

        void start(){
            receive_message();
        }
        void receive_message(){
            auto self(shared_from_this());
            socket_.async_read_some(
                    asio::buffer(data_,128),
                    [this,self](std::error_code ec,std::size_t){
                        if (!ec){
                            info<<"processing message ..."<<endl;
                            message msg(data_);
                            process_message(msg);
                        }
                        else {
            //                cout<<"error occurred"<<endl;
                        }
                        receive_message();
                    });
        }
        void process_message(message& msg){
            switch(msg.type()){
                case 0:
                case 1:
                case 2:
                case 3:
                default:
                    info<<"processing message "<<msg<<endl;
                    echo(msg.file());
                    break;
            }
        }
        void echo(string filename){
            //LOG(INFO)<<"requst of file "<<filename<<"from "<<socket_.remote_endpoint().address()<<std::endl;
            response(filename);
        }
        void response(string filename){
            auto self(shared_from_this());
            message msg(6,"received the request for file" + filename);
            sprintf(data_,"%s",msg.data());
            asio::async_write(socket_,
                    asio::buffer(data_),
                    [this,self](std::error_code ec,std::size_t){
                        if (!ec){
                            info<<"send successfully to "<< socket_.remote_endpoint().address()<<std::endl;
                            //do something if send successfully
                        }
                    });
        }
    private:
        tcp::socket socket_;
        char data_[128];

};
class FileSession : public std::enable_shared_from_this<FileSession>{
    public:
        FileSession(tcp::socket socket):socket_(std::move(socket)){
            info<<"(FS)starting to process....."<<endl;
        }
        void start(){
            receive_messages();
        }
        void receive_messages(){
            auto self(shared_from_this());
            socket_.async_read_some(
                    asio::buffer(data_,128),
                    [this,self](std::error_code ec,std::size_t){
                        if (!ec){
                            info<<"processing message ..."<<endl;
                            message msg(data_);
                            process_message(msg);
                        }
                        else {
                            cout<<"error occurred"<<endl;
                        }
                        receive_message();
                    });
        }
        void process_messages(message msg){
            int type = msg.type();
            switch (type) {
                case 0 : //ADD
                    fs_.add(msg.file(),socket_.remote_endpoint())
                    break;
                case 1 : //Query
                    auto ep = fs_.query(msg.file());
                    break;
                default : 
                    cout<<"waiting for more operation"
                    break;
            }
        }
    private:
        tcp::socket socket_;
        static FileService fs_;
};
template <class T=session>
class server{
    public:
        server(asio::io_service& io,const tcp::endpoint& ep):acceptor_(io,ep),socket_(io){
            do_accept();
        }

        void do_accept(){
            acceptor_.async_accept(socket_,
                    [this](std::error_code ec){
                        if (!ec){
                            info<<"create new connection w/i "<<socket_.remote_endpoint().address()<<":"<<socket_.remote_endpoint().port()<<endl;
                            std::make_shared<T>(std::move(socket_))->start();
                        }
                        do_accept();
                    });
        }
    private:
        tcp::acceptor acceptor_;
        tcp::socket socket_;
};
int main(){
    FLAGS_log_dir="./";
    google::InitGoogleLogging(__FILE__);
    asio::io_service io;
    tcp::endpoint ep(tcp::v4(),8081);
    server<> s(io,ep);
    io.run();
    return 0;
}


