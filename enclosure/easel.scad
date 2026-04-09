// ePaper Art Device — Portrait Easel Enclosure v5
// Parametric OpenSCAD for FDM 3D printing — NO SUPPORTS NEEDED
//
// ACTUAL HARDWARE MEASUREMENTS (user-verified):
//   Active display:  65mm × 85mm
//   PCB module:      80.5mm × 101mm
//   Dead space:      L=12mm, R=3.5mm, T=8mm, B=8mm
//   Ribbon cable:    18mm extends below PCB bottom
//
// FRAME LAYOUT (front view):
//   Left/Right/Top bezel = 12mm (identical)
//   Bottom bezel = 27mm (cable routing hidden inside)
//   Frame = 87mm × 122mm
//   Window = 63mm × 83mm
//
// Print orientations (all support-free):
//   front_frame   → face DOWN (flat)
//   back_shell    → open side UP (flat bottom)
//   easel_front   → lying on BACK (flat)
//   easel_back    → flat
//
// ═══════════════════════════════════════════════════════════

$fn = 40;

// ───────────────────────────────────────────
//  HARDWARE DIMENSIONS (user-measured)
// ───────────────────────────────────────────

// Display active area
disp_w     = 65.0;
disp_h     = 85.0;

// PCB module (derived: dead_L + disp + dead_R, dead_T + disp + dead_B)
dead_left  = 12.0;          // Left dead space (chin, internal wiring)
dead_right = 3.5;           // Right dead space
dead_top   = 8.0;           // Top dead space
dead_bottom = 8.0;          // Bottom dead space
pcb_w      = dead_left + disp_w + dead_right;     // 80.5mm
pcb_h      = dead_top + disp_h + dead_bottom;     // 101mm
pcb_d      = 1.6;
glass_d    = 1.2;

// Ribbon cable extends below PCB
cable_len  = 18.0;

// ESP32 DevKit V1
esp_w      = 52.0;
esp_h      = 28.0;
esp_d      = 10.0;

// ───────────────────────────────────────────
//  ENCLOSURE PARAMETERS
// ───────────────────────────────────────────

wall       = 2.0;
corner_r   = 2.5;
overlap    = 1.0;           // Frame overlaps display glass edge

// Bezels: L = R = Top = 12mm (identical from front)
bezel      = 12.0;

// Display window (visible opening)
win_w      = disp_w - overlap * 2;                 // 63mm
win_h      = disp_h - overlap * 2;                 // 83mm

// Frame outer width
frame_w    = bezel + win_w + bezel;                // 87mm

// Bottom bezel: cable(18) + dead_bottom(8) + overlap(1) + wall(2) = 29
// Using 27mm (cable_bottom lands at wall=2mm exactly)
bezel_bottom = wall + cable_len + dead_bottom + overlap;  // 29mm
frame_h    = bezel_bottom + win_h + bezel;          // 124mm

// Window position
win_x      = bezel;                                // 12mm from left
win_y      = bezel_bottom;                         // 29mm from bottom

// PCB target position (where feet will place it)
pcb_x      = win_x + overlap - dead_left;          // 1mm from left
pcb_y      = win_y + overlap - dead_bottom;        // 22mm from bottom

// Cable bottom check: pcb_y - cable_len = 22 - 18 = 4mm (> wall ✓)

// Frame depth — extra room for printable feet
front_d    = wall + glass_d + pcb_d + 1.5;         // 6.3mm
back_d     = 14.0;

// Pocket — generous recess, clamped within frame walls
pocket_wall = 1.0;          // Minimum pocket wall thickness
pocket_x   = pocket_wall;
pocket_y   = pocket_wall;
pocket_w   = frame_w - pocket_wall * 2;
pocket_h   = frame_h - pocket_wall * 2;

// Friction-fit lip
fit_lip_h  = 2.0;
fit_lip_t  = 0.8;
fit_tol    = 0.15;

// ───────────────────────────────────────────
//  RENDER CONTROL
// ───────────────────────────────────────────
render_part = "all";

// ───────────────────────────────────────────
//  EASEL PARAMETERS
// ───────────────────────────────────────────
leg_section    = 5.0;
leg_height     = 140.0;      // Taller frame needs taller easel
bottom_spread  = 85.0;
top_spread     = 24.0;
shelf_h        = 12.0;
shelf_lip      = 9.0;        // Extended lip
shelf_thick    = 3.5;

back_leg_len   = 105.0;
back_leg_w     = 10.0;
back_leg_d     = 4.0;
slot_tol       = 0.3;

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
//  L/R/Top bezel = 12mm (identical from front)
//  Bottom bezel = 29mm (hides cable routing area)
//  PCB pocket: generous recess from back
//  4 corner feet position display behind window
//

