# ePaper Art Device

A generative art display powered by an ESP32 and a Waveshare 4.2" e-paper screen. It cycles through algorithmically generated artwork and curated poetry, creating an ever-changing digital canvas that looks like a small framed painting.

## What It Does

Every 10 minutes, the device creates a new piece of art — drawn stroke-by-stroke over 3 minutes, then displayed for 7 minutes of quiet contemplation.

### Art Modes
- **Illustrated Poetry** (75%) — Generative art paired with a curated quote from a library of 195 poems, haiku, and philosophical fragments
- **Pure Art** (25%) — Full-screen generative artwork with no text

### Generative Algorithms
| Algorithm | Description |
|-----------|-------------|
| Clifford Attractor | Swirling particle trails from strange attractors |
| Topographic Contours | Perlin noise rendered as elevation contour maps |
| Joy Division Mountains | Stacked ridgelines with organic noise |
| Moiré Interference | Overlapping concentric circle patterns |
| Recursive Mondrian | Random rectangle subdivision, some filled black |
| Stipple Field | Dual-layer noise creating light and shadow from dots |
| Maze Generation | Perfect mazes via recursive backtracking |
| Lissajous Curves | Overlapping harmonic oscilloscope curves |
| L-System Botanist | Procedural plants using stochastic L-systems |
| Story Canvas | Generative landscape scenes |
| Mood Window | Weather-driven atmospheric art |

### Smart Features
- **WiFi duty cycling** — Radio powers off during drawing (~80-150mA savings)
- **Weather-aware art** — Fetches live weather data to influence mood-based artwork
- **Hardware RNG seeding** — `esp_random()` ensures no two reboots produce the same sequence
- **30-stage animation** — Dramatic reveals with 6-second intervals between partial e-paper refreshes

## Hardware

- **MCU**: ESP32 DevKit V1
- **Display**: Waveshare 4.2" e-Paper Module (400×300, black/white)
- **Power**: USB (5V)

### Pin Mapping

| ESP32 Pin | Display Pin |
|-----------|-------------|
| GPIO 5 | CS |
| GPIO 17 | DC |
| GPIO 16 | RST |
| GPIO 4 | BUSY |
| GPIO 23 | MOSI (SPI) |
| GPIO 18 | SCK (SPI) |

## Setup

### 1. Clone and configure

```bash
git clone https://github.com/amirmissaghi/ePaper-Art.git
cd ePaper-Art

# Create your config with WiFi credentials
cp src/config.h.example src/config.h
# Edit src/config.h with your WiFi SSID, password, and location
```

### 2. Build and flash

Requires [PlatformIO](https://platformio.org/).

```bash
pio run --target upload
pio device monitor --baud 115200
```

## Enclosure

A 3D-printable mini easel enclosure is included in `enclosure/`.

### Parts
| File | Description |
|------|-------------|
| `front_frame.stl` | Picture frame (print face down) |
| `back_shell.stl` | ESP32 housing (print open side up) |
| `easel_front.stl` | A-frame front legs with shelf |
| `easel_back_leg.stl` | Rear support leg |

### Parametric Source
The enclosure is fully parametric in OpenSCAD — edit `enclosure/easel.scad` to adjust dimensions for your specific hardware.

## Project Structure

```
src/
├── main.cpp                    # Entry point
├── config.h.example            # Configuration template
├── animation/
│   └── AnimationEngine.*       # Draw command buffering & staged playback
├── display/
│   └── DisplayManager.*        # E-paper driver wrapper
├── modes/
│   ├── ArtMode.h               # Base class for art generators
│   ├── ModeManager.*           # Orchestrates mode cycling & overlays
│   ├── AmbientPoetryMode.*     # 195-quote library with themed borders
│   ├── BotanistMode.*          # L-system procedural plants
│   ├── DailyCanvasMode.*       # 8 wild generative art algorithms
│   ├── MoodWindowMode.*        # Weather-driven atmospheric art
│   └── StoryCanvasMode.*       # Generative landscape scenes
├── network/
│   ├── WiFiManager.*           # WiFi duty cycling
│   ├── TimeManager.*           # NTP time sync
│   └── WeatherClient.*         # Open-Meteo weather API
└── utils/
    ├── Noise.*                 # Perlin noise (via FastLED)
    └── Hatching.*              # Line hatching utilities
enclosure/
├── easel.scad                  # Parametric OpenSCAD source
├── front_frame.stl
├── back_shell.stl
├── easel_front.stl
└── easel_back_leg.stl
```

## License

MIT
