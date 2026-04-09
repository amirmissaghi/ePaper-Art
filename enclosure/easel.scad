// ePaper Art Device — Portrait Easel Enclosure v4
// Parametric OpenSCAD for FDM 3D printing — NO SUPPORTS NEEDED
//
// v4 CHANGES:
//  1. Chin-side bezel +1mm (left=12mm, right=11mm), play for adjustment
//  2. PCB pocket 5% larger — user can slide display before gluing
//  3. Four corner feet in front frame for even display seating
//  4. Bottom extended 10mm — cables route inside, no external harness holes
//  5. ESP32 feet reduced 50% (3mm)
//  6. All vents uniform 4mm size, evenly spaced top-to-bottom
//  7. Friction-fit perimeter ridge joins front+back
//  8. Cross-slot joint connects back leg to A-frame
//  9. Easel shelf lip extended +2mm
// 10. All parts print flat, no supports required
//
// Print orientations:
//   front_frame   → face DOWN (flat)
//   back_shell    → open side UP (flat bottom)
//   easel_front   → lying on BACK (flat)
//   easel_back    → flat
//
// ═══════════════════════════════════════════════════════════

$fn = 40;

// ───────────────────────────────────────────
//  HARDWARE DIMENSIONS
// ───────────────────────────────────────────

pcb_w      = 78.5;          // Display module width
pcb_h      = 103.0;         // Display module height
pcb_d      = 1.6;           // PCB thickness
glass_d    = 1.2;           // E-ink glass above PCB

disp_w     = 68.0;          // Active display width
disp_h     = 84.8;          // Active display height
dead_left  = 10.0;          // Left dead space (chin side, internal wiring)

esp_w      = 52.0;
esp_h      = 28.0;
esp_d      = 10.0;
usb_w      = 8.0;
usb_h      = 3.5;

// ───────────────────────────────────────────
//  ENCLOSURE PARAMETERS
// ───────────────────────────────────────────

wall       = 2.0;
corner_r   = 2.5;
overlap    = 1.0;

// Bezels — asymmetric left (chin) vs right, bottom extended
bezel_left   = 12.0;        // Chin side — +1mm to hide non-functional border
bezel_right  = 11.0;
bezel_top    = 13.0;
bottom_extra = 10.0;        // Extra at bottom for internal cable routing
bezel_bottom = 13.0 + bottom_extra;  // = 23mm (thicker bottom bezel)

// Display window
win_w      = disp_w - overlap * 2;               // 66mm
win_h      = disp_h - overlap * 2;               // 82.8mm

// Frame outer dimensions
frame_w    = bezel_left + win_w + bezel_right;    // 89mm
frame_h    = bezel_bottom + win_h + bezel_top;    // 118.8mm

// Window position
win_x      = bezel_left;                          // 12mm from left
win_y      = bezel_bottom;                        // 23mm from bottom

// PCB nominal position (display centered on window)
pcb_x      = win_x + overlap - dead_left;         // 3mm from left
pcb_y      = win_y + overlap - (pcb_h - disp_h)/2; // ~14.9mm from bottom

// PCB pocket — 5% oversize for adjustment play (~2mm per side)
pocket_tol = 2.0;
pocket_x   = pcb_x - pocket_tol;
pocket_y   = pcb_y - pocket_tol;
pocket_w   = pcb_w + pocket_tol * 2;
pocket_h   = pcb_h + pocket_tol * 2;

// Frame depths
front_d    = wall + glass_d + pcb_d + 0.5;        // ~5.3mm
back_d     = 14.0;                                 // Interior depth

// Friction-fit lip dimensions
fit_lip_h  = 2.0;           // Height of friction ridge
fit_lip_t  = 0.8;           // Thickness of ridge wall
fit_tol    = 0.1;           // Interference (tight fit)

// ───────────────────────────────────────────
//  RENDER CONTROL
// ───────────────────────────────────────────
// "front", "back", "easel_front", "easel_back", "all"
render_part = "all";

// ───────────────────────────────────────────
//  EASEL PARAMETERS
// ───────────────────────────────────────────
leg_section    = 5.0;
leg_height     = 135.0;
bottom_spread  = 85.0;       // Wider than frame for stability
top_spread     = 24.0;
shelf_h        = 12.0;
shelf_lip      = 9.0;        // +2mm from v3 (was 7mm)
shelf_thick    = 3.5;

back_leg_len   = 100.0;
back_leg_w     = 10.0;       // Slightly wider for cross-slot strength
back_leg_d     = 4.0;

