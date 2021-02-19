#include "nexoid-ed/include/gtd.h"

struct PermanentTerminalSpecificData e0 = {
    .ifdSerialNumber = &(struct string8){ .s = "123456" },
    .commandKeyBypassPinLabel = &(struct string6){ .s = "NO PIN" },
    .commandKeyClearLabel = { .s = "CLEAR" },
    .commandKeyEnterLabel = { .s = "ENTER" },
    .commandKeyScrollLabel = { .s = "SCROL" },
    .commandKeyChangeApplicationLabel = { .s = "C APP" }
};

struct TerminalSpecificData e1 = {
    .terminalType.e = ATTENDED_ONLINE_ONLY_OPERATED_BY_MERCHANT,
    .terminalSettings = {
        .merchantReceiptPrintedFirst = 1,
        .cardholderReceipt = {
            .printApproved = 1,
            .printDeclined = 1,
            .printVoiceAuth = 1
        },
        .merchantReceipt = {
            .printApproved = 1,
            .printDeclined = 1,
            .printVoiceAuth = 1
        },
    },
    .terminalCapabilities = {
        // FIXME: It looks like that cardDataInputCapability isn't used in nexo
        .cardDataInputCapability = {
            .icWithContacts = 1,
            .magneticStripe = 1,
            .manualKeyEntry = 1
        },
        .cvmCapability = {
            .noCvmRequired = 1,
            .signature = 1,
            .plainTextPinForIccVerification = 1
        },
        .securityCapability = {
            .sda = 1,
            .cardCapture = 1
        }
    },
    .additionalTerminalCapabilities = {
        .transactionType = {
        },
        .terminalDataInput = {
            .numericKeys = 1,
            .alphabeticAndSpecialCharactersKeys = 1
        },
        // NOTE: It looks like during Output Interaction terminalDataOutput
        // isn't checked by nexo spec
        .terminalDataOutput = {
            .printAttendant = 1,
            .printCardholder = 1,
            .displayAttendant = 1,
            .displayCardholder = 1,
        }
    },
    .defaultCardService = S_CARD_VALIDITY_CHECK,
    .configuredServices = {
        .cardValidityCheck = 1
    },
    .terminalTransactionCurrencyExponent = { .raw = 0x02},
    .currencyCodeAlpha3 = { .Code = Currency_PLN },
    .terminalSupportedLanguageList = {
        .l = 5,
        .a = {
            { .e = LANG_POLISH },
            { .e = LANG_ENGLISH },
            { .e = LANG_DANISH },
            { .e = LANG_CZECH },
            { .e = LANG_RUSSIAN },
        }
    },
    .cardholderDefaultLanguage = { .e = LANG_POLISH }
};

struct ApplicationProfileSelectionTable* e2 = &(struct ApplicationProfileSelectionTable){
    .applicationProfileAid = {
        .l_raw = 6,
        .raw = { 0xA0, 0x00, 0x00, 0x00, 0x00, 0x01 }
    },
    .profileNumber = 0x01,
    .supportedServices = {
        .payment = 1,
        .cardValidityCheck = 1
    },

    .applicationProfileAcquirerNumber = &(bcd_t){ 0x01 },
    .aidMatchCriteria = &(bool){ false },

    .next = &(struct ApplicationProfileSelectionTable){
        .applicationProfileAid = {
            .l_raw = 6,
            .raw = { 0xA0, 0x00, 0x00, 0x00, 0x00 }
        },
        .profileNumber = 0x02,
        .supportedServices = {
            .payment = 1,
            .cardValidityCheck = 1
        },

        .next = NULL
    }
};

