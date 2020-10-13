#pragma once
// TODO: This file should be generated

extern "C" {
#include <nexoid/types.h>
}

#include <memory>
#include <set>
#include <variant>
#include <optional>
#include <vector>

namespace scapi {

    struct Nak {
        enum NokReason nokReason;
        ::std::optional<enum TerminalErrorReason> terminalErrorReason;
    };

    struct UpdateInterfaces {
        enum InterfaceStatus interfaceStatus;
    };

    struct PrintMessage {
        enum Receipt type;
        void* extraData;
    };

    struct LanguageSelection {
    };

    struct ServiceSelection {
        enum ServiceId serviceId;
    };

    typedef ::std::variant<
        LanguageSelection
      , ServiceSelection
    > Event;

    typedef ::std::variant<
        enum CardholderMessage // msg
      , enum CardholderMessage // ssn
    > Interaction;

    typedef ::std::variant<
        UpdateInterfaces // updateInterfaces
      , ::std::set<Interaction> // output
      , PrintMessage // print
      , ::std::set<Interaction> // entry
    > Request;

    typedef ::std::variant<
        Nak // nak
      , ::std::monostate // ack
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

    }
}
