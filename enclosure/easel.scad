// ePaper Art Device — Portrait Mini Easel Enclosure v2
// Parametric OpenSCAD model for FDM 3D printing
//
// ORIENTATION: Portrait (ribbon at bottom, short side)
//   Display: 68mm wide, biased right on PCB (9mm dead left)
//   Frame has EVEN bezel centered on display, not PCB
//
// THREE PRINTED PARTS:
//   1. Front frame  — picture frame with centered window
//   2. Back shell   — holds ESP32, snaps onto frame
//   3. Easel stand  — classic 2-front-leg + 1-back-leg easel
//
// Set render_part below to export each STL
//
// ═══════════════════════════════════════════════════════════

$fn = 40;

// ───────────────────────────────────────────
//  MODULE DIMENSIONS (from actual hardware)
// ───────────────────────────────────────────

// Waveshare 4.2" module — portrait orientation
pcb_w     = 78.5;          // Shorter side (ribbon here)
pcb_h     = 103.0;         // Longer side (portrait tall)
pcb_d     = 1.6;           // PCB thickness
glass_d   = 1.2;           // Glass panel above PCB

// Display active area (portrait)
disp_w    = 68.0;          // Active width
disp_h    = 84.8;          // Active height
dead_left = 9.0;           // Dead space on left (cabling)
// Right dead space: pcb_w - dead_left - disp_w = 1.5mm

// Display center relative to PCB left edge
disp_center_on_pcb = dead_left + disp_w / 2;  // = 43mm

// ESP32 DevKit V1
esp_w     = 52.0;
esp_h     = 28.0;
esp_d     = 10.0;
usb_w     = 8.0;
usb_h     = 3.5;

// ───────────────────────────────────────────
//  ENCLOSURE PARAMETERS
// ───────────────────────────────────────────

wall      = 2.0;           // Wall thickness
tol       = 0.3;           // Print tolerance
corner_r  = 2.5;           // Corner rounding

// Bezel: even visual border around display window
bezel     = 12.0;          // Visible bezel width (all sides)
overlap   = 1.0;           // How much bezel overlaps display glass edge

// Display window size (slightly smaller than active area)
win_w     = disp_w - overlap * 2;   // 66mm
win_h     = disp_h - overlap * 2;   // 82.8mm

// Frame outer size — centered on display window
frame_w   = win_w + bezel * 2;      // 90mm
frame_h   = win_h + bezel * 2;      // 106.8mm

// Window position (centered in frame)
win_x     = bezel;                   // 12mm from left
win_y     = bezel;                   // 12mm from bottom

// Where the PCB sits in frame coordinates
// Display left in frame = win_x + overlap = 13mm
// Display left on PCB = dead_left = 9mm
// So PCB left edge in frame = 13 - 9 = 4mm
pcb_x     = win_x + overlap - dead_left;     // ~4mm
pcb_y     = (frame_h - pcb_h) / 2;           // centered vertically

// Frame depths
front_d   = wall + glass_d + pcb_d + 0.5;    // ~5.3mm
back_d    = 13.0;                              // Interior depth for ESP32 + wires

// Ribbon cable slot
ribbon_w  = 30.0;
ribbon_h  = 3.0;

// ───────────────────────────────────────────
//  WHICH PART TO RENDER
// ───────────────────────────────────────────
// "front", "back", "easel_front", "easel_back", or "all"
render_part = "all";

// ───────────────────────────────────────────
//  EASEL PARAMETERS  
// ───────────────────────────────────────────

// Front A-frame
leg_section   = 5.0;        // Leg cross-section (square with rounded edges)
leg_height    = 125.0;      // Total leg height
bottom_spread = 80.0;       // Leg spacing at bottom (> frame_w for stability)
top_spread    = 20.0;       // Leg spacing at top (converging)
shelf_h       = 12.0;       // Height of shelf from bottom of legs
shelf_lip     = 7.0;        // How far shelf sticks forward
shelf_thick   = 3.0;        // Shelf thickness

