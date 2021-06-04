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
    void update(enum NokReason);
    void update(enum AuthorisationResponseCode);
    void update(enum TransactionResult);
    enum TerminalErrorReason update(enum TerminalErrorReason);
    std::optional<enum CvdPresence> update(enum CvdPresence);

    /** Exception handler, should be used on a boundary between C and C++
     */
    void handle_exception(const char* func = nullptr) noexcept;

    /** Should be called when wrong kind of response was received
     */
    void handle_bad_response(const scapi::Response&) noexcept;

    enum NokReason fetch_nok_reason(void);
    enum TerminalErrorReason fetch_ter_reason(void);
    enum ServiceId fetch_selected_service(void);
    union Iso639_1 fetch_selected_language(void);
    union bcd6 fetch_transaction_amount(void);
    union CurrencyAlpha3 fetch_transaction_currency_code_alpha3(void);
    union bcd fetch_transaction_currency_exponent(void);
    std::vector<std::string> fetch_missing_parameters(void);
    enum CtlssIndicatorStatus fetch_ctlss_indicator_status(void);

    /** Unconditionaly clears TTD. Useful for testing
     */
    void reset(void) noexcept;
};
