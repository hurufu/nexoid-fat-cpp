#pragma once

#include "scapi_messages.hpp"

#include <vector>

::std::vector<unsigned char>
encode(const ::scapi::socket::Request&);

const ::scapi::socket::Response&
decode(const ::std::vector<unsigned char>&);