// Back support leg
back_leg_len  = 95.0;       // Back leg length
back_leg_w    = 8.0;        // Back leg width
back_leg_d    = 4.0;        // Back leg thickness
peg_r         = 2.5;        // Peg radius for joint
peg_len       = 8.0;        // Peg insertion depth

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
//  1. FRONT FRAME — Picture frame face
// ═══════════════════════════════════════════════════════════

module front_frame() {
    difference() {
        // Solid frame body
        rrect(frame_w, frame_h, front_d, corner_r);

        // Display window (centered — this is the key!)
        translate([win_x, win_y, -0.1])
            rrect(win_w, win_h, front_d + 0.2, 1.5);

        // PCB pocket (from the back)
        // Sized to hold the full PCB with tolerance
        translate([pcb_x - tol, pcb_y - tol, wall])
            cube([pcb_w + tol*2, pcb_h + tol*2, front_d]);
    }

    // Retaining clips (hold glass against front wall)
    clip_h = glass_d + 0.3;
    clip_t = 1.0;
    clip_len = 8.0;

    // Left side clips (in the dead space — won't interfere with display)
    translate([pcb_x + 1, pcb_y + pcb_h * 0.3, wall])
        cube([clip_t, clip_len, clip_h]);
    translate([pcb_x + 1, pcb_y + pcb_h * 0.65, wall])
        cube([clip_t, clip_len, clip_h]);

    // Right side clips
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
//  2. BACK SHELL — Holds ESP32 and wiring
// ═══════════════════════════════════════════════════════════

module back_shell() {
    shell_d = back_d + wall;

    difference() {
        // Outer shell
        rrect(frame_w, frame_h, shell_d, corner_r);

        // Hollow interior
        translate([wall, wall, wall])
            rrect(frame_w - wall*2, frame_h - wall*2, shell_d, corner_r - 0.5);

        // ── Ribbon cable slot (bottom center) ──
        translate([(frame_w - ribbon_w) / 2, -0.1, wall + 1])
            cube([ribbon_w, wall + 0.2, ribbon_h + 1]);

        // ── USB cable slot (bottom, offset left) ──
        translate([frame_w * 0.12, -0.1, wall + ribbon_h + 3])
            cube([usb_w + 4, wall + 0.2, usb_h + 2]);

        // ── Vent slots behind ESP32 area ──
        vent_y = wall + 8;  // Near bottom where ESP32 sits
        for (i = [0:2]) {
            translate([frame_w * 0.25, vent_y + i * 10, -0.1])
                cube([frame_w * 0.5, 1.5, wall + 0.2]);
        }

        // ── Additional vent slots on back (upper area) ──
        for (i = [0:2]) {
            translate([frame_w * 0.2, frame_h * 0.5 + i * 15, -0.1])
                cube([frame_w * 0.6, 1.2, wall + 0.2]);
        }
    }

    // ── Snap-fit lip (mates with front frame) ──
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

    // ── ESP32 wall brackets ──
    // Simple L-shaped walls instead of thin posts
    // ESP32 positioned near bottom for short USB cable run
    bracket_h = 8.0;
    bracket_t = 2.0;

    esp_x = (frame_w - esp_w) / 2;
    esp_y = wall + 5;

    // Left bracket wall
    translate([esp_x - bracket_t, esp_y - 1, wall])
        cube([bracket_t, esp_h + 2, bracket_h]);

    // Right bracket wall
    translate([esp_x + esp_w, esp_y - 1, wall])
        cube([bracket_t, esp_h + 2, bracket_h]);

    // Bottom bracket wall (back stop)
    translate([esp_x, esp_y - bracket_t, wall])
        cube([esp_w, bracket_t, bracket_h]);

