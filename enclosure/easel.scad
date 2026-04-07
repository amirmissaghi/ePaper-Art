// ePaper Art Device — Portrait Easel Enclosure v3
// Parametric OpenSCAD for FDM 3D printing
//
// PORTRAIT orientation — taller than wide
// Ribbon / wire harness exits BOTTOM
// Even 11mm bezel on L/R, ~12.5mm on T/B
// PCB offset inside: 10mm dead space on left
//
// ═══════════════════════════════════════════════════════════

$fn = 40;

// ───────────────────────────────────────────
//  HARDWARE DIMENSIONS
// ───────────────────────────────────────────

// Waveshare 4.2" module — PORTRAIT (ribbon at bottom)
pcb_w      = 78.5;         // Shorter side (bottom, ribbon here)
pcb_h      = 103.0;        // Longer side (portrait tall)
pcb_d      = 1.6;          // PCB thickness
glass_d    = 1.2;          // Glass above PCB

// Display active area (portrait)
disp_w     = 68.0;         // Active width
disp_h     = 84.8;         // Active height
dead_left  = 10.0;         // Left dead space (internal wiring)
// Right dead: pcb_w - dead_left - disp_w = 0.5mm

// ESP32 DevKit V1
esp_w      = 52.0;
esp_h      = 28.0;
esp_d      = 10.0;
usb_w      = 8.0;
usb_h      = 3.5;

// ───────────────────────────────────────────
//  ENCLOSURE PARAMETERS
// ───────────────────────────────────────────

wall       = 2.0;
tol        = 0.3;
corner_r   = 2.5;
overlap    = 1.0;          // Bezel overlaps display glass edge

// Bezel: 11mm on L and R sides
bezel_lr   = 11.0;

// Window (visible display opening)
win_w      = disp_w - overlap * 2;          // 66mm
win_h      = disp_h - overlap * 2;          // 82.8mm

// Frame outer width: window + 11mm bezel each side
frame_w    = win_w + bezel_lr * 2;          // 88mm

// PCB position inside frame (X axis)
// Display left in frame = bezel_lr + overlap = 12mm
// Display left on PCB = dead_left = 10mm
// So PCB left = 12 - 10 = 2mm from frame left
pcb_x      = bezel_lr + overlap - dead_left;  // 2mm

// Frame height: center PCB vertically with even T/B bezels
// Display is centered on PCB: dead_top = (pcb_h - disp_h)/2 = 9.1mm
dead_tb    = (pcb_h - disp_h) / 2;           // 9.1mm

// PCB vertical position: centered in frame
// For even bezels, pcb_y such that display is centered
// display_bottom = pcb_y + dead_tb
// window_bottom = display_bottom + overlap
// bezel_bottom = window_bottom
// frame_h = window_bottom + win_h + bezel_bottom
// → frame_h = 2 * (pcb_y + dead_tb + overlap) + win_h
// For pcb_y = wall + tol = 2.3:
//   bezel_tb = 2.3 + 9.1 + 1 = 12.4mm
//   frame_h = 12.4 + 82.8 + 12.4 = 107.6 → 108mm

pcb_y      = 2.5;                             // Slight spacing from bottom
bezel_tb   = pcb_y + dead_tb + overlap;       // ~12.6mm
frame_h    = bezel_tb * 2 + win_h;            // ~108mm

// Window position (centered by construction)
win_x      = bezel_lr;                        // 11mm from left
win_y      = bezel_tb;                        // ~12.6mm from bottom

// Frame depths
front_d    = wall + glass_d + pcb_d + 0.5;    // ~5.3mm
back_d     = 14.0;                            // Interior for ESP32 + wiring

// Wire harness slot (bottom edge)
harness_w  = 35.0;         // Wide enough for ribbon + jumper wires
harness_h  = 5.0;          // Slot height

// ───────────────────────────────────────────
//  RENDER CONTROL
// ───────────────────────────────────────────
// "front", "back", "easel_front", "easel_back", "all"
render_part = "all";

// ───────────────────────────────────────────
//  EASEL PARAMETERS
// ───────────────────────────────────────────
leg_section    = 5.0;
leg_height     = 130.0;
bottom_spread  = 82.0;     // Wider than frame for stability
top_spread     = 22.0;
shelf_h        = 12.0;
shelf_lip      = 7.0;
shelf_thick    = 3.0;

back_leg_len   = 100.0;
back_leg_w     = 8.0;
back_leg_d     = 4.0;
peg_r          = 2.5;
peg_len        = 8.0;

// ═══════════════════════════════════════════════════════════
//  UTILITY
// ═══════════════════════════════════════════════════════════

