#include "tostring.hpp"

#include <stdexcept>

using namespace std;

const char*
tostring(const enum ProcedureResult r) {
    switch (r) {
        case PR_UNINITIALISED: return "PR_UNINITIALISED";
        case PR_NOT_IMPLEMENTED: return "PR_NOT_IMPLEMENTED";
        case PR_INTERNAL_INCONSISTENCY: return "PR_INTERNAL_INCONSISTENCY";
        case PR_REBOOT: return "PR_REBOOT";
        case PR_TERMINATE: return "PR_TERMINATE";
        case PR_SHUTDOWN: return "PR_SHUTDOWN";
        case PR_STARTUP_SEQUENCE: return "PR_STARTUP_SEQUENCE";
        case PR_CONTACT_TMS: return "PR_CONTACT_TMS";
        case PR_BAIL: return "PR_BAIL";
        case PR_OK: return "PR_OK";
        case PR_NOK: return "PR_NOK";
        case PR_NON_EMV: return "PR_NON_EMV";
        case PR_TAP_AGAIN: return "PR_TAP_AGAIN";
        case PR_END_APPLICATION: return "PR_END_APPLICATION";
        case PR_REFERENCE_ENTRY: return "PR_REFERENCE_ENTRY";
        case PR_ACCEPT: return "PR_ACCEPT";
        case PR_FOUND: return "PR_FOUND";
        case PR_NOT_FOUND: return "PR_NOT_FOUND";
        case PR_SUCCESSFUL: return "PR_SUCCESSFUL";
        case PR_NOT_SUCCESSFUL: return "PR_NOT_SUCCESSFUL";
        case PR_NOT_SUPPORTED: return "PR_NOT_SUPPORTED";
        case PR_MATCH: return "PR_MATCH";
        case PR_NO_MATCH: return "PR_NO_MATCH";
        case PR_DONE: return "PR_DONE";
        case PR_ONLINE: return "PR_ONLINE";
        case PR_UNABLE_TO_GO_ONLINE: return "PR_UNABLE_TO_GO_ONLINE";
        case PR_CONDITIONS_SATISFIED: return "PR_CONDITIONS_SATISFIED";
        case PR_CONDITIONS_NOT_SATISFIED: return "PR_CONDITIONS_NOT_SATISFIED";
        case PR_MANUAL_ENTRY: return "PR_MANUAL_ENTRY";
        case PR_REINITIALISE: return "PR_REINITIALISE";
        case PR_NEW_EVENT: return "PR_NEW_EVENT";
        case PR_RETRY_AFTER_CONTACTLESS: return "PR_RETRY_AFTER_CONTACTLESS";
        case PR_FALLBACK: return "PR_FALLBACK";
        case PR_PSE_UNSUCCESSFUL: return "PR_PSE_UNSUCCESSFUL";
        case PR_A: return "PR_A";
        case PR_B: return "PR_B";
        case PR_C: return "PR_C";
        case PR_D: return "PR_D";
        case PR_E: return "PR_E";
        case PR_PARTIAL_MATCH: return "PR_PARTIAL_MATCH";
        case PR_EXACT_MATCH: return "PR_EXACT_MATCH";
        case PR_SKIP: return "PR_SKIP";
        case PR_CVM_APPLICABLE: return "PR_CVM_APPLICABLE";
        case PR_CVM_NOT_APPLICABLE: return "PR_CVM_NOT_APPLICABLE";
        case PR_CVM_SUCCESSFUL: return "PR_CVM_SUCCESSFUL";
        case PR_CVM_UNSUCCESSFUL: return "PR_CVM_UNSUCCESSFUL";
        case PR_CVM_SUPPORTED: return "PR_CVM_SUPPORTED";
        case PR_CVM_NOT_SUPPORTED: return "PR_CVM_NOT_SUPPORTED";
        case PR_CVM_RETRY: return "PR_CVM_RETRY";
        case PR_MAX: return "PR_MAX";
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}

const char*
tostring(const enum CardholderMessage m) {
    switch (m) {
        case CRDHLDR_ACT_NONE: return "CRDHLDR_ACT_NONE";
        case CRDHLDR_EMV_APPROVED: return "✅ CRDHLDR_EMV_APPROVED";
        case CRDHLDR_EMV_VOICE_AUTH_REQUIRED: return "CRDHLDR_EMV_VOICE_AUTH_REQUIRED";
        case CRDHLDR_EMV_CARD_ERROR: return "CRDHLDR_EMV_CARD_ERROR";
        case CRDHLDR_EMV_DECLINED: return "❌ CRDHLDR_EMV_DECLINED";
        case CRDHLDR_EMV_INCORRECT_PIN: return "CRDHLDR_EMV_INCORRECT_PIN";
        case CRDHLDR_EMV_INSERT_CARD: return "CRDHLDR_EMV_INSERT_CARD";
        case CRDHLDR_EMV_PLEASE_WAIT: return "CRDHLDR_EMV_PLEASE_WAIT";
        case CRDHLDR_EMV_PROCESSING_ERROR: return "CRDHLDR_EMV_PROCESSING_ERROR";
        case CRDHLDR_EMV_REMOVE_CARD: return "CRDHLDR_EMV_REMOVE_CARD";
        case CRDHLDR_EMV_USE_CHIP_READER: return "CRDHLDR_EMV_USE_CHIP_READER";
        case CRDHLDR_EMV_USE_MAG_STRIPE: return "CRDHLDR_EMV_USE_MAG_STRIPE";
        case CRDHLDR_EMV_TRY_AGAIN: return "CRDHLDR_EMV_TRY_AGAIN";
        case CRDHLDR_MSG_WELCOME: return "CRDHLDR_MSG_WELCOME";
        case CRDHLDR_MSG_PRESENT_CARD: return "CRDHLDR_MSG_PRESENT_CARD";
        case CRDHLDR_MSG_PROCESSING: return "CRDHLDR_MSG_PROCESSING";
        case CRDHLDR_MSG_CARD_READ_OK_REMOVE_CARD: return "CRDHLDR_MSG_CARD_READ_OK_REMOVE_CARD";
        case CRDHLDR_MSG_PLEASE_INSERT_OR_SWIPE_CARD: return "CRDHLDR_MSG_PLEASE_INSERT_OR_SWIPE_CARD";
        case CRDHLDR_MSG_PLEASE_INSERT_ONE_CARD_ONLY: return "CRDHLDR_MSG_PLEASE_INSERT_ONE_CARD_ONLY";
        case CRDHLDR_MSG_APPROVED_PLEASE_SIGN: return "CRDHLDR_MSG_APPROVED_PLEASE_SIGN";
        case CRDHLDR_MSG_AUTHORISING_PLEASE_WAIT: return "CRDHLDR_MSG_AUTHORISING_PLEASE_WAIT";
        case CRDHLDR_MSG_INSERT_SWIPE_OR_TRY_ANOTHER_CARD: return "CRDHLDR_MSG_INSERT_SWIPE_OR_TRY_ANOTHER_CARD";
        case CRDHLDR_MSG_PLEASE_INSERT_CARD: return "CRDHLDR_MSG_PLEASE_INSERT_CARD";
        case CRDHLDR_ACT_CLEAR: return "CRDHLDR_ACT_CLEAR";
        case CRDHLDR_MSG_SEE_PHONE_FOR_INSTRUCTIONS: return "CRDHLDR_MSG_SEE_PHONE_FOR_INSTRUCTIONS";
        case CRDHLDR_MSG_PRESENT_CARD_AGAIN: return "CRDHLDR_MSG_PRESENT_CARD_AGAIN";
        case CRDHLDR_MSG_CARD_WRONG_WAY_OR_NO_CHIP: return "CRDHLDR_MSG_CARD_WRONG_WAY_OR_NO_CHIP";
        case CRDHLDR_MSG_READ_ERROR: return "CRDHLDR_MSG_READ_ERROR";
        case CRDHLDR_MSG_AMOUNT: return "CRDHLDR_MSG_AMOUNT";
        case CRDHLDR_MSG_MAX_AMOUNT: return "CRDHLDR_MSG_MAX_AMOUNT";
        case CRDHLDR_MSG_ENTER_PIN: return "CRDHLDR_MSG_ENTER_PIN";
        case CRDHLDR_MSG_ENTER: return "CRDHLDR_MSG_ENTER";
        case CRDHLDR_MSG_AMOUNT_AUTHORISED: return "CRDHLDR_MSG_AMOUNT_AUTHORISED";
        case CRDHLDR_MSG_LEFT_TO_BE_PAID: return "CRDHLDR_MSG_LEFT_TO_BE_PAID";
        case CRDHLDR_MSG_TRANSACTION_ABORTED: return "🚫 CRDHLDR_MSG_TRANSACTION_ABORTED";
        case CRDHLDR_MSG_PAYMENT_APPROVED_CASHBACK_DECLINED: return "CRDHLDR_MSG_PAYMENT_APPROVED_CASHBACK_DECLINED";
        case CRDHLDR_MSG_CHIP_ERROR_RE_ENTER_PIN: return "CRDHLDR_MSG_CHIP_ERROR_RE_ENTER_PIN";
        case CRDHLDR_MSG_PRESENT_CARD_OR_USE_MAGSTRIPE: return "CRDHLDR_MSG_PRESENT_CARD_OR_USE_MAGSTRIPE";
        case CRDHLDR_MSG_INSERT_OR_PRESENT_CARD: return "CRDHLDR_MSG_INSERT_OR_PRESENT_CARD";
        case CRDHLDR_MSG_INSERT_OR_SWIPE_CARD: return "CRDHLDR_MSG_INSERT_OR_SWIPE_CARD";
        case CRDHLDR_MSG_NO_PIN: return "CRDHLDR_MSG_NO_PIN";
        case CRDHLDR_MSG_DIFFERENT_CHOICE: return "CRDHLDR_MSG_DIFFERENT_CHOICE";
        case CRDHLDR_MSG_CHOOSE_APPLICATION: return "CRDHLDR_MSG_CHOOSE_APPLICATION";
        case CRDHLDR_MSG_AMOUNT_ESTIMATED: return "CRDHLDR_MSG_AMOUNT_ESTIMATED";
        case CRDHLDR_MSG_FINAL_AMOUNT: return "CRDHLDR_MSG_FINAL_AMOUNT";
        case CRDHLDR_MSG_AMOUNT_INCREMENT: return "CRDHLDR_MSG_AMOUNT_INCREMENT";
        case CRDHLDR_MSG_AMOUNT_DECREMENT: return "CRDHLDR_MSG_AMOUNT_DECREMENT";
        case CRDHLDR_MSG_PRINTER_OUT_OF_ORDER: return "CRDHLDR_MSG_PRINTER_OUT_OF_ORDER";
        case CRDHLDR_MSG_TIP: return "CRDHLDR_MSG_TIP";
        case CRDHLDR_MSG_CASHBACK: return "CRDHLDR_MSG_CASHBACK";
        case CRDHLDR_MSG_PAYMENT: return "CRDHLDR_MSG_PAYMENT";
        case CRDHLDR_MSG_TOTAL: return "CRDHLDR_MSG_TOTAL";
        case CRDHLDR_MSG_REQUEST_SIGNATURE: return "CRDHLDR_MSG_REQUEST_SIGNATURE";
        case CRDHLDR_MSG_RECEIPT_PRINTING_FAILED: return "❌ CRDHLDR_MSG_RECEIPT_PRINTING_FAILED";
        case CRDHLDR_MSG_TERMINAL_MANAGMENT_IN_PROGRESS: return "CRDHLDR_MSG_TERMINAL_MANAGMENT_IN_PROGRESS";
        case CRDHLDR_MSG_FORCE_TRANSACTION_APPROVAL: return "CRDHLDR_MSG_FORCE_TRANSACTION_APPROVAL";
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME: return "CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME";
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_RESTART: return "CRDHLDR_SRC_UI_PARAMETERS_FOR_RESTART";
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_COMPLETION: return "CRDHLDR_SRC_UI_PARAMETERS_FOR_COMPLETION";
        case CRDHLDR_SRC_DECLINE_DISPLAY_MESSAGE: return "CRDHLDR_SRC_DECLINE_DISPLAY_MESSAGE";
        case CRDHLDR_SRC_SELECTED_SERVICE: return "CRDHLDR_SRC_SELECTED_SERVICE";
        case CRDHLDR_SRC_CARDHOLDER_MESSAGE: return "CRDHLDR_SRC_CARDHOLDER_MESSAGE";
        case CRDHLDR_SRC_NOK_REASON: return "CRDHLDR_SRC_NOK_REASON";
        case CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME_STATUS_ONLY: return "CRDHLDR_SRC_UI_PARAMETERS_FOR_OUTCOME_STATUS_ONLY";
        case CRDHLDR_SRC_APPLICATION_LABEL_DISPLAYED: return "CRDHLDR_SRC_APPLICATION_LABEL_DISPLAYED";
        case CRDHLDR_SRC_PAYMENT_AMOUNT: return "CRDHLDR_SRC_PAYMENT_AMOUNT";
        case CRDHLDR_SRC_COMMAND_KEY_ENTER_LABEL: return "CRDHLDR_SRC_COMMAND_KEY_ENTER_LABEL";
        case CRDHLDR_SRC_TRX_CURRENCY_ALPHA3: return "CRDHLDR_SRC_TRX_CURRENCY_ALPHA3";
        case CRDHLDR_SRC_TRX_AMOUNT: return "CRDHLDR_SRC_TRX_AMOUNT";
        case CRDHLDR_SRC_COMMAND_KEY_CHANGE_APPLICATION_LABEL: return "CRDHLDR_SRC_COMMAND_KEY_CHANGE_APPLICATION_LABEL";
        case CRDHLDR_SRC_CASHBACK_AMOUNT: return "CRDHLDR_SRC_CASHBACK_AMOUNT";
        case CRDHLDR_SRC_COMMAND_KEY_PIN_BYPASS_LABEL: return "CRDHLDR_SRC_COMMAND_KEY_PIN_BYPASS_LABEL";
        case CRDHLDR_SRC_SUPPLEMENTARY_AMOUNT: return "CRDHLDR_SRC_SUPPLEMENTARY_AMOUNT";
        case CRDHLDR_SRC_SEARCH_TRX_RESULT_LIST: return "CRDHLDR_SRC_SEARCH_TRX_RESULT_LIST";
        case CRDHLDR_SRC_UPDATE_PRE_AUTH_TOTAL_AMOUNT: return "CRDHLDR_SRC_UPDATE_PRE_AUTH_TOTAL_AMOUNT";
        case CRDHLDR_SRC_MINUS: return "CRDHLDR_SRC_MINUS";
        case CRDHLDR_SSN_CARD_REMOVAL_REQUESTED: return "CRDHLDR_SSN_CARD_REMOVAL_REQUESTED";
        case CRDHLDR_SSN_CARD_REMOVED: return "CRDHLDR_SSN_CARD_REMOVED";
        case CRDHLDR_SSN_REQUEST_SIGNATURE: return "CRDHLDR_SSN_REQUEST_SIGNATURE";
        case CRDHLDR_SSN_RECEIPT_PRINTING_FAILED: return "CRDHLDR_SSN_RECEIPT_PRINTING_FAILED";
        case CRDHLDR_ENT_ENTER_PAN: return "CRDHLDR_ENT_ENTER_PAN";
        case CRDHLDR_ENT_ENTER_EXPIRY_DATE: return "CRDHLDR_ENT_ENTER_EXPIRY_DATE";
        case CRDHLDR_ENT_CVD_PRESENCE: return "CRDHLDR_ENT_CVD_PRESENCE";
        case CRDHLDR_ENT_CVD: return "CRDHLDR_ENT_CVD";
        case CRDHLDR_ENT_DCC_CONFIRMATION: return "CRDHLDR_ENT_DCC_CONFIRMATION";
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}

const char*
tostring(const enum NokReason n) {
    switch (n) {
        case N_NONE: return "N_NONE";
        case N_NOT_IMPLEMENTED: return "N_NOT_IMPLEMENTED";
        case N_ORIGINAL_TRX_NOT_FOUND: return "N_ORIGINAL_TRX_NOT_FOUND";
        case N_TECHNICAL_ERROR: return "N_TECHNICAL_ERROR";
        case N_MISSING_DATA: return "N_MISSING_DATA";
        case N_NO_PERMISSION: return "N_NO_PERMISSION";
        case N_CONFIGURATION_ERROR: return "N_CONFIGURATION_ERROR";
        case N_AMOUNT_ERROR: return "N_AMOUNT_ERROR";
        case N_KERNEL_ERROR: return "N_KERNEL_ERROR";
        case N_DATA_ERROR: return "N_DATA_ERROR";
        case N_NO_CARD_INSERTED: return "N_NO_CARD_INSERTED";
        case N_CANCELLED: return "N_CANCELLED";
        case N_CARD_MISSING: return "N_CARD_MISSING";
        case N_NO_PROFILE: return "N_NO_PROFILE";
        case N_ABORTED: return "N_ABORTED";
        case N_TIMEOUT: return "N_TIMEOUT";
        case N_CONF_ERROR: return "N_CONF_ERROR";
        case N_FALLBACK_PROHIBITED: return "N_FALLBACK_PROHIBITED";
        case N_TECHNOLOGY_NOT_SUPPORTED: return "N_TECHNOLOGY_NOT_SUPPORTED";
        case N_CHIP_ERROR: return "N_CHIP_ERROR";
        case N_GPO6985: return "N_GPO6985";
        case N_CARD_BLOCKED: return "N_CARD_BLOCKED";
        case N_EMPTY_LIST: return "N_EMPTY_LIST";
        case N_MAX:
            break;
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}

const char*
tostring(const enum TerminalErrorReason t) {
    switch (t) {
        case TE_NONE: return "TE_NONE";
        case TE_HARDWARE_ERROR: return "TE_HARDWARE_ERROR";
        case TE_MEMORY_FAILURE: return "TE_MEMORY_FAILURE";
        case TE_CONFIGURATION_ERROR: return "TE_CONFIGURATION_ERROR";
        case TE_CRYPTOGRAPHIC_KEYS_MISSING: return "TE_CRYPTOGRAPHIC_KEYS_MISSING";
        case TE_LOG_LIMIT_EXCEEDED: return "TE_LOG_LIMIT_EXCEEDED";
        case TE_COMMUNICATION_ERROR: return "TE_COMMUNICATION_ERROR";
        case TE_UNSPECIFIED: return "TE_UNSPECIFIED";
        case TE_NEXO_FAST_FAILURE: return "TE_NEXO_FAST_FAILURE";
        case TE_INTERACTION_ERROR: return "TE_INTERACTION_ERROR";
        case TE_OVERSPEND: return "TE_OVERSPEND";
        case TER_NOT_IMPLEMENTED: return "TER_NOT_IMPLEMENTED";
        case TER_INTERFACE_CONTRACT_VIOLATION: return "TER_INTERFACE_CONTRACT_VIOLATION";
        case TER_INTERNAL_ERROR: return "TER_INTERNAL_ERROR";
        case TER_OS_ERROR: return "TER_OS_ERROR";
        case TER_TIMEOUT: return "TER_TIMEOUT";
        case TER_MAX:
            break;
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}

const char*
tostring(const enum LanguageAlpha2 a) {
    switch (a) {
        case LANG_AFAR: return "LANG_AFAR";
        case LANG_ABKHAZIAN: return "LANG_ABKHAZIAN";
        case LANG_AFRIKAANS: return "LANG_AFRIKAANS";
        case LANG_AKAN: return "LANG_AKAN";
        case LANG_AMHARIC: return "LANG_AMHARIC";
        case LANG_ARAGONESE: return "LANG_ARAGONESE";
        case LANG_ARABIC: return "LANG_ARABIC";
        case LANG_ASSAMESE: return "LANG_ASSAMESE";
        case LANG_AVAR: return "LANG_AVAR";
        case LANG_AYMARA: return "LANG_AYMARA";
        case LANG_AZERBAIJANI: return "LANG_AZERBAIJANI";
        case LANG_BASHKIR: return "LANG_BASHKIR";
        case LANG_BELARUSIAN: return "LANG_BELARUSIAN";
        case LANG_BULGARIAN: return "LANG_BULGARIAN";
        case LANG_BIHARI: return "LANG_BIHARI";
        case LANG_BISLAMA: return "LANG_BISLAMA";
        case LANG_BAMBARA: return "LANG_BAMBARA";
        case LANG_BENGALI: return "LANG_BENGALI";
        case LANG_TIBETAN: return "LANG_TIBETAN";
        case LANG_BRETON: return "LANG_BRETON";
        case LANG_BOSNIAN: return "LANG_BOSNIAN";
        case LANG_CATALAN: return "LANG_CATALAN";
        case LANG_CHECHEN: return "LANG_CHECHEN";
        case LANG_CHAMORRO: return "LANG_CHAMORRO";
        case LANG_CORSICAN: return "LANG_CORSICAN";
        case LANG_CREE: return "LANG_CREE";
        case LANG_CZECH: return "LANG_CZECH";
        case LANG_OLD_CHURCH_SLAVONIC: return "LANG_OLD_CHURCH_SLAVONIC";
        case LANG_CHUVASH: return "LANG_CHUVASH";
        case LANG_WELSH: return "LANG_WELSH";
        case LANG_DANISH: return "LANG_DANISH";
        case LANG_GERMAN: return "LANG_GERMAN";
        case LANG_DIVEHI: return "LANG_DIVEHI";
        case LANG_DZONGKHA: return "LANG_DZONGKHA";
        case LANG_EWE: return "LANG_EWE";
        case LANG_GREEK: return "LANG_GREEK";
        case LANG_ENGLISH: return "LANG_ENGLISH";
        case LANG_ESPERANTO: return "LANG_ESPERANTO";
        case LANG_SPANISH: return "LANG_SPANISH";
        case LANG_ESTONIAN: return "LANG_ESTONIAN";
        case LANG_BASQUE: return "LANG_BASQUE";
        case LANG_PERSIAN: return "LANG_PERSIAN";
        case LANG_PEUL: return "LANG_PEUL";
        case LANG_FINNISH: return "LANG_FINNISH";
        case LANG_FIJIAN: return "LANG_FIJIAN";
        case LANG_FAROESE: return "LANG_FAROESE";
        case LANG_FRENCH: return "LANG_FRENCH";
        case LANG_WEST_FRISIAN: return "LANG_WEST_FRISIAN";
        case LANG_IRISH: return "LANG_IRISH";
        case LANG_SCOTTISH_GAELIC: return "LANG_SCOTTISH_GAELIC";
        case LANG_GALICIAN: return "LANG_GALICIAN";
        case LANG_GUARANI: return "LANG_GUARANI";
        case LANG_GUJARATI: return "LANG_GUJARATI";
        case LANG_MANX: return "LANG_MANX";
        case LANG_HAUSA: return "LANG_HAUSA";
        case LANG_HEBREW: return "LANG_HEBREW";
        case LANG_HINDI: return "LANG_HINDI";
        case LANG_HIRI_MOTU: return "LANG_HIRI_MOTU";
        case LANG_CROATIAN: return "LANG_CROATIAN";
        case LANG_HAITIAN: return "LANG_HAITIAN";
        case LANG_HUNGARIAN: return "LANG_HUNGARIAN";
        case LANG_ARMENIAN: return "LANG_ARMENIAN";
        case LANG_HERERO: return "LANG_HERERO";
        case LANG_INTERLINGUA: return "LANG_INTERLINGUA";
        case LANG_INDONESIAN: return "LANG_INDONESIAN";
        case LANG_INTERLINGUE: return "LANG_INTERLINGUE";
        case LANG_IGBO: return "LANG_IGBO";
        case LANG_SICHUAN_YI: return "LANG_SICHUAN_YI";
        case LANG_INUPIAK: return "LANG_INUPIAK";
        case LANG_IDO: return "LANG_IDO";
        case LANG_ICELANDIC: return "LANG_ICELANDIC";
        case LANG_ITALIAN: return "LANG_ITALIAN";
        case LANG_INUKTITUT: return "LANG_INUKTITUT";
        case LANG_JAPANESE: return "LANG_JAPANESE";
        case LANG_JAVANESE: return "LANG_JAVANESE";
        case LANG_GEORGIAN: return "LANG_GEORGIAN";
        case LANG_KONGO: return "LANG_KONGO";
        case LANG_KIKUYU: return "LANG_KIKUYU";
        case LANG_KUANYAMA: return "LANG_KUANYAMA";
        case LANG_KAZAKH: return "LANG_KAZAKH";
        case LANG_GREENLANDIC: return "LANG_GREENLANDIC";
        case LANG_CAMBODIAN: return "LANG_CAMBODIAN";
        case LANG_KANNADA: return "LANG_KANNADA";
        case LANG_KOREAN: return "LANG_KOREAN";
        case LANG_KANURI: return "LANG_KANURI";
        case LANG_KASHMIRI: return "LANG_KASHMIRI";
        case LANG_KURDISH: return "LANG_KURDISH";
        case LANG_KOMI: return "LANG_KOMI";
        case LANG_CORNISH: return "LANG_CORNISH";
        case LANG_KIRGHIZ: return "LANG_KIRGHIZ";
        case LANG_LATIN: return "LANG_LATIN";
        case LANG_LUXEMBOURGISH: return "LANG_LUXEMBOURGISH";
        case LANG_GANDA: return "LANG_GANDA";
        case LANG_LIMBURGIAN: return "LANG_LIMBURGIAN";
        case LANG_LINGALA: return "LANG_LINGALA";
        case LANG_LAOTIAN: return "LANG_LAOTIAN";
        case LANG_LITHUANIAN: return "LANG_LITHUANIAN";
        case LANG_LUBA_KATANGA: return "LANG_LUBA_KATANGA";
        case LANG_LATVIAN: return "LANG_LATVIAN";
        case LANG_MALAGASY: return "LANG_MALAGASY";
        case LANG_MARSHALLESE: return "LANG_MARSHALLESE";
        case LANG_MAORI: return "LANG_MAORI";
        case LANG_MACEDONIAN: return "LANG_MACEDONIAN";
        case LANG_MALAYALAM: return "LANG_MALAYALAM";
        case LANG_MONGOLIAN: return "LANG_MONGOLIAN";
        case LANG_MOLDOVAN: return "LANG_MOLDOVAN";
        case LANG_MARATHI: return "LANG_MARATHI";
        case LANG_MALAY: return "LANG_MALAY";
        case LANG_MALTESE: return "LANG_MALTESE";
        case LANG_BURMESE: return "LANG_BURMESE";
        case LANG_NAURUAN: return "LANG_NAURUAN";
        case LANG_NORWEGIAN: return "LANG_NORWEGIAN";
        case LANG_NORWEGIAN_BOKMAL: return "LANG_NORWEGIAN_BOKMAL";
        case LANG_NORWEGIAN_NYNORSK: return "LANG_NORWEGIAN_NYNORSK";
        case LANG_NEPALI: return "LANG_NEPALI";
        case LANG_NDONGA: return "LANG_NDONGA";
        case LANG_DUTCH: return "LANG_DUTCH";
        case LANG_NORTH_NDEBELE: return "LANG_NORTH_NDEBELE";
        case LANG_SOUTH_NDEBELE: return "LANG_SOUTH_NDEBELE";
        case LANG_NAVAJO: return "LANG_NAVAJO";
        case LANG_CHICHEWA: return "LANG_CHICHEWA";
        case LANG_OCCITAN: return "LANG_OCCITAN";
        case LANG_OJIBWA: return "LANG_OJIBWA";
        case LANG_OROMO: return "LANG_OROMO";
        case LANG_ORIYA: return "LANG_ORIYA";
        case LANG_OSSETIAN: return "LANG_OSSETIAN";
        case LANG_PANJABI: return "LANG_PANJABI";
        case LANG_PALI: return "LANG_PALI";
        case LANG_POLISH: return "LANG_POLISH";
        case LANG_PASHTO: return "LANG_PASHTO";
        case LANG_PORTUGUESE: return "LANG_PORTUGUESE";
        case LANG_QUECHUA: return "LANG_QUECHUA";
        case LANG_RAETO_ROMANCE: return "LANG_RAETO_ROMANCE";
        case LANG_KIRUNDI: return "LANG_KIRUNDI";
        case LANG_ROMANIAN: return "LANG_ROMANIAN";
        case LANG_RUSSIAN: return "LANG_RUSSIAN";
        case LANG_RWANDI: return "LANG_RWANDI";
        case LANG_SANSKRIT: return "LANG_SANSKRIT";
        case LANG_SARDINIAN: return "LANG_SARDINIAN";
        case LANG_SINDHI: return "LANG_SINDHI";
        case LANG_NORTHERN_SAMI: return "LANG_NORTHERN_SAMI";
        case LANG_SANGO: return "LANG_SANGO";
        case LANG_SERBO_CROATIAN: return "LANG_SERBO_CROATIAN";
        case LANG_SINHALESE: return "LANG_SINHALESE";
        case LANG_SLOVAK: return "LANG_SLOVAK";
        case LANG_SLOVENIAN: return "LANG_SLOVENIAN";
        case LANG_SAMOAN: return "LANG_SAMOAN";
        case LANG_SHONA: return "LANG_SHONA";
        case LANG_SOMALIA: return "LANG_SOMALIA";
        case LANG_ALBANIAN: return "LANG_ALBANIAN";
        case LANG_SERBIAN: return "LANG_SERBIAN";
        case LANG_SWATI: return "LANG_SWATI";
        case LANG_SOUTHERN_SOTHO: return "LANG_SOUTHERN_SOTHO";
        case LANG_SUNDANESE: return "LANG_SUNDANESE";
        case LANG_SWEDISH: return "LANG_SWEDISH";
        case LANG_SWAHILI: return "LANG_SWAHILI";
        case LANG_TAMIL: return "LANG_TAMIL";
        case LANG_TELUGU: return "LANG_TELUGU";
        case LANG_TAJIK: return "LANG_TAJIK";
        case LANG_THAI: return "LANG_THAI";
        case LANG_TIGRINYA: return "LANG_TIGRINYA";
        case LANG_TURKMEN: return "LANG_TURKMEN";
        case LANG_TAGALOG: return "LANG_TAGALOG";
        case LANG_TSWANA: return "LANG_TSWANA";
        case LANG_TONGA: return "LANG_TONGA";
        case LANG_TURKISH: return "LANG_TURKISH";
        case LANG_TSONGA: return "LANG_TSONGA";
        case LANG_TATAR: return "LANG_TATAR";
        case LANG_TWI: return "LANG_TWI";
        case LANG_TAHITIAN: return "LANG_TAHITIAN";
        case LANG_UYGHUR: return "LANG_UYGHUR";
        case LANG_UKRAINIAN: return "LANG_UKRAINIAN";
        case LANG_URDU: return "LANG_URDU";
        case LANG_UZBEK: return "LANG_UZBEK";
        case LANG_VENDA: return "LANG_VENDA";
        case LANG_VIETNAMESE: return "LANG_VIETNAMESE";
        case LANG_VOLAPUK: return "LANG_VOLAPUK";
        case LANG_WALLOON: return "LANG_WALLOON";
        case LANG_WOLOF: return "LANG_WOLOF";
        case LANG_XHOSA: return "LANG_XHOSA";
        case LANG_YIDDISH: return "LANG_YIDDISH";
        case LANG_YORUBA: return "LANG_YORUBA";
        case LANG_ZHUANG: return "LANG_ZHUANG";
        case LANG_CHINESE: return "LANG_CHINESE";
        case LANG_ZULU: return "LANG_ZULU";
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}

const char*
tostring(const enum ServiceId s) {
    switch (s) {
        case S_NONE: return "S_NONE";
        case S_PAYMENT: return "S_PAYMENT";
        case S_REFUND: return "S_REFUND";
        case S_CANCELLATION: return "S_CANCELLATION";
        case S_PRE_AUTH: return "S_PRE_AUTH";
        case S_UPDATE_PRE_AUTH: return "S_UPDATE_PRE_AUTH";
        case S_PAYMENT_COMPLETION: return "S_PAYMENT_COMPLETION";
        case S_CASH_ADVANCE: return "S_CASH_ADVANCE";
        case S_DEFFERED_PAYMENT: return "S_DEFFERED_PAYMENT";
        case S_DEFFERED_PAYMENT_COMPLETION: return "S_DEFFERED_PAYMENT_COMPLETION";
        case S_VOICE_AUTHORISATION: return "S_VOICE_AUTHORISATION";
        case S_CARDHOLDER_DETECTION: return "S_CARDHOLDER_DETECTION";
        case S_CARD_VALIDITY_CHECK: return "S_CARD_VALIDITY_CHECK";
        case S_NO_SHOW: return "S_NO_SHOW";
        case S_MAX: return "S_MAX";
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}

const char*
tostring(const enum CvdPresence p) {
    switch (p) {
        case CVD_ENTRY_BYPASSED: return "CVD_ENTRY_BYPASSED";
        case CVD_PRESENT: return "CVD_PRESENT";
        case CVD_ILLEGIBLE: return "CVD_ILLEGIBLE";
        case CVD_NOT_PRESENT: return "CVD_NOT_PRESENT";
    }
    throw runtime_error(__PRETTY_FUNCTION__);
}