    // Top bracket wall (with gap for wires)
    translate([esp_x, esp_y + esp_h, wall])
        cube([esp_w * 0.3, bracket_t, bracket_h]);
    translate([esp_x + esp_w * 0.7, esp_y + esp_h, wall])
        cube([esp_w * 0.3, bracket_t, bracket_h]);
}

// ═══════════════════════════════════════════════════════════
//  3a. EASEL FRONT — A-frame (two front legs + shelf)
// ═══════════════════════════════════════════════════════════

module easel_front_leg(side) {
    // Single tapered leg from bottom spread to top convergence
    // side: 0 = left, 1 = right
    bot_x = side * (bottom_spread - leg_section);
    top_x = (bottom_spread - top_spread) / 2 + side * (top_spread - leg_section);

    hull() {
        translate([bot_x, 0, 0])
            cube([leg_section, leg_section, 1]);
        translate([top_x, 0, leg_height - 1])
            cube([leg_section, leg_section, 1]);
    }
}

module easel_front() {
    // Two front legs
    easel_front_leg(0);
    easel_front_leg(1);

    // Bottom crossbar / shelf
    translate([0, 0, shelf_h])
        cube([bottom_spread, leg_section, shelf_thick]);

    // Shelf lip (extends FORWARD from the crossbar)
    translate([0, -shelf_lip, shelf_h])
        cube([bottom_spread, shelf_lip + leg_section, shelf_thick / 2]);

    // Small front lip ridge (prevents frame from sliding off)
    translate([0, -shelf_lip, shelf_h])
        cube([bottom_spread, 2, shelf_thick + 3]);

    // Top crossbar connecting the two legs
    top_x_left = (bottom_spread - top_spread) / 2;
    translate([top_x_left, 0, leg_height - leg_section])
        cube([top_spread, leg_section, leg_section]);

    // Peg socket holes for the back leg (in the top crossbar)
    // Two sockets at 1/3 and 2/3 of the top crossbar
    // (these are negative space — we'll subtract them)
}

module easel_front_with_sockets() {
    difference() {
        easel_front();

        // Peg socket in top crossbar (centered)
        top_x_center = bottom_spread / 2;
        translate([top_x_center, leg_section / 2, leg_height - leg_section / 2])
            rotate([0, 0, 0])
                cylinder(h = peg_len + 0.5, r = peg_r + 0.2);
    }
}

// ═══════════════════════════════════════════════════════════
//  3b. EASEL BACK LEG — Single rear support
// ═══════════════════════════════════════════════════════════

module easel_back_leg() {
    union() {
        // Main leg body
        rrect(back_leg_w, back_leg_len, back_leg_d, 1.5);

        // Peg at one end (inserts into front A-frame socket)
        translate([back_leg_w / 2, back_leg_len - 1, back_leg_d / 2])
            rotate([-90, 0, 0])
                cylinder(h = peg_len, r = peg_r);

        // Wider foot at the other end
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
    // Front frame (display face toward viewer, in XY plane)
    color("WhiteSmoke", 0.9)
        front_frame();

    // Back shell (behind the front frame)
    translate([0, 0, -(back_d + wall)])
        color("LightGray", 0.7)
            back_shell();

    // Ghost display (reference)
    color("DarkSlateGray", 0.3)
        translate([pcb_x + dead_left + overlap,
                   pcb_y + (pcb_h - disp_h)/2 + overlap,
                   -0.3])
            cube([disp_w - overlap*2, disp_h - overlap*2, 0.3]);

    // Easel A-frame (behind the assembled frame)
    easel_offset_x = (frame_w - bottom_spread) / 2;
    easel_offset_z = -(back_d + wall + 2);  // Just behind back shell

    translate([easel_offset_x, -shelf_lip, easel_offset_z])
        rotate([-8, 0, 0])  // Slight tilt — legs lean back
            color("BurlyWood", 0.8)
                easel_front_with_sockets();

    // Back leg (angled backward from top of A-frame)
    back_leg_x = frame_w / 2 - back_leg_w / 2;
    translate([back_leg_x,
               frame_h * 0.8,
               easel_offset_z - 5])
        rotate([55, 0, 0])  // Angled backward
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
    // Print lying on its back
    easel_front_with_sockets();
} else if (render_part == "easel_back") {
    // Print flat
    easel_back_leg();
} else {
    // "all" — assembly preview
    assembly();
}
