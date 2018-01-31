#include "cs.h"

ostream& operator<<(ostream& os,ascii_message& msg){
    int type;
    os<<type<<msg.body_;
    msg.type_ = static_cast<MSG_TYPE>(type);
    return os;
}
istream& operator<<(istream& is,ascii_message& msg){
    int type = static_cast<int>(msg.type_);
    is>>type>>msg.body_;
}

MSG_TYPE ascii_message::get_type() override{
    return type_;
}
MSG_TYPE ascii_message::build_message() override{
    //TODO:
}


