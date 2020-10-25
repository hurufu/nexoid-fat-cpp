#pragma once

#include "scapi_messages.hpp"

#include <vector>

::std::vector<unsigned char>
encode(const ::scapi::socket::Request&);

::scapi::socket::Response
decode(const ::std::vector<unsigned char>&);

::scapi::Response
decode_nng(const ::std::vector<unsigned char>& buf);

::std::vector<unsigned char>
encode_nng(const ::scapi::Request& r);

::scapi::Notification
decode_nng_ntf(const ::std::vector<unsigned char>& buf);
