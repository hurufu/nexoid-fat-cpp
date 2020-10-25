#pragma once
// TODO: This file should be generated

extern "C" {
#include <nexoid/types.h>
}

#include <memory>
#include <variant>
#include <optional>
#include <vector>

namespace scapi {

    struct Nak {
        enum ::NokReason nokReason;
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
    };

    typedef ::std::variant<
        LanguageSelection
      , ServiceSelection
      , ManualEntry
      , ::std::monostate // Terminate
      , ::std::monostate // Reboot
    > Event;

    typedef ::std::variant<
        enum ::CardholderMessage // msg
      , enum ::CardholderMessage // ssn
    > Interaction;

    typedef ::std::variant<
        UpdateInterfaces // updateInterfaces
      , ::std::vector<Interaction> // output
      , PrintMessage // print
      , ::std::vector<Interaction> // entry
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

        typedef ::std::variant<
            ::scapi::Response
          , ::scapi::Notification
        > Response;

    }
}
