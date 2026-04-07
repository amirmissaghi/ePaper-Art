// LSystem.h — L-System string generation + turtle graphics interpreter
#pragma once

#include <Arduino.h>
#include <vector>
#include "animation/AnimationEngine.h"

// A single production rule
struct LRule {
    char symbol;
    const char *replacement;
};

// Stochastic rule — multiple possible replacements for the same symbol
struct LStochasticRule {
    char symbol;
    const char *replacements[4];  // Up to 4 variants
    int count;                     // How many variants
};

// Turtle state for rendering
struct TurtleState {
    float x, y;
    float angle;  // in radians
};

class LSystem {
public:
    // Generate L-system string from axiom and rules
    // WARNING: strings grow exponentially — keep iterations ≤ 5
    static String generate(const char *axiom, const LRule *rules, int numRules, int iterations);

    // Stochastic generation — randomly picks rule variants
    static String generateStochastic(const char *axiom, const LStochasticRule *rules,
                                      int numRules, int iterations);

    // Interpret the L-system string as turtle graphics and push draw commands
    // turnAngle in degrees
    // Characters: F=forward+draw, f=forward, +=turn right, -=turn left, [=push, ]=pop
    static void renderTurtle(const String &lstring, AnimationEngine &engine,
                             float startX, float startY, float startAngle,
                             float stepLength, float turnAngle,
                             uint16_t color = GxEPD_BLACK);

private:
    static std::vector<TurtleState> stateStack;
};