module rrect(w, h, d, r) {
    hull() {
        translate([r, r, 0])     cylinder(d, r, r);
        translate([w-r, r, 0])   cylinder(d, r, r);
        translate([r, h-r, 0])   cylinder(d, r, r);
        translate([w-r, h-r, 0]) cylinder(d, r, r);
    }
}

// ═══════════════════════════════════════════════════════════
//  1. FRONT FRAME
// ═══════════════════════════════════════════════════════════
//
//  Portrait rectangle — taller than wide
//  Even 11mm bezel on L/R, ~12.5mm on T/B
//  Wire harness slot at bottom edge
//
module front_frame() {
    difference() {
        // Solid frame body
        rrect(frame_w, frame_h, front_d, corner_r);

        // Display window (centered on display, NOT on PCB)
        translate([win_x, win_y, -0.1])
            rrect(win_w, win_h, front_d + 0.2, 1.5);

        // PCB pocket (from the back)
        translate([pcb_x - tol, pcb_y - tol, wall])
            cube([pcb_w + tol * 2, pcb_h + tol * 2, front_d]);

        // Wire harness slot at bottom (ribbon + jumpers exit here)
        translate([(frame_w - harness_w) / 2, -0.1, wall])
            cube([harness_w, wall + pcb_y + 0.2, harness_h]);
    }

    // Retaining clips (hold display glass down)
    clip_h = glass_d + 0.3;
    clip_t = 1.0;
    clip_len = 8.0;

    // Left clips (in dead space zone)
    translate([pcb_x + 1, pcb_y + pcb_h * 0.3, wall])
        cube([clip_t, clip_len, clip_h]);
    translate([pcb_x + 1, pcb_y + pcb_h * 0.65, wall])
        cube([clip_t, clip_len, clip_h]);

    // Right clips
    translate([pcb_x + pcb_w - 1 - clip_t, pcb_y + pcb_h * 0.3, wall])
        cube([clip_t, clip_len, clip_h]);
    translate([pcb_x + pcb_w - 1 - clip_t, pcb_y + pcb_h * 0.65, wall])
        cube([clip_t, clip_len, clip_h]);

    // Top clips
    translate([pcb_x + pcb_w * 0.3, pcb_y + pcb_h - 2, wall])
        cube([clip_len, clip_t, clip_h]);
    translate([pcb_x + pcb_w * 0.6, pcb_y + pcb_h - 2, wall])
        cube([clip_len, clip_t, clip_h]);
}

// ═══════════════════════════════════════════════════════════
//  2. BACK SHELL
// ═══════════════════════════════════════════════════════════
//
//  ESP32 in upper half (clear of wire harness area)
//  Wire harness slot at bottom matches front frame
//  Vents behind ESP32
//
module back_shell() {
    shell_d = back_d + wall;

    // ESP32 in upper half
    bracket_h = 9.0;
    bracket_t = 2.5;
    esp_x = (frame_w - esp_w) / 2;
    esp_y = frame_h * 0.52;

    difference() {
        // Outer shell
        rrect(frame_w, frame_h, shell_d, corner_r);

        // Hollow interior
        translate([wall, wall, wall])
            rrect(frame_w - wall*2, frame_h - wall*2, shell_d, corner_r - 0.5);

        // ── Wire harness slot at bottom ──
        // Matches front frame — ribbon + jumper wires + USB all exit here
        translate([(frame_w - harness_w) / 2, -0.1, wall])
            cube([harness_w, wall + 0.2, harness_h]);

        // ── Vent slots behind ESP32 ──
        for (i = [0:2]) {
            translate([frame_w * 0.2, esp_y + 2 + i * 10, -0.1])
                cube([frame_w * 0.6, 1.5, wall + 0.2]);
        }

        // ── Lower area vents ──
        for (i = [0:1]) {
            translate([frame_w * 0.25, wall + 15 + i * 14, -0.1])
                cube([frame_w * 0.5, 1.2, wall + 0.2]);
        }
    }

    // ── Snap-fit lip ──
    lip_h = 1.5;
    lip_t = 0.8;
    translate([wall - lip_t/2, wall - lip_t/2, shell_d - lip_h])
        difference() {
            rrect(frame_w - wall*2 + lip_t,
                  frame_h - wall*2 + lip_t,
                  lip_h, corner_r - 0.5);
            translate([lip_t, lip_t, -0.1])
                rrect(frame_w - wall*2 - lip_t,
                      frame_h - wall*2 - lip_t,
                      lip_h + 0.2, corner_r - 1.0);
        }

