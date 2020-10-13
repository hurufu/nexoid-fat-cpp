#pragma once

#include "scapi_messages.hpp"

#include <vector>

::std::vector<unsigned char>
encode(const ::scapi::socket::Request&);

::scapi::socket::Response
decode(const ::std::vector<unsigned char>&);
