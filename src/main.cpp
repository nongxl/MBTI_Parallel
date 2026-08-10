#include <cstdio>
#include "MBTIProfile.h"
#include "Scenario.h"
#include "DecisionEngine.h"
#include "ScenarioBuilder.h"
#include "ScenarioMapper.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void runTestSelection(const char* testName, const UserSelection& selection) {
    printf("==================================================\n");
    printf(" PHASE 2 TEST: %s\n", testName);
    printf(" SELECTION: %s | %s | Concern: %s(%s) | Priority: %s\n",
           getDecisionTypeName(selection.decisionType),
           getMotivationName(selection.motivation),
           getConcernName(selection.concern),
           getIntensityName(selection.intensity),
           getPriorityName(selection.priority));
    printf("--------------------------------------------------\n");

    Scenario scenario = buildScenario(selection);
    printf(" GENERATED SCENARIO PARAMETERS:\n");
    printf("  Risk: %4.1f | Cost: %4.1f | Time: %4.1f | Novelty: %4.1f\n",
           scenario.risk, scenario.cost, scenario.time, scenario.novelty);
    printf("  Social: %4.1f | Effort: %4.1f | Uncertain: %4.1f\n",
           scenario.social, scenario.effort, scenario.uncertainty);
    printf("  EmotionVal: %4.1f | PracticalVal: %4.1f\n",
           scenario.emotionalValue, scenario.practicalValue);
    printf("--------------------------------------------------\n");

    DecisionResult results[MBTI_COUNT];
    simulateAll(scenario, results);

    for (int i = 0; i < MBTI_COUNT; ++i) {
        printf("%-6s | %-5s | Score: %5.1f | Reason: %s\n",
               getMBTIName(results[i].personality),
               getDecisionName(results[i].decision),
               results[i].score,
               results[i].reason);
    }

    DecisionSummary summary = summarizeResults(results);
    MBTIType yesType, noType;
    findBiggestSplit(results, yesType, noType);

    printf("\n SUMMARY: YES: %d, NO: %d, MAYBE: %d\n", summary.yesCount, summary.noCount, summary.maybeCount);
    printf(" BIGGEST SPLIT: %s (YES) VS %s (NO)\n", getMBTIName(yesType), getMBTIName(noType));
    printf("==================================================\n\n");
}

void runRandomTest(int presetIndex) {
    const RandomPreset& preset = getRandomPreset(presetIndex);
    printf("==================================================\n");
    printf(" PHASE 2 RANDOM SCENARIO: %s\n", preset.title);
    printf(" Description: %s\n", preset.description);
    printf("--------------------------------------------------\n");

    DecisionResult results[MBTI_COUNT];
    simulateAll(preset.scenario, results);

    DecisionSummary summary = summarizeResults(results);
    MBTIType yesType, noType;
    findBiggestSplit(results, yesType, noType);

    printf(" SUMMARY: YES: %d, NO: %d, MAYBE: %d\n", summary.yesCount, summary.noCount, summary.maybeCount);
    printf(" BIGGEST SPLIT: %s (YES) VS %s (NO)\n", getMBTIName(yesType), getMBTIName(noType));
    printf("==================================================\n\n");
}

void runPhase2Tests() {
    printf("\n\n");
    printf("==================================================\n");
    printf("        MBTI SCENARIO BUILDER (PHASE 2) TEST      \n");
    printf("==================================================\n\n");

    // Test 1: GET + WANT + COST(HIGH) + EXPERIENCE
    UserSelection test1 = {
        DecisionType::GET,
        Motivation::WANT,
        Concern::COST,
        Intensity::HIGH,
        Priority::EXPERIENCE
    };
    runTestSelection("Test 1 (GET WANT HIGH COST)", test1);

    // Test 2: GO + FUN + UNKNOWN(HIGH) + EXPERIENCE
    UserSelection test2 = {
        DecisionType::GO,
        Motivation::FUN,
        Concern::UNKNOWN,
        Intensity::HIGH,
        Priority::EXPERIENCE
    };
    runTestSelection("Test 2 (GO FUN HIGH UNKNOWN)", test2);

    // Test 3: DO + NEED + RISK(LOW) + PRACTICAL
    UserSelection test3 = {
        DecisionType::DO,
        Motivation::NEED,
        Concern::RISK,
        Intensity::LOW,
        Priority::PRACTICAL
    };
    runTestSelection("Test 3 (DO NEED LOW RISK)", test3);

    // Test 4: CHANGE + OPPORTUNITY + TIME(HIGH) + EXPERIENCE
    UserSelection test4 = {
        DecisionType::CHANGE,
        Motivation::OPPORTUNITY,
        Concern::TIME,
        Intensity::HIGH,
        Priority::EXPERIENCE
    };
    runTestSelection("Test 4 (CHANGE OPPORTUNITY HIGH TIME)", test4);

    // RANDOM 模式测试
    runRandomTest(0); // LAST-MINUTE TRIP
    runRandomTest(2); // BUY SOMETHING DESIRED
}

#ifdef ARDUINO
void setup() {
    Serial.begin(115200);
    delay(1000);
    runPhase2Tests();
}

void loop() {
    delay(1000);
}
#else
int main() {
    runPhase2Tests();
    return 0;
}
#endif