module front_frame() {
    difference() {
        // Solid frame body
        rrect(frame_w, frame_h, front_d, corner_r);

        // Display window — shows active area
        translate([win_x, win_y, -0.1])
            rrect(win_w, win_h, front_d + 0.2, 1.5);

        // PCB pocket — generous recess from back
        // Thin walls OK here — back shell provides real containment
        translate([pocket_x, pocket_y, wall])
            cube([pocket_w, pocket_h, front_d]);
    }

    // ── Four corner feet ──
    // Position the display so active area aligns with window
    // Height fills gap between pocket floor and PCB back
    foot_sz = 5.0;
    foot_h  = front_d - wall - glass_d - pcb_d;   // ~1.5mm

    // Feet at PCB's intended corner positions
    // Slightly inset from PCB edges
    fi = 2.0;  // foot inset from PCB edge
    translate([pcb_x + fi, pcb_y + fi, wall])
        cube([foot_sz, foot_sz, foot_h]);
    translate([pcb_x + pcb_w - foot_sz - fi, pcb_y + fi, wall])
        cube([foot_sz, foot_sz, foot_h]);
    translate([pcb_x + fi, pcb_y + pcb_h - foot_sz - fi, wall])
        cube([foot_sz, foot_sz, foot_h]);
    translate([pcb_x + pcb_w - foot_sz - fi,
               pcb_y + pcb_h - foot_sz - fi, wall])
        cube([foot_sz, foot_sz, foot_h]);
}

// ═══════════════════════════════════════════════════════════
//  2. BACK SHELL
// ═══════════════════════════════════════════════════════════
//
//  Matches front frame dimensions (87 × 124mm)
//  ESP32 centered on a single vent slot
//  6 uniform 4mm vents evenly spaced
//  Friction-fit ridge mates with front frame
//

module back_shell() {
    shell_d = back_d + wall;

    // Vent layout: 6 evenly spaced, all 4mm tall
    vent_count  = 6;
    vent_h      = 4.0;
    vent_margin = 10.0;
    vent_range  = frame_h - vent_margin * 2;
    vent_spacing = vent_range / (vent_count - 1);

    // ESP32 centered on vent #3
    vent3_y      = vent_margin + 3 * vent_spacing;
    esp_center_y = vent3_y + vent_h / 2;
    esp_x = (frame_w - esp_w) / 2;
    esp_y = esp_center_y - esp_h / 2;

    bracket_h = 9.0;
    bracket_t = 2.5;
    foot   = 4.0;
    foot_h = 3.0;

    difference() {
        rrect(frame_w, frame_h, shell_d, corner_r);

        // Hollow interior
        translate([wall, wall, wall])
            rrect(frame_w - wall*2, frame_h - wall*2,
                  shell_d, corner_r - 0.5);

        // ── Vents ──
        for (i = [0 : vent_count - 1]) {
            vy = vent_margin + i * vent_spacing;
            if (i == 3) {
                // ESP32 vent — narrower, between feet
                translate([esp_x + foot + 1, vy, -0.1])
                    cube([esp_w - foot*2 - 2, vent_h, wall + 0.2]);
            } else {
                translate([frame_w * 0.15, vy, -0.1])
                    cube([frame_w * 0.7, vent_h, wall + 0.2]);
            }
        }
    }

    // ── Friction-fit perimeter ridge ──
    translate([wall + fit_tol, wall + fit_tol, shell_d - fit_lip_h])
        difference() {
            rrect(frame_w - wall*2 - fit_tol*2,
                  frame_h - wall*2 - fit_tol*2,
                  fit_lip_h, corner_r - 0.5);
            translate([fit_lip_t, fit_lip_t, -0.1])
                rrect(frame_w - wall*2 - fit_tol*2 - fit_lip_t*2,
                      frame_h - wall*2 - fit_tol*2 - fit_lip_t*2,
                      fit_lip_h + 0.2, corner_r - 1.0);
        }

    // ── ESP32 brackets (all 4 walls split with cable gap) ──
    usb_gap = 12.0;
    side_half = (esp_h + 2 - usb_gap) / 2;
    bot_half  = (esp_w - usb_gap) / 2;

    // Left bracket
    translate([esp_x - bracket_t, esp_y - 1, wall])
        cube([bracket_t, side_half, bracket_h]);
    translate([esp_x - bracket_t,
               esp_y - 1 + esp_h + 2 - side_half, wall])
        cube([bracket_t, side_half, bracket_h]);

