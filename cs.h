#include <asio.hpp>
#include <iostream>

using namespace std;
using namespace asio;
using namespace asio::ip;

class server {
	public:
		server(short port=12345):acceptor_(io_,tcp::endpoint(tcp::v4(),port)){
			accept();
		}
		void run(){
			io_.run();
		}
		void accept(){
			shared_ptr<tcp::socket> sock(new tcp::socket(io_));
			acceptor_.async_accept(*sock,
								[=](const asio::error_code& ec){
									if (!ec){
										cout<<"connect from "<<sock->remote_endpoint().address()<<endl;

										sock->async_read_some(buffer(buf),
															[=](const asio::error_code& ec,size_t n){
																cout<<"read "<<n<<" bytes"<<endl;
																cout<<buf<<endl;

																sock->async_write_some(buffer(buf),
																[](const asio::error_code& ec, size_t n){
																	cout<<"send " << n << " bytes";
																});
															});
									accept();
								}
							});
		}
	private:
		io_service io_;
		tcp::acceptor acceptor_;
		char buf[1024];
};
class client {
	public:
		client(short port=12345):endpoint_(address::from_string("127.0.0.1"),port){
			sock_ = std::make_shared<tcp::socket>(io_);
			sock_->connect(endpoint_);
		}
		void read(){
			char buf[512];
			sock_->read_some(buffer(buf));
			cout<< "received from server [" << buf << "]" <<endl;
		}
		void write(string msg){
			sock_->write_some(buffer(msg));
		}
		void run(){
			io_.run();
		}
	private:
		io_service io_;
		tcp::endpoint endpoint_;
		shared_ptr<tcp::socket> sock_;
};