// Cross-slot joint dimensions
slot_tol       = 0.3;        // Tolerance for cross-slot fit

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
//  • Chin (left) bezel = 12mm, right = 11mm
//  • PCB pocket 5% oversize for adjustment play
//  • 4 corner feet for even display seating
//  • Bottom extended 10mm (cables route inside)
//  • No external harness holes
//  • Prints face DOWN, no supports
//
module front_frame() {
    difference() {
        // Solid frame body
        rrect(frame_w, frame_h, front_d, corner_r);

        // Display window — centered on active area
        translate([win_x, win_y, -0.1])
            rrect(win_w, win_h, front_d + 0.2, 1.5);

        // PCB pocket — 5% oversize for adjustment play
        // User can slide display L/R up to ~2mm before gluing
        translate([pocket_x, pocket_y, wall])
            cube([pocket_w, pocket_h, front_d]);
    }

    // ── Four corner feet ──
    // Small pads so display sits equidistant from face
    // Height = pocket depth - display module thickness
    foot_sz = 5.0;
    foot_h  = max(0.5, front_d - wall - glass_d - pcb_d);

    // Bottom-left foot
    translate([pocket_x + 2, pocket_y + 2, wall])
        cube([foot_sz, foot_sz, foot_h]);
    // Bottom-right foot
    translate([pocket_x + pocket_w - foot_sz - 2, pocket_y + 2, wall])
        cube([foot_sz, foot_sz, foot_h]);
    // Top-left foot
    translate([pocket_x + 2, pocket_y + pocket_h - foot_sz - 2, wall])
        cube([foot_sz, foot_sz, foot_h]);
    // Top-right foot
    translate([pocket_x + pocket_w - foot_sz - 2,
               pocket_y + pocket_h - foot_sz - 2, wall])
        cube([foot_sz, foot_sz, foot_h]);
}

// ═══════════════════════════════════════════════════════════
//  2. BACK SHELL
// ═══════════════════════════════════════════════════════════
//
//  • Friction-fit ridge mates with front frame
//  • ESP32 in upper half with cable gaps on all 4 walls
//  • ESP32 corner feet reduced to 3mm (50%)
//  • All vents uniform 4mm tall, evenly spaced
//  • Vent behind ESP32 guaranteed
//  • No external harness holes (cables route inside)
//  • Prints open-side UP, no supports
//
module back_shell() {
    shell_d = back_d + wall;

    // Vent layout: 6 evenly spaced, all 4mm tall
    vent_count  = 6;
    vent_h      = 4.0;
    vent_margin = 10.0;
    vent_range  = frame_h - vent_margin * 2;
    vent_spacing = vent_range / (vent_count - 1);

    // ESP32 position: CENTER it on vent slot #3 (0-indexed)
    // so one vent is directly behind the chip
    vent3_y     = vent_margin + 3 * vent_spacing;
    esp_center_y = vent3_y + vent_h / 2;
    esp_x = (frame_w - esp_w) / 2;
    esp_y = esp_center_y - esp_h / 2;

    bracket_h = 9.0;
    bracket_t = 2.5;

    // ESP32 corner feet dimensions
    foot   = 4.0;
    foot_h = 3.0;

