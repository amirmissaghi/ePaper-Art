// AmbientPoetryMode.h — 🖋️ Quote system with themed art pairing
#pragma once

#include "modes/ArtMode.h"
#include "display/DisplayManager.h"

// Theme tags that map quotes to art algorithms
enum class QuoteTheme : uint8_t {
    NATURE,    // → Botanist (plants, organic)
    SKY,       // → Mood Window (atmospheric)
    WATER,     // → Flow Field / Circle Packing
    ABSTRACT,  // → Spirograph / Voronoi
    SCENE,     // → Story Canvas (landscapes)
};

// A quote paired with its visual theme
struct Quote {
    const char* text;
    QuoteTheme theme;
};

class AmbientPoetryMode : public ArtMode {
public:
    const char* name() override { return "Ambient Poetry"; }
    void init(uint32_t seed) override;
    void generate(AnimationEngine &engine) override;

    // Render quote text in a given Y region of the display
    // startY: top of the text area, height: available height
    void renderText(DisplayManager &dm, int startY = 270, int height = 100);

    // ── Static accessors for ModeManager to use directly ──
    static const Quote& getQuote(int index);
    static int getQuoteCount();
    static uint32_t hashQuote(const char* text);

    // Current quote (set by init, or forced by setQuoteIndex)
    int currentQuoteIndex() const { return _quoteIndex; }
    void setQuoteIndex(int idx) { _quoteIndex = idx % getQuoteCount(); }
private:
    uint32_t _seed;
    int _quoteIndex;
    int _borderStyle;

    void drawVineBorder(AnimationEngine &engine);
    void drawGeometricBorder(AnimationEngine &engine);
    void drawDotBorder(AnimationEngine &engine);
    void drawLineBorder(AnimationEngine &engine);
};
