#pragma once

extern "C" {
#include <nexoid/types.h>
}

#include <sys/types.h> // Fixes missing include in the library header
#include "asn_codecs.h"

#include <iostream>
#include <type_traits>

const char* tostring(enum ProcedureResult);
const char* tostring(enum CardholderMessage);
const char* tostring(enum NokReason);
const char* tostring(enum TerminalErrorReason);
const char* tostring(enum LanguageAlpha2);
const char* tostring(enum ServiceId);
const char* tostring(enum CvdPresence);
const char* tostring(enum TransactionResult);
const char* tostring(enum asn_dec_rval_code_e);
const char* tostring(enum IdleEvent);

// TODO: Check if there is a way to avoid repetitios 'operator <<' overloading
static inline ::std::ostream& operator << (::std::ostream& os, const enum ProcedureResult& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum CardholderMessage& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum NokReason& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum TerminalErrorReason& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum LanguageAlpha2& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum ServiceId& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum CvdPresence& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum TransactionResult& x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum asn_dec_rval_code_e x) { return os << tostring(x); }
static inline ::std::ostream& operator << (::std::ostream& os, const enum IdleEvent x) { return os << tostring(x); }
