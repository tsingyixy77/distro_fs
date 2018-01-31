#include "cs.h"
ostream& operator<<(ostream& os,ascii_message& msg){
    int type = static_cast<int>(msg.type_);
	os<<type<<msg.body_;
    return os;
}
istream& operator>>(istream& is,ascii_message& msg){
    int type ;
    is>>type>>msg.body_;
	msg.type_ = static_cast<MSG_TYPE>(type);
	return is;
}

MSG_TYPE ascii_message::get_type(){
    return type_;
}
void ascii_message::build_message(){
    //TODO:
}