    // ── ESP32 wall brackets (upper half) ──
    // Left bracket
    translate([esp_x - bracket_t, esp_y - 1, wall])
        cube([bracket_t, esp_h + 2, bracket_h]);
    // Right bracket
    translate([esp_x + esp_w, esp_y - 1, wall])
        cube([bracket_t, esp_h + 2, bracket_h]);
    // Bottom bracket (solid wall between ESP32 and wire area)
    translate([esp_x, esp_y - bracket_t, wall])
        cube([esp_w, bracket_t, bracket_h]);
    // Top bracket (with center gap for wires to ESP32 pins)
    translate([esp_x, esp_y + esp_h, wall])
        cube([esp_w * 0.3, bracket_t, bracket_h]);
    translate([esp_x + esp_w * 0.7, esp_y + esp_h, wall])
        cube([esp_w * 0.3, bracket_t, bracket_h]);
}

// ═══════════════════════════════════════════════════════════
//  3a. EASEL FRONT — A-frame with shelf
// ═══════════════════════════════════════════════════════════

module easel_front_leg(side) {
    bot_x = side * (bottom_spread - leg_section);
    top_x = (bottom_spread - top_spread) / 2
            + side * (top_spread - leg_section);
    hull() {
        translate([bot_x, 0, 0])
            cube([leg_section, leg_section, 1]);
        translate([top_x, 0, leg_height - 1])
            cube([leg_section, leg_section, 1]);
    }
}

module easel_front_with_sockets() {
    difference() {
        union() {
            // Two front legs
            easel_front_leg(0);
            easel_front_leg(1);

            // Bottom crossbar + shelf
            translate([0, 0, shelf_h])
                cube([bottom_spread, leg_section, shelf_thick]);

            // Shelf lip (extends forward, holds frame)
            translate([0, -shelf_lip, shelf_h])
                cube([bottom_spread, shelf_lip + leg_section,
                      shelf_thick / 2]);

            // Front lip ridge (prevents frame from sliding)
            translate([0, -shelf_lip, shelf_h])
                cube([bottom_spread, 2, shelf_thick + 3]);

            // Top crossbar
            top_x_left = (bottom_spread - top_spread) / 2;
            translate([top_x_left, 0, leg_height - leg_section])
                cube([top_spread, leg_section, leg_section]);
        }

        // Peg socket for back leg
        translate([bottom_spread / 2, leg_section / 2,
                   leg_height - leg_section / 2])
            cylinder(h = peg_len + 0.5, r = peg_r + 0.2);
    }
}

// ═══════════════════════════════════════════════════════════
//  3b. EASEL BACK LEG
// ═══════════════════════════════════════════════════════════

module easel_back_leg() {
    union() {
        // Main leg
        rrect(back_leg_w, back_leg_len, back_leg_d, 1.5);

        // Peg (inserts into front A-frame)
        translate([back_leg_w / 2, back_leg_len - 1,
                   back_leg_d / 2])
            rotate([-90, 0, 0])
                cylinder(h = peg_len, r = peg_r);

        // Wider foot
        hull() {
            translate([0, 0, 0])
                rrect(back_leg_w, 5, back_leg_d, 1.5);
            translate([-3, -2, 0])
                rrect(back_leg_w + 6, 4, back_leg_d, 1.5);
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  ASSEMBLY PREVIEW
// ═══════════════════════════════════════════════════════════

module assembly() {
    // Front frame — facing viewer
    color("WhiteSmoke", 0.9)
        front_frame();

    // Back shell — behind it
    translate([0, 0, -(back_d + wall)])
        color("LightGray", 0.7)
            back_shell();

    // Ghost display for reference
    color("DarkSlateGray", 0.3)
        translate([pcb_x + dead_left + overlap,
                   pcb_y + dead_tb + overlap,
                   -0.3])
            cube([win_w, win_h, 0.3]);

    // Easel A-frame — behind and below
    easel_x = (frame_w - bottom_spread) / 2;
    easel_z = -(back_d + wall + 2);
    translate([easel_x, -shelf_lip, easel_z])
        rotate([-8, 0, 0])
            color("BurlyWood", 0.8)
                easel_front_with_sockets();

    // Back leg
    translate([frame_w/2 - back_leg_w/2,
               frame_h * 0.8,
               easel_z - 5])
        rotate([55, 0, 0])
            color("BurlyWood", 0.7)
                easel_back_leg();
}

// ═══════════════════════════════════════════════════════════
//  RENDER
// ═══════════════════════════════════════════════════════════

if (render_part == "front") {
    front_frame();
} else if (render_part == "back") {
    back_shell();
} else if (render_part == "easel_front") {
    easel_front_with_sockets();
} else if (render_part == "easel_back") {
    easel_back_leg();
} else {
    // "all" — assembly preview
    assembly();
}
