#pragma once

#include "tostring.hpp"

#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <typeinfo>
#include <variant>

struct not_implemented : public std::logic_error {
    using std::logic_error::logic_error;
};

struct endec_error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct unmet_constraints : public endec_error {
    using endec_error::endec_error;
};

/** Signals that the same event arrived more then once in a sinle message.
 *
 *  This conditions isn't currently supported, and I doubt it will ever be,
 *  because both ignoring consecutive events and overwritting by the consecutive
 *  events isn't a good solution
 *
 *  @note The exception has can only by used for SCAP
 */
struct duplicated_event : public not_implemented {
    using not_implemented::not_implemented;
};

/**
 *  @todo: Expand encoding_error exception class
 */
struct encoding_error : public endec_error {
    using endec_error::endec_error;
};

struct decoding_error : public endec_error {
    /** Number of bytes already consumed by the decoder.
     */
    size_t consumed;

    decoding_error(const size_t c, const char* const d) :
        endec_error(d), consumed(c)
    {}

    /** Descriptive error code string.
     */
    virtual std::string get_code_str(void) const = 0;

    /** Data in an implementation defined printable format (eg plain or hex).
     *
     *  @return Full received data buffer, that wasn't properly decoded
     */
    virtual std::string get_data_str(void) const = 0;
};

/** Implements decoding_error for different encodings XER/BER or even APIs.
 *
 *  @todo: Template WithDecodingData should handle both binary and text data
 */
template <typename ErrorCode, typename DataType = std::string>
class WithDecodingData : public decoding_error {
    ErrorCode code;
    DataType data;
public:

    WithDecodingData(const ErrorCode e, const size_t c, const DataType d, const char* const desc) :
        decoding_error(c, desc), code(e), data(d)
    {}

    std::string get_code_str(void) const override {
        std::stringstream ss;
        ss << code;
        return ss.str();
    }

    std::string get_data_str(void) const override {
        return data;
    }
};

struct bad_mapping : public std::invalid_argument {
    long from;

    template <typename... Args>
    bad_mapping(const long f, Args&&... args) :
        std::invalid_argument(std::forward<Args>(args)...),
        from(f)
    {}
};

struct bad_variant_mapping : public std::invalid_argument {
    int index;
    const std::type_info& tinfo;

    template <typename... Variant>
    bad_variant_mapping(std::variant<Variant...> v, const char* const desc) :
        std::invalid_argument(desc),
        index(v.index()),
        tinfo(typeid(v))
    {}
};

struct null_argument : public std::invalid_argument {
    int null_arg_index;

    template <typename... Args>
    null_argument(const std::initializer_list<const void*> param, Args&&... args) :
        std::invalid_argument(std::forward<Args>(args)...),
        null_arg_index(std::distance(param.begin(), std::find(param.begin(), param.end(), nullptr)))
    {}
};

class out_of_memory : public std::bad_alloc {
    const char* desc;
public:

    /**
     *  @param d shall be a pointer to string literal
     */
    out_of_memory(const char* const d) : desc(d) {}

    virtual const char* what(void) const noexcept {
        return desc;
    }
};

template <typename... Args>
std::string make_desc(Args&&... args) {
    std::stringstream ss;
    ((ss << args), ...);
    return ss.str();
}
