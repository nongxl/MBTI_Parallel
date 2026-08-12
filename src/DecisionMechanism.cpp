#include "DecisionMechanism.h"
#include <cstring>

static DecisionSignature g_signatureHistory[16];
static int g_signatureHistoryCount = 0;

void initMechanismEngine() {
    memset(g_signatureHistory, 0, sizeof(g_signatureHistory));
    g_signatureHistoryCount = 0;
}

bool isSignatureCoolingDown(const DecisionSignature& sig) {
    for (int i = 0; i < g_signatureHistoryCount; ++i) {
        if (g_signatureHistory[i].equals(sig)) {
            return true;
        }
    }
    return false;
}

void markSignatureUsed(const DecisionSignature& sig) {
    for (int i = 15; i > 0; --i) {
        g_signatureHistory[i] = g_signatureHistory[i - 1];
    }
    g_signatureHistory[0] = sig;
    if (g_signatureHistoryCount < 16) {
        g_signatureHistoryCount++;
    }
}
