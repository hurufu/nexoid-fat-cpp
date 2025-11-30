extern "C" {
#include "nexoid-ed/include/gtd.h"
}

static struct E1KernelData s_e1kd;
static struct TerminalListOfAid s_TerminalListOfAid;

struct ApplicationProfile ap;
struct ServiceSettingsEntry sc;
struct TerminalTransactionData ttd;
struct EntryPointData ep;
struct CardResponse* cr;
struct E1KernelData* e1kd = &s_e1kd;
struct OnlineResponseData ord;
struct SearchTransactionResultList* sl;
struct AidPreferenceTable* ee;

enum PrinterStatus g_PrinterStatus = PRINTER_UNAVAILABLE;
enum TmsContactLevel g_callTms = CALL_TMS_NONE;

struct TerminalListOfAid* g_TerminalListOfAid = &s_TerminalListOfAid;
struct CandidateList* g_CandidateList = NULL;
struct AidPreferenceTable* g_AidPreferenceTable = NULL;
