#pragma once

#include "scapi_messages.hpp"

#include <nngpp/buffer.h>
#include <vector>

::std::vector<unsigned char>
encode(const ::scapi::socket::Request&);

::scapi::socket::Response
decode(const ::std::vector<unsigned char>&);

::scapi::Response
decode_nng(const nng::buffer& buf);

nng::buffer
encode_nng(const ::scapi::Request& r);
