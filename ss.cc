#include "cs.h"

void server::accept(){
    shared_ptr<tcp::socket> sock(new tcp::socket(io_));
    acceptor_.async_accept(*sock,
        [=](const asio::error_code& ec){
            if (!ec){
                cout<<"connect from "<<sock->remote_endpoint().address()<<endl;

                sock->async_read_some(buf.prepare(64),
                    [=](const asio::error_code& ec,size_t n){
                        cout<<"read "<<n<<" bytes"<<endl;
                        buf.commit(n);

                        istream is(&buf);
                        ascii_message msg;
                        is>>msg;

                        if (msg.get_type() == MSG_TYPE::OTHER)
                            cout<<"read other things "<<msg<<endl;

                        sock->async_write_some(buf.data(),
                        [](const asio::error_code& ec, size_t n){
                            cout<<"send " << n << " bytes";
                        });
                    });
            accept();
        }
    });
}
int main(){
	server s;
	s.run();
	return 0;
}
