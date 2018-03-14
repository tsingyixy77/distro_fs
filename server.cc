#include <iostream>
#include <asio.hpp>
#include <string>
#include <vector>
#include <map>
#include "message.h"

using namespace asio::ip;
using namespace std;

class FileService {
    public:
        void register_client(tcp::endpoint ep){
            clients.push_back(ep);
        }
        void create_new_file(string filename){
        }
        void query_file(string filename){
        }
        void read_file(string filename){
        }
    private:
        std::map<string,tcp::endpoint> files_to_client_;
        vector<tcp::endpoint> clients;
};
class session : public std::enable_shared_from_this<session> {
    public:
        session(tcp::socket socket):socket_(std::move(socket)){
			cout<<"starting to process....."<<endl;
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
							size_t p1 = asio::buffer_size(asio::buffer(data_));
							cout<<"processing message ..."<<endl;
							cout<<p1<<endl;
                            message msg(data_);
                            process_message(msg);
                        }
						else {
							cout<<"error occurred"<<endl;
						}
                    });
        }
        void process_message(message msg){
            switch(msg.type()){
                case 0:
                case 1:
                case 2:
                case 3:
                default:
                    echo(msg.file());
                    break;
            }
        }
        void echo(string filename){
            std::cout<<"requst of file "<<filename<<"from "<<socket_.remote_endpoint().address()<<std::endl;
            response(filename);
        }
        void response(string filename){
            auto self(shared_from_this());
            message msg(6,"received the request for file" + filename);
			memcpy(data_,msg.data(),msg.length());
            asio::async_write(socket_,
                    asio::buffer(data_),
                    [this,self](std::error_code ec,std::size_t){
                        if (!ec){
                            std::cout<<"send successfully to "<< socket_.remote_endpoint().address()<<std::endl;
                            //do something if send successfully
                        }
                    });
        }
    private:
        tcp::socket socket_;
        char data_[128];

};
class server{
    public:
        server(asio::io_service& io,const tcp::endpoint& ep):acceptor_(io,ep),socket_(io){
            do_accept();
        }

        void do_accept(){
            acceptor_.async_accept(socket_,
                    [this](std::error_code ec){
                        if (!ec){
                            std::make_shared<session>(std::move(socket_))->start();
                        }
                        do_accept();
                    });
        }
    private:
        tcp::acceptor acceptor_;
        tcp::socket socket_;
};
int main(){
	asio::io_service io;
	tcp::endpoint ep(tcp::v4(),8081);
	server s(io,ep);
	io.run();
    return 0;
}


