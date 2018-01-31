#include <asio.hpp>
#include <iostream>

using namespace std;
using namespace asio;
using namespace asio::ip;

enum class MSG_TYPE { ADD,DEL,WRITE,READ,REG,OTHER};
class message {
    public:
        virtual MSG_TYPE get_type() = 0;
        virtual void build_message() = 0;
};
class ascii_message{
    public:
        ascii_message(MSG_TYPE type,string body):type_(type),body_(body){}
        ascii_message(){}
        friend std::ostream& operator<<(std::ostream& os,ascii_message& msg);
        friend std::istream& operator>>(std::istream& is,ascii_message& msg);
		/*
        friend std::ostream& operator<<(std::ostream& os,ascii_message& msg){
			int type = static_cast<int>(msg.type_);
			os<<type<<msg.body_;
			return os;
		}
        friend std::istream& operator>>(std::istream& is,ascii_message& msg){
			int type;
			is>>type;
			is>>msg.body_;
			msg.type_ = static_cast<MSG_TYPE>(type);
			return is;
		}
		*/
        MSG_TYPE get_type();
        void build_message();
        void show(){
            cout<<"TYPE : ";
            switch(type_){
                case MSG_TYPE::ADD:
                    cout<<"ADD";
                    break;
                case MSG_TYPE::DEL:
                    cout<<"DEL";
                    break;
                case MSG_TYPE::WRITE:
                    cout<<"WRITE";
                    break;
                case MSG_TYPE::READ:
                    cout<<"READ";
                    break;
                case MSG_TYPE::REG:
                    cout<<"REG";
                    break;
                default:
                    cout<<"OTHER";
                    break;
            }
            cout<<"Body : "<< body_ <<endl;
        }
    private:
        MSG_TYPE type_;
        string body_;
};
class server {
	public:
		server(short port=12345):acceptor_(io_,tcp::endpoint(tcp::v4(),port)){
			accept();
		}
		void run(){
			io_.run();
		}
		void accept();
	private:
		io_service io_;
		tcp::acceptor acceptor_;
		//char buf[1024];
        asio::streambuf buf;
};
class client {
	public:
		client(short port=12345):endpoint_(address::from_string("127.0.0.1"),port){
			sock_ = std::make_shared<tcp::socket>(io_);
			sock_->connect(endpoint_);
		}
		void read(){
            asio::streambuf buf;
			size_t n = sock_->read_some(buf.prepare(64));
            buf.commit(n);
            std::ostream os(&buf);
            ascii_message msg;
            os<<msg;
            msg.show();
		}
		void write(string str){
            asio::streambuf sb;
            ostream os(&sb);
            ascii_message msg(MSG_TYPE::OTHER,str);
            os<<msg<<endl;
			sock_->write_some(sb.data());
		}
		void run(){
			io_.run();
		}
	private:
		io_service io_;
		tcp::endpoint endpoint_;
		shared_ptr<tcp::socket> sock_;
};
