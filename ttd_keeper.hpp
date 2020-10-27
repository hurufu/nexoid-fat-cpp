#pragma once

#include "singleton.hpp"
#include "scapi_messages.hpp"

/** The purpose of this class is to guarantee TTD consitency on updates
 */
class TtdKeeper final : public Singleton<TtdKeeper> {
    friend class Singleton<TtdKeeper>;
    TtdKeeper(void) = default;
public:
    void update(const scapi::Event&);

    /** Exception handler, should be used on a boundary between C and C++
     */
    void handle_exception(void) noexcept;

    /** Should be called when wrong kind of response was received
     */
    void handle_bad_response(const scapi::Response&);

    enum NokReason fetch_nok_reason(void);
    enum TerminalErrorReason fetch_ter_reason(void);

    /** Unconditionaly clears TTD. Useful for testing
     */
    void reset(void) noexcept;
};
