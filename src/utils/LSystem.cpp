// LSystem.cpp
#include "utils/LSystem.h"
#include <cmath>

std::vector<TurtleState> LSystem::stateStack;

String LSystem::generate(const char *axiom, const LRule *rules, int numRules, int iterations) {
    String current = axiom;

    for (int iter = 0; iter < iterations; iter++) {
        String next;
        next.reserve(current.length() * 2);  // Estimate growth

        for (size_t i = 0; i < current.length(); i++) {
            char c = current[i];
            bool matched = false;
            for (int r = 0; r < numRules; r++) {
                if (c == rules[r].symbol) {
                    next += rules[r].replacement;
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                next += c;
            }
        }
        current = next;

        // Safety: abort if string gets too long for ESP32 memory
        if (current.length() > 15000) {
            Serial.printf("[LSystem] String exceeded 15000 chars at iter %d, stopping\n", iter);
            break;
        }
    }

    Serial.printf("[LSystem] Generated string: %d chars\n", (int)current.length());
    return current;
}

String LSystem::generateStochastic(const char *axiom, const LStochasticRule *rules,
                                    int numRules, int iterations) {
    String current = axiom;

    for (int iter = 0; iter < iterations; iter++) {
        String next;
        next.reserve(current.length() * 2);

        for (size_t i = 0; i < current.length(); i++) {
            char c = current[i];
            bool matched = false;
            for (int r = 0; r < numRules; r++) {
                if (c == rules[r].symbol) {
                    int variant = random(0, rules[r].count);
                    next += rules[r].replacements[variant];
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                next += c;
            }
        }
        current = next;

        if (current.length() > 15000) {
            Serial.printf("[LSystem] Stochastic string exceeded 15000 at iter %d\n", iter);
            break;
        }
    }

    Serial.printf("[LSystem] Stochastic generated: %d chars\n", (int)current.length());
    return current;
}

void LSystem::renderTurtle(const String &lstring, AnimationEngine &engine,
                            float startX, float startY, float startAngle,
                            float stepLength, float turnAngle,
                            uint16_t color) {
    float x = startX;
    float y = startY;
    float angle = startAngle * PI / 180.0f;  // Convert to radians
    float turnRad = turnAngle * PI / 180.0f;

    stateStack.clear();

    for (size_t i = 0; i < lstring.length(); i++) {
        char c = lstring[i];
        switch (c) {
            case 'F': {
                // Move forward and draw
                float nx = x + stepLength * cos(angle);
                float ny = y + stepLength * sin(angle);
                engine.line((int16_t)x, (int16_t)y, (int16_t)nx, (int16_t)ny, color);
                x = nx;
                y = ny;
                break;
            }
            case 'f': {
                // Move forward without drawing
                x += stepLength * cos(angle);
                y += stepLength * sin(angle);
                break;
            }
            case '+':
                angle += turnRad;
                break;
            case '-':
                angle -= turnRad;
                break;
            case '[':
                stateStack.push_back({x, y, angle});
                break;
            case ']':
                if (!stateStack.empty()) {
                    TurtleState state = stateStack.back();
                    stateStack.pop_back();
                    x = state.x;
                    y = state.y;
                    angle = state.angle;
                }
                break;
            // Additional symbols can be added for specific plant types
            case 'L': {
                // Draw a small leaf (filled circle)
                engine.fillCircle((int16_t)x, (int16_t)y, 2, color);
                break;
            }
            case 'O': {
                // Draw a flower/berry (open circle)
                engine.circle((int16_t)x, (int16_t)y, 3, color);
                break;
            }
        }
    }

    Serial.printf("[LSystem] Rendered %d turtle commands\n", (int)lstring.length());
}