    // Right bracket
    translate([esp_x + esp_w, esp_y - 1, wall])
        cube([bracket_t, side_half, bracket_h]);
    translate([esp_x + esp_w,
               esp_y - 1 + esp_h + 2 - side_half, wall])
        cube([bracket_t, side_half, bracket_h]);

    // Bottom bracket
    translate([esp_x, esp_y - bracket_t, wall])
        cube([bot_half, bracket_t, bracket_h]);
    translate([esp_x + esp_w - bot_half,
               esp_y - bracket_t, wall])
        cube([bot_half, bracket_t, bracket_h]);

    // Top bracket
    translate([esp_x, esp_y + esp_h, wall])
        cube([bot_half, bracket_t, bracket_h]);
    translate([esp_x + esp_w - bot_half,
               esp_y + esp_h, wall])
        cube([bot_half, bracket_t, bracket_h]);

    // ── ESP32 corner feet (3mm tall, on solid back plate) ──
    translate([esp_x, esp_y, wall])
        cube([foot, foot, foot_h]);
    translate([esp_x + esp_w - foot, esp_y, wall])
        cube([foot, foot, foot_h]);
    translate([esp_x, esp_y + esp_h - foot, wall])
        cube([foot, foot, foot_h]);
    translate([esp_x + esp_w - foot, esp_y + esp_h - foot, wall])
        cube([foot, foot, foot_h]);
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

module easel_front() {
    top_x_left = (bottom_spread - top_spread) / 2;

    difference() {
        union() {
            easel_front_leg(0);
            easel_front_leg(1);

            // Bottom crossbar + shelf
            translate([0, 0, shelf_h])
                cube([bottom_spread, leg_section, shelf_thick]);

            // Shelf lip (frame rests on this)
            translate([0, -shelf_lip, shelf_h])
                cube([bottom_spread,
                      shelf_lip + leg_section,
                      shelf_thick / 2]);

            // Front lip ridge (prevents sliding)
            translate([0, -shelf_lip, shelf_h])
                cube([bottom_spread, 2.5, shelf_thick + 4]);

            // Top crossbar
            translate([top_x_left, 0, leg_height - leg_section])
                cube([top_spread, leg_section, leg_section]);
        }

        // Cross-slot: cut from TOP of crossbar
        slot_w = back_leg_d + slot_tol;
        slot_depth = leg_section / 2 + 0.5;
        translate([top_x_left + (top_spread - slot_w) / 2,
                   -0.1,
                   leg_height - slot_depth])
            cube([slot_w, leg_section + 0.2, slot_depth + 0.1]);
    }
}

// ═══════════════════════════════════════════════════════════
//  3b. EASEL BACK LEG — cross-slot tab
// ═══════════════════════════════════════════════════════════

module easel_back_leg() {
    difference() {
        union() {
            rrect(back_leg_w, back_leg_len, back_leg_d, 1.5);

            // Wider foot
            hull() {
                translate([0, 0, 0])
                    rrect(back_leg_w, 5, back_leg_d, 1.5);
                translate([-4, -3, 0])
                    rrect(back_leg_w + 8, 5, back_leg_d, 1.5);
            }
        }

        // Cross-slot: from top end
        slot_w = leg_section + slot_tol;
        slot_depth = back_leg_d / 2 + 0.5;
        translate([(back_leg_w - slot_w) / 2,
                   back_leg_len - leg_section - 0.1,
                   -0.1])
            cube([slot_w, leg_section + 0.2, slot_depth + 0.1]);
    }
}

// ═══════════════════════════════════════════════════════════
//  ASSEMBLY PREVIEW
// ═══════════════════════════════════════════════════════════

module assembly() {
    color("WhiteSmoke", 0.9)
        front_frame();

    translate([0, 0, -(back_d + wall)])
        color("LightGray", 0.7)
            back_shell();

    // Ghost display
    color("DarkSlateGray", 0.3)
        translate([win_x + overlap, win_y + overlap, -0.3])
            cube([disp_w - overlap*2, disp_h - overlap*2, 0.3]);

    // Easel
    easel_x = (frame_w - bottom_spread) / 2;
    easel_z = -(back_d + wall + 3);
    translate([easel_x, -shelf_lip + 2, easel_z])
        rotate([-8, 0, 0])
            color("BurlyWood", 0.8)
                easel_front();

    translate([frame_w/2 - back_leg_w/2,
               frame_h * 0.75,
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
    easel_front();
} else if (render_part == "easel_back") {
    easel_back_leg();
} else {
    assembly();
}
