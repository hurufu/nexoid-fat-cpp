#include "utils.hpp"
#include <algorithm>

using namespace std;

stream_state::stream_state(ios_base& ios) : ref(ios), flags(ios.flags()) {
}

stream_state::~stream_state() {
    this->ref.flags(this->flags);
}

ostream& operator<<(ostream& os, as_hex v) {
    stream_state s(os);
    os << '[' << hex << setfill('0');
    ranges::for_each(v.buf, [&os](unsigned char b){os << setw(2) << static_cast<unsigned>(b);});
    os << ']';
    return os;
}