struct CaPublicKeyTable* e3 = &(struct CaPublicKeyTable){
    .entry = {
        .rid.v = { 0xA0, 0x00, 0x00, 0x00, 0x00 },
        .index = 0x44,
        .hashAlgorithmIndicator = CA_HASH_ALGORITHM_SHA1,
        .publicKeyAlgorithmIndicator = CA_PUB_KEY_ALGORITHM_RSA,
        .caPublicKeyModulus = {
            .l = 112,
            .r = {
                0x86, 0xDC, 0x79, 0x3C, 0x84, 0x3D, 0x4C, 0x4D, 0x0B, 0x4B, 0x3C, 0xB9, 0x20, 0x11, 0xAA, 0x2F,
                0x38, 0x65, 0x2A, 0xAA, 0xF4, 0x2F, 0xD5, 0x5D, 0x33, 0x9B, 0xF4, 0x8D, 0xE3, 0x11, 0x8A, 0xB3,
                0x08, 0x31, 0xA3, 0x45, 0x1A, 0x66, 0x1E, 0xBA, 0x6A, 0x02, 0xF8, 0xAE, 0xC9, 0x8B, 0x65, 0x8B,
                0xE4, 0x1D, 0xE5, 0x81, 0xB4, 0x6C, 0x41, 0x94, 0x8C, 0xF1, 0xF6, 0xD7, 0xE9, 0x2D, 0x5D, 0xAB,
                0xA0, 0x92, 0x68, 0xB8, 0x71, 0xC5, 0xBB, 0x01, 0xA8, 0x6C, 0x82, 0x90, 0xA1, 0x33, 0xA6, 0x0D,
                0xAC, 0x05, 0x75, 0xA7, 0x9C, 0xD1, 0x57, 0x69, 0x82, 0xA9, 0x4D, 0xC1, 0x48, 0x0D, 0xE8, 0x6E,
                0x6C, 0x0B, 0xBE, 0xD1, 0x34, 0x36, 0xDF, 0x95, 0x2B, 0xDB, 0xEC, 0x90, 0xC9, 0x35, 0xA1, 0xEB
            }
        },
        .caPublicKeyExponent = {
            .l = 1,
            .r = { 0x03 }
        },
        .kcv = &(struct KeyCheckValue){
            .c = {
                0x86, 0xDC, 0x79, 0x3C, 0x84, 0x3D, 0x4C, 0x4D, 0x0B, 0x4B, 0x3C, 0xB9, 0x20, 0x11, 0xAA, 0x2F,
                0x38, 0x65
            }
        }
    },
    .next = NULL
};

struct ServiceSettingsTable e4 = {
    .s = 3,
    .a = {
        {
            .serviceIdentifier = S_CARD_VALIDITY_CHECK,
            .cardholderInitialMessage = CRDHLDR_MSG_WELCOME,
            .serviceSettings = {
                .msrProcessingModeSupported = 1,
                .ServiceProtected = 1,
                .contactProcessingModeSupported = 1,
                .contactChipHasPriority = 1,
                .manualEntryProcessingModeSupported = 1,
            },
            .allowedServiceStartEvents = {
                .manualEntry = 1,
                .cardInserted = 1,
                .cardSwiped = 1
            },
            .minimumServiceStartConditions = {
                .s = 3,
                .a = {
                    { .cardInserted = 1 }
                  , { .cardSwiped = 1 }
                  , { .manualEntry = 1 }
                }
            }
        },
        {
            .serviceIdentifier = S_PAYMENT,
            .cardholderInitialMessage = CRDHLDR_MSG_WELCOME,
            .serviceSettings = {
                .msrProcessingModeSupported = 1,
                .ServiceProtected = 0,
                .contactProcessingModeSupported = 1,
                .contactChipHasPriority = 1,
                .manualEntryProcessingModeSupported = 1,
            },
            .allowedServiceStartEvents = {
                .amountEntry = 1,
                .manualEntry = 1,
                .cardInserted = 1,
                .cardSwiped = 1,
            },
            .minimumServiceStartConditions = {
                .s = 3,
                .a = {
                    { .amountEntry = 1, .manualEntry = 1 }
                  , { .amountEntry = 1, .cardInserted = 1 }
                  , { .amountEntry = 1, .cardSwiped = 1 }
                }
            }
        },
        {
            .serviceIdentifier = S_UPDATE_PRE_AUTH,
            .cardholderInitialMessage = CRDHLDR_MSG_WELCOME,
            .serviceSettings = {
                .ServiceProtected = 1,
            },
            .allowedServiceStartEvents = {
                .amountEntry = 1,
                .referenceEntry = 1
            },
            .minimumServiceStartConditions = {
                .s = 1,
                .a = {
                    { .amountEntry = 1, .referenceEntry = 1 }
                }
            }
        }
    }
};

