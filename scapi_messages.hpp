#pragma once
// TODO: This file should be generated

extern "C" {
#include "nexoid-ed/include/types.h"
}

#include <memory>
#include <variant>
#include <optional>
#include <vector>
#include <string>

namespace scapi {

    typedef ::std::variant<
        enum CvdPresence
      , struct cn2
    > CvdData;

    struct Nak {
        ::std::optional<enum ::NokReason> nokReason;
        ::std::optional<enum ::TerminalErrorReason> terminalErrorReason;
    };

    struct UpdateInterfaces {
        enum ::InterfaceStatus interfaceStatus;
    };

    struct PrintMessage {
        enum ::PrintMessage type;
        void* extraData;
    };

    struct LanguageSelection {
        union ::Iso639_1 selectedLanguage;
    };

    struct ServiceSelection {
        enum ::ServiceId serviceId;
    };

    struct ManualEntry {
        ::std::string pan;
        union ExpirationDate expirationDate;
        ::std::optional<CvdData> cvdData;
    };

    struct AmountEntry {
        union ::bcd6 totalAmount;
        ::std::optional<bool> minus;
        ::std::optional<::std::variant<
            bool, // confirmed
            union ::bcd6 // amount
        >> supplementaryAmount;
        ::std::optional<union ::bcd6> cashbackAmount;
    };

    struct CardInserted {
    };

    typedef ::std::variant<
        LanguageSelection
      , ServiceSelection
      , ManualEntry
      , ::std::monostate // Terminate
      , ::std::monostate // Reboot
      , AmountEntry
      , CardInserted
    > Event;

    typedef ::std::variant<
        enum ::CardholderMessage // msg
      , enum ::CardholderMessage // ssn
      , struct ::UiParameters // uiParametersForOutcome
      , struct ::UiParameters // uiParametersForRestart
      , struct ::UiParameters // uiParametersForCompletion
      , struct ::UiParameters // uiParametersForOutcomeStatusOnly
      , ::std::string // declineDisplayMessage
      , ::std::string // cardholderMessage - FIXME: See Scapi.asn1
      , ::std::string // applicationLabelDisplayed
      , struct ::string5 // commandKeyEnterLabel
      , struct ::string6 // commandKeyChangeApplicationLabel
      , struct ::string6 // commandKeyPinBypassLabel
      , union ::bcd6 // paymentAmount
      , union ::bcd6 // trxAmount
      , union ::bcd6 // cashbackAmount
      , union ::bcd6 // supplementaryAmount
      , union ::CurrencyAlpha3 // trxCurrencyAlpha3
      , enum ::ServiceId // selectedService
      , enum ::NokReason // nokReason
      , ::std::monostate // FIXME: Implement searchTrxResultList
      , union ::bcd6 // updatePreAuthTotalAmount
      , bool // minus
      , union ::bcd // trxCurrencyExponent
      , ::std::vector<::std::string> // missingParameters // non-nexo
      , enum CtlssIndicatorStatus // Contactless LEDs status
    > Interaction;

    typedef ::std::variant<
        std::string // PAN
      , enum ::CvdPresence // CVD presence
      , struct ::cn2 // CVD
      , void* // FIXME: Implement PIN
      , union ::ExpirationDate // Card expiry date
    > AckEntry;

    typedef ::std::variant<
        UpdateInterfaces // updateInterfaces
      , ::std::vector<Interaction> // output
      , PrintMessage // print
      , ::std::vector<Interaction> // entry
    > Request;

    typedef ::std::variant<
        Nak // nak
      , ::std::monostate // ack
      , ::std::vector<AckEntry> // ackEntry
    > Response;

    struct Notification {
        ::std::vector<Event> events;
    };

    namespace socket {

        struct RegistrationRequest {
        };

        struct NotificationRequest {
        };

        typedef ::std::variant<
            ::scapi::Request
          , RegistrationRequest
          , NotificationRequest
        > Request;

        typedef ::std::variant<
            ::scapi::Response
          , ::scapi::Notification
        > Response;

    }

    namespace nng {

        typedef long ExchangeId_t;

        struct Request {
            ::scapi::Request req;
            ExchangeId_t id;
        };

        struct Response {
            ::scapi::Response rsp;
            ExchangeId_t id;
        };

        struct Notification {
            ::scapi::Notification ntf;
            ExchangeId_t id;
        };

    }
}
