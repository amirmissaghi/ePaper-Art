# ePaper Art — Enclosure

## Mini Easel Enclosure

A 3-piece 3D-printable enclosure for the ePaper Art device.

### Parts

| Part | File | Description |
|---|---|---|
| **Front Frame** | `easel.scad` (set `render_part = "front"`) | Display window with bezel + clip tabs |
| **Back Shell** | `easel.scad` (set `render_part = "back"`) | Houses ESP32, has USB slot + vent slots |
| **Easel Legs** | `easel.scad` (set `render_part = "legs"`) | Two angled legs that attach to the back |

### How to Export STL Files

1. Install [OpenSCAD](https://openscad.org/downloads.html) (free)
2. Open `easel.scad`
3. Change `render_part` to `"front"`, press F6 (Render), then File → Export as STL
4. Repeat for `"back"` and `"legs"`

### Print Settings

| Setting | Value |
|---|---|
| Material | PLA or PETG |
| Layer Height | 0.2mm |
| Infill | 15-20% |
| Supports | None needed (print each part flat side down) |
| Walls | 3 perimeters |
| Color | White or light gray recommended (looks like a real art frame) |

### Assembly

1. Place the e-paper display face-down into the **front frame** (the clip tabs hold it)
2. Route the FPC ribbon cable through the slot in the top of the **back shell**
3. Mount the ESP32 between the corner posts in the back shell
4. Connect the wiring between ESP32 and display
5. Route the USB cable through the bottom slot
6. Snap the back shell onto the front frame
7. Attach the **easel legs** to the back

### Customization

All dimensions are parametric — edit the values at the top of `easel.scad`:

- `tolerance` — Increase if parts are too tight (try 0.5 or 0.6)
- `bezel` — Width of the border around the display window
- `easel_angle` — Tilt angle (default 15°)
- `corner_r` — Corner roundness

### Dimensions

The enclosure dimensions (with default settings):
- **Front face**: ~107 × 97 mm
- **Depth**: ~27 mm (including legs folded)
- **Weight**: ~40-50g printed