struct ApplicationProfileList* e6 = &(struct ApplicationProfileList){
    .entry = {
        .profileNumber = 0x01,
        .acquirerNumber = 0x01,
        .acquirerIdentifier = { .v = { 0x45, 0x00, 0x00, 0x00, 0x00, 0x01 } },
        .terminalCountryCode = { .bcd.v = { 0x06, 0x16 } },
        .terminalFloorLimit = &(union bcd6){ .v = { 0x00, 0x00, 0x00, 0x00, 0x10, 0x0 } },
        .cvcDefaultAmount = &(union bcd6){ .v = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} },
        .tacDefault = &(union TerminalVerificationResults){ },
        .tacDenial = &(union TerminalVerificationResults){ },
        .tacOnline = &(union TerminalVerificationResults){ },
        .terminalCapabilities = &(union TerminalCapabilities){
            .cvmCapability = {
                .noCvmRequired = 1,
                .plainTextPinForIccVerification = 1,
                .encipheredPinForOfflineVerification = 1,
                .signature = 1
            }
        },
        .additionalTerminalCapabilities = &(union AdditionalTerminalCapabilities){
            .terminalDataInput = {
                .numericKeys = 1,
                .alphabeticAndSpecialCharactersKeys = 1,
                .commandKeys = 1,
                .functionKeys = 1
            },
            .terminalDataOutput = {
                .printAttendant = 1,
                .printCardholder = 1,
                .displayAttendant = 1,
                .displayCardholder = 1
            }
        },
        .cvmManualEntry = &(enum CvmManualEntry){ CVM_MAN_NO_CVM },
        .applicationProfileSettings = {
            .forcedApprovalAllowedForOnlineDeclinedTrx = 1,
            .forcedApprovalAllowedForOfflineDeclinedTrx = 1,
            .cvdRequiredForManualEntry = 1,
            .pinBypassAllowed = 1,
            .merchantReceipt = {
                .printApproved = 1,
                .printDeclined = 1,
                .printAborted = 1,
                .printDolOnApprovedAndAbortedAndVoiceAuth = 1,
                .printDolOnDeclinedAndAborted = 1
            },
            .cardholderReceipt = {
                .specificPanProcessingForEmvChip = 1,
                .printApproved = 1,
                .printDeclined = 1,
                .printAborted = 1
            }
        }
    },
    .next = &(struct ApplicationProfileList){
        .entry = {
            .profileNumber = 0x02,
            .acquirerNumber = 0x01,
            .acquirerIdentifier = { .v = { 0x45, 0x00, 0x00, 0x00, 0x00, 0x01 } },
            .terminalCountryCode = { .bcd.v = { 0x06, 0x16 } },
            .tacDefault = &(union TerminalVerificationResults){ },
            .tacDenial = &(union TerminalVerificationResults){ },
            .tacOnline = &(union TerminalVerificationResults){ },
            .terminalCapabilities = &(union TerminalCapabilities){
                .cvmCapability = {
                    .noCvmRequired = 1,
                    .plainTextPinForIccVerification = 1,
                    .signature = 1
                }
            },
            .cvmMagneticStripe = CVM_MSR_SIGNATURE,
            .cvmManualEntry = &(enum CvmManualEntry){ CVM_MAN_NO_CVM },
            .applicationProfileSettings = {
                .cvdRequiredForManualEntry = 1,
                .merchantReceipt = {
                    .printApproved = 1,
                    .printDeclined = 1,
                    .printAborted = 1,
                    .printDolOnApprovedAndAbortedAndVoiceAuth = 1,
                    .printDolOnDeclinedAndAborted = 1
                },
                .cardholderReceipt = {
                    .printApproved = 1,
                    .printDeclined = 1,
                    .printAborted = 1
                }
            }
        },
        .next = NULL
    }
};

struct TerminalListOfBid* e7 = &(struct TerminalListOfBid){
    .Bid = {
        .size = 2,
        .value = { 0x11, 0x22 }
    },
    .matchingPattern = {
        .type = MATCH_PREFIX,
        .prefix = {
            .size = 1,
            .value = "4"
        },
        .next = NULL
    },
    .next = NULL
};

struct ApplicationProfileSelectionTableNonChip* e8 = &(struct ApplicationProfileSelectionTableNonChip){
    .Bid = {
        .size = 2,
        .value = { 0x11, 0x22 }
    },
    .ApplicationProfileNumber = 0x02,
    .SupportedServices = {
        .payment = 1,
        .cardValidityCheck = 1
    }
};

struct ExceptionFile* e9 = &(struct ExceptionFile){
    .entry = {
        .prefix = {
            .size = 2,
            .value = "47"
        }
    },
    .next = NULL
};