    difference() {
        // Outer shell
        rrect(frame_w, frame_h, shell_d, corner_r);

        // Hollow interior
        translate([wall, wall, wall])
            rrect(frame_w - wall*2, frame_h - wall*2,
                  shell_d, corner_r - 0.5);

        // ── Evenly spaced vents (all same size) ──
        // Vent #3 is directly behind ESP32 center
        // That vent is narrower to avoid cutting under the feet
        for (i = [0 : vent_count - 1]) {
            vy = vent_margin + i * vent_spacing;
            if (i == 3) {
                // ESP32 vent — narrower, fits between the feet
                // Feet are at esp_x and esp_x+esp_w-foot
                // Vent goes from foot right edge to next foot left edge
                translate([esp_x + foot + 1, vy, -0.1])
                    cube([esp_w - foot*2 - 2, vent_h, wall + 0.2]);
            } else {
                // Standard full-width vent
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

    // ── ESP32 wall brackets ──
    usb_gap = 12.0;

    // Left bracket (split with cable gap)
    side_half = (esp_h + 2 - usb_gap) / 2;
    translate([esp_x - bracket_t, esp_y - 1, wall])
        cube([bracket_t, side_half, bracket_h]);
    translate([esp_x - bracket_t,
               esp_y - 1 + esp_h + 2 - side_half, wall])
        cube([bracket_t, side_half, bracket_h]);

    // Right bracket (split with cable gap)
    translate([esp_x + esp_w, esp_y - 1, wall])
        cube([bracket_t, side_half, bracket_h]);
    translate([esp_x + esp_w,
               esp_y - 1 + esp_h + 2 - side_half, wall])
        cube([bracket_t, side_half, bracket_h]);

    // Bottom bracket (split with cable gap)
    bot_half = (esp_w - usb_gap) / 2;
    translate([esp_x, esp_y - bracket_t, wall])
        cube([bot_half, bracket_t, bracket_h]);
    translate([esp_x + esp_w - bot_half,
               esp_y - bracket_t, wall])
        cube([bot_half, bracket_t, bracket_h]);

    // Top bracket (split with cable gap)
    translate([esp_x, esp_y + esp_h, wall])
        cube([bot_half, bracket_t, bracket_h]);
    translate([esp_x + esp_w - bot_half,
               esp_y + esp_h, wall])
        cube([bot_half, bracket_t, bracket_h]);

    // ── ESP32 corner feet (3mm tall) ──
    // Attached to back plate (z=wall = inner surface of back wall)
    // Positioned at corners AWAY from the center vent
    // so they rest on solid back plate, not over the vent slot
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
//
//  • Two front legs converging to top crossbar
//  • Shelf lip extended to 9mm
//  • Cross-slot joint at top for back leg
//  • Prints lying on BACK, no supports
//

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
            // Two front legs
            easel_front_leg(0);
            easel_front_leg(1);

            // Bottom crossbar + shelf
            translate([0, 0, shelf_h])
                cube([bottom_spread, leg_section, shelf_thick]);

            // Shelf lip (extends forward — frame rests on this)
            translate([0, -shelf_lip, shelf_h])
                cube([bottom_spread,
                      shelf_lip + leg_section,
                      shelf_thick / 2]);

            // Front lip ridge (prevents frame from sliding off)
            translate([0, -shelf_lip, shelf_h])
                cube([bottom_spread, 2.5, shelf_thick + 4]);

            // Top crossbar
            translate([top_x_left, 0, leg_height - leg_section])
                cube([top_spread, leg_section, leg_section]);
        }

        // ── Cross-slot: cut from TOP of crossbar ──
        // Slot goes halfway down for interlocking with back leg
        slot_w = back_leg_d + slot_tol;
        slot_depth = leg_section / 2 + 0.5;
        translate([top_x_left + (top_spread - slot_w) / 2,
                   -0.1,
                   leg_height - slot_depth])
            cube([slot_w, leg_section + 0.2, slot_depth + 0.1]);
    }
}

// ═══════════════════════════════════════════════════════════
//  3b. EASEL BACK LEG — with cross-slot tab
// ═══════════════════════════════════════════════════════════
//
//  • Interlocking cross-slot at top joins to A-frame
//  • Wider foot for stability
//  • Prints FLAT, no supports
//

module easel_back_leg() {
    difference() {
        union() {
            // Main leg body
            rrect(back_leg_w, back_leg_len, back_leg_d, 1.5);

            // Wider foot at bottom
            hull() {
                translate([0, 0, 0])
                    rrect(back_leg_w, 5, back_leg_d, 1.5);
                translate([-4, -3, 0])
                    rrect(back_leg_w + 8, 5, back_leg_d, 1.5);
            }
        }

        // ── Cross-slot: cut from TOP end (bottom of leg in print) ──
        // Goes halfway through thickness for interlocking
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
    // Front frame — display faces viewer (XY plane)
    color("WhiteSmoke", 0.9)
        front_frame();

    // Back shell — behind front frame
    translate([0, 0, -(back_d + wall)])
        color("LightGray", 0.7)
            back_shell();

    // Ghost display for reference
    color("DarkSlateGray", 0.3)
        translate([win_x + overlap, win_y + overlap, -0.3])
            cube([disp_w - overlap*2, disp_h - overlap*2, 0.3]);

    // Easel A-frame — behind and below
    easel_x = (frame_w - bottom_spread) / 2;
    easel_z = -(back_d + wall + 3);
    translate([easel_x, -shelf_lip + 2, easel_z])
        rotate([-8, 0, 0])
            color("BurlyWood", 0.8)
                easel_front();

    // Back leg — angled backward, cross-slot joined
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
    // "all" — assembly preview
    assembly();
}
