# Trashbot — Portfolio Documentation

---

## Project Management

### High-Level Objectives

The Trashbot project was born out of a very real, very personal frustration: dealing with trash is gross, inconvenient, and surprisingly disruptive to daily life. The goal of this project is to build a smart, motorized carrier that can transport a standard residential trashcan — autonomously or via remote control — so that the chore of taking out the trash is no longer something a human has to think about or physically do.

Phase 1 objective: a remote-controlled trashcan carrier. The user can stand at a distance, use a handheld wireless controller, and drive the trashcan to the curb and back without ever touching it.

Phase 2 objective: full automation. The carrier will know what day and time it is, know the trash pickup schedule, and autonomously take the trashcan out and bring it back in — without any human input at all.

---

### Timeline

**Phase 1: February 2026 – April 2026** (approx. 3 months, part-time)
- Month 1: Research, component sourcing, procurement, returns/exchanges, and lead time
- Month 2: Design, build, and testing
- Month 3: Documentation, demos, and polish

**Phase 2: May 2026 – July 2026** (approx. 3 months, part-time)
- Same monthly breakdown: research → build → document

---

### Budget & Procurement

Target budget for Phase 1: **~$150**

Major cost drivers:
- Battery: ~$60 per unit
- ESP32 dev boards: ~$25–30 for 3–4 boards
- Motor controllers (ZS-X11HV2): ~$20–25

The rest of the budget covers the wooden platform, caster wheel, 3D printing filament, screws, spacers, wiring, and miscellaneous hardware. Sourcing smart (e.g., Goodwill for the hoverboard at $2) was a key strategy to keep costs down.

---

### MVP

The MVP is deliberately narrow: a remote-controlled carrier that can move a trashcan from point A to point B without the user touching the trashcan. No automation, no scheduling, no sensors — just reliable, controllable movement. Once that is proven, Phase 2 layers intelligence on top.

---

### Quality Evaluations

*[Placeholder — to be filled in with testing criteria, pass/fail metrics, and demo results]*

---

---

## The Origin Story

This project started with frustration. Not one frustration — many, stacked on top of each other, building up over time until the only reasonable response was to engineer a solution.

Here's the full picture of what made trash so unbearable:

**Touching the trashcan is gross.** There's trash water, residue, and general filth on and around it. Nobody wants to put their hands on it.

**The timing is a nightmare.** The trash company comes three times a week: trash pickup on Tuesday and Friday mornings, recycling on Wednesday morning. You can't put the can out too early — neighbors don't appreciate an ugly trashcan sitting on the curb since Monday — but you also can't put it out too late, because the pickup truck doesn't give you a warning call. Sometimes they come early, sometimes they don't. So the window is roughly late Monday night or early Tuesday morning, with no precise timing.

**We forget.** Life is busy. When you miss a pickup day, trash piles up. And when trash piles up, you're forced to take everything out at once — including recycling mixed in with trash — which defeats the purpose of recycling in the first place. Environmental guilt on top of logistical failure.

**The return trip is just as bad.** After a long day at work — heavy backpack, groceries, tired — the trashcan is sitting outside in the Arizona sun. It has been baking all day. It smells. It's dirty. And it needs to be dragged back to the backyard before you can even go inside and rest. Nobody has the energy or the extra hands for that.

**Manual systems don't work.** Whiteboards with schedules, notes about whose turn it is, reminders — all of it created conflict. Someone would forget, expectations weren't met, and the frustration shifted from "trash is annoying" to "why don't we have a system" to arguments. And if you were the one who forgot, the guilt was its own problem.

After many conversations about how to solve this, the realization finally clicked: the problem isn't about habits or reminders. The problem is that the task itself is unpleasant enough that no habit will stick reliably. The solution has to remove the task entirely — or at minimum, make it so frictionless and even fun that it stops being a source of stress.

That's where the idea of remote control came in. If you can stand at the front door and drive the trashcan to the curb with a joystick — without ever stepping outside in the dark, without touching anything gross — it stops being a chore and starts being almost entertaining. And once the remote-control layer is solid, automating the whole thing becomes the obvious next step.

---

---

## Mechanical

### Design Space & Design Approach

#### Raw Ideas

The first instinct was simple: motorize the trashcan itself. Replace its existing wheels with motorized ones, add a battery and controller at the bottom, and go.

That idea fell apart quickly once the constraints became clear:

- The trashcan has almost no clearance at the bottom. There's no room to attach hardware underneath without major modifications to the can itself.
- The wheel attachment points on the trashcan are specific, and finding motorized wheels that match that geometry is difficult and limiting.
- In Arizona, any hardware mounted on the side or bottom of the trashcan would be exposed to intense sun. Batteries and electronics in direct desert sun degrade fast.
- Attaching hardware to the plastic trashcan body directly is a problem — screwing into plastic is unreliable, adhesives fail in high heat, and it's just gross to be fastening things to a trash receptacle.
- The biggest problem: trash water and rain. If the hardware is mounted on the trashcan, washing the can becomes complicated. Replacing the trashcan (which happens eventually) means removing and reinstalling all the hardware. That's gross, inefficient, and defeats the whole purpose of keeping the system clean and reusable.

The pivot was straightforward: instead of modifying the trashcan, build a separate carrier platform that the trashcan sits on top of. Clean separation between dirty and smart. The trashcan stays a trashcan. The carrier does all the work.

#### Considerations in 3D Printing

The hoverboard motor wheels have a small hollow metal tube at their center (where the wires run through). The original hoverboard mounting hardware doesn't fit flat against the wooden platform due to geometric mismatch. Additionally, the motorized wheels and the front caster wheel needed to all sit at the same height so the platform would be level — a requirement for stability when carrying a heavy, top-heavy trashcan.

The solution was to design and 3D print solid adapter cubes. Each cube:
- Has a half-pipe/half-cylinder cutout on top for the wheel's metal tube to rest in
- Is sized to match the footprint of the hoverboard's metal mounting plate (which comes with each wheel)
- Has 4 holes that align with the 4 screw holes on the metal plate
- Long screws and spacers pass through the platform board, the cube, and the metal plate to clamp everything together

The cubes were printed solid (non-hollow) intentionally — no risk of cracking under the load of a full trashcan.

This design worked on the first print. No iteration needed.

---

### Tools: SolidWorks

*[Placeholder — expand with phases of design, working in assembly mode, configurations, and prototype iterations. Include screenshots and renders.]*

---

### Designs

#### Chassis

**Platform:** A 24-inch round pine wood board, purchased at a hardware store as a slightly imperfect board for under $10. The round shape distributes weight evenly and gives clearance on all sides for the wheel assemblies.

**Caster wheel:** Before designing anything in SolidWorks, the caster wheel was selected first. This was a deliberate sequencing choice — the caster wheel's height determines the ground clearance, which in turn determines how tall the 3D-printed adapter cubes need to be. A 6-inch rounded-profile caster wheel was selected. The rounded profile (vs. a flat-cylinder edge) was chosen by physically rolling both types across the floor at the hardware store — the rounded edge had lower rolling resistance on rough concrete, which matters for a driveway surface. The 6-inch height also provides enough ground clearance for the electronics mounted underneath and to roll over cracks and surface irregularities.

**Assembly sequence:**
1. Selected and purchased the 6-inch caster wheel
2. Mounted it on the platform and measured the resulting ground-to-platform height
3. Measured the height of the hoverboard motor wheel assemblies
4. Calculated the cube dimensions needed to bring the motor wheels up to the same height as the platform
5. Designed the cubes in SolidWorks
6. 3D printed and assembled — worked on first try

**Motor wheels:** Salvaged from a used hoverboard purchased at Goodwill for $2. Hoverboards are frequently donated due to dead batteries, while the motors themselves remain functional. The battery was confirmed dead via multimeter. The hall sensors (which detect wheel rotation for motor control) were confirmed working by manually spinning the wheels and feeling the expected magnetic resistance. A bench test with a spare ~24V battery confirmed the motors spun correctly when powered.

The hoverboard came with two brushless hub motors, two motor controllers (not reused — see Electronics section), the wiring harness, and the metal mounting plates used in the cube assembly.

#### Handheld Controller

*[Placeholder — revisit with photos and SolidWorks renders]*

#### Phase 2: Gate Opener / Automated System

*[Placeholder — end-of-document section, after Phase 1 is complete]*

---

---

## Electronics

### Basics

*[Placeholder — expand with fundamentals covered during the project: PWM, motor control logic, ADC, voltage levels, etc.]*

---

### Procurement

| Component | Source | Cost | Notes |
|---|---|---|---|
| Hoverboard (for motors + wiring) | Goodwill | $2 | Dead battery, working motors |
| ESP32 dev boards (x3) | Amazon | ~$25–30 | Used dev boards with breakout pins for prototyping |
| Motor controllers ZS-X11HV2 (x2, then x2 more) | Amazon | ~$25 first set, ~$27 second set | First set: one burnt due to modified ESP8266 wiring issue |
| 6-inch caster wheel | Ace Hardware | ~$8–10 | Rounded profile, selected in-store |
| 24V round pine wood board | Hardware store | <$10 | Slightly imperfect board, discounted |

**Note on hoverboard motor controllers:** The hoverboard came with its own motor controllers, but these were not reused. Hoverboard controllers run proprietary firmware — there is no accessible way to reprogram them without reverse-engineering the system, which could involve cracking encrypted firmware or finding undocumented protocols. Rather than spending time on that rabbit hole, off-the-shelf brushless motor controllers (ZS-X11HV2, 6–60V, 400W) were purchased for ~$25. This kept the project moving and gave full control over the motor behavior through clean, documented PWM inputs.

---

### Testing Firmware / Unit Tests

#### Philosophy

The approach to electronics testing mirrored the approach to software unit testing: start with the smallest possible isolated test, confirm it works, then build up. Rather than wiring everything together and debugging a complex system, each component was validated independently first.

The small-scale test rig consisted of:
- 1x ESP32 dev board
- 1x small Arduino DC motor
- 1x H-bridge motor driver
- 1x KY-023 joystick module

This let the motor control logic and wireless communication be validated before touching the actual hoverboard wheel system.

---

#### Session Notes: March 3, 2026

**1. Joystick wiring bug (fixed)**

One joystick axis wasn't reading correctly. Initially suspected the joystick module itself, and tried adding a second joystick (one for X, one for Y). The problem persisted. Used a multimeter to check the Y pin voltage while moving the joystick on the X axis — the voltage was changing when it shouldn't have been. Traced it back to the Vcc pin being wired to 5V instead of 3.3V. Lesson: always check the datasheet for voltage requirements before wiring.

**2. ESP-NOW working between two ESP32 boards**

Successfully sent joystick data wirelessly using ESP-NOW. Key learnings:
- Used `memcpy()` to unpack received data bytes back into a struct
- Sending every 20ms (50 packets/second) is more than enough for smooth DC motor control
- ESP-NOW uses the WiFi radio hardware but doesn't need a router — direct board-to-board communication
- Both boards must be on the same WiFi channel. Used the NetSpot app to find the least congested channel after other methods didn't work
- Master needs the slave's MAC address to send; slave just listens
- ESP-NOW max payload is 250 bytes per packet — joystick X/Y using `uint16_t` is only 4 bytes total, well within limits
- `uint16_t` (2 bytes, max 65535) is the most efficient type for 12-bit ADC values (0–4095)

**3. Joystick → motor control logic**

X axis controls direction (left/right turning), Y axis controls speed and forward/reverse. Key learnings:
- Used a deadzone (~40–60 on a mapped 0–100 scale) to prevent motor twitching at idle
- `map()` with a flipped output range handles reverse direction speed correctly
- Cannot pass `y=0` to `set_speed()` because it triggers full reverse — use `ledcWrite(speedPin, 0)` directly to stop motors
- PWM resolution set to 16-bit (0–65535) on ESP32 — this is a software configuration, not a hardware ceiling. 8-bit would also work
- ADC2 pins are unsafe to use when any wireless protocol (WiFi, ESP-NOW, Bluetooth) is active — must use ADC1 pins (GPIO 32–39) for analog reads

**4. C/C++ concepts refreshed**
- Pointers: `&var` gets the address, `*ptr` dereferences it
- Arrays decay to pointers automatically when passed to functions
- `(uint8_t*)&myStruct` — `&` gets the address, `(uint8_t*)` casts it so ESP-NOW can treat it as raw bytes
- Struct variables declared inside a function are local scope only — declare at top level if needed across functions
- `map()` at global scope runs before Arduino initializes — declare globally but assign inside `setup()`
- Forward declarations (prototypes) let you define functions after they're called
- `sizeof()` returns the byte size of any variable or type

**5. arduino-cli workflow**
- `arduino-cli board list` to find connected board ports
- Compile once, upload to multiple boards separately using different `-p` port flags
- `sketch.yaml` must exist in the project folder with the correct `default_fqbn` or LSP won't work
- `arduino-cli monitor -p <port> --config 115200` to open serial monitor

---

#### Session Notes: March 5, 2026

**Switched from H-bridge to brushless motor controllers**

Moved from the small test H-bridge to the real hoverboard motor controllers (ZS-X11HV2, 6–60V, 400W).

Key specs:
- PWM input range: 2.5–5V amplitude, 50Hz–20kHz frequency
- Direction control: LOW level is active (REVERSE_ON = LOW, REVERSE_OFF = HIGH)
- Brake: HIGH level active

*[Placeholder — expand with full wiring details, pin mapping, and bench test results for the real motor controllers]*

---

#### The Burnt Motor Controller

Before the unit test phase with the ESP32 dev boards, an earlier attempt was made using a modified ESP8266 (borrowed from a previous project). The ESP8266 had some pins already pre-connected internally — a modification that wasn't visually obvious. During wiring, the wrong pins were connected together, which sent incorrect signals to the motor controller and burnt it out.

The debugging process:
1. Loaded the program — wheel didn't respond as expected
2. Broke the code down into minimal unit tests — still no correct response
3. Researched the motor controller specs — found that the stop pin on these Chinese-made controllers doesn't work as documented. No clear datasheet available. Decided to drop the stop function and use the brake function instead — which works fine
4. Used a multimeter to check each pin on the motor controller for correct voltage output (high/low) per the program
5. Checked continuity — discovered some pins on the ESP8266 were internally connected due to the pre-modification, causing incorrect wiring

After the burnt controller and the difficulty of working around an unknown hardware modification, the ESP8266 was retired. The ESP32 was chosen as the replacement — newer, more capable, and critically, unmodified. Three ESP32 dev boards were purchased from Amazon (with breakout pin headers for easy prototyping without soldering).

---

### Tools: KiCad + Wiring Diagram

*[Placeholder — revisit with full schematic, KiCad screenshots, and wiring diagram. Connect to the motor controller specs and ESP32 pin assignments documented above.]*

---

---

## Software

### Nvim Environment

Arduino IDE was the obvious starting point — it's what most people use for ESP32 and Arduino projects. But early on, a decision was made to move everything to Neovim instead and use it as the single development environment for the entire project.

The motivation: consistency. Rather than context-switching between Arduino IDE for firmware, a text editor for notes, and something else for exploring documentation, Neovim handles all of it. The learning curve was real, but once past it, the payoff was significant — a deeper understanding of the underlying toolchain (compilers, serial monitors, LSP, build systems) rather than just clicking buttons in a GUI. Neovim became the environment for writing code, taking notes, testing, and exploring.

---

### Arduino → Nvim Environment

The bridge between Neovim and Arduino development is `arduino-cli` — a command-line tool that handles everything the Arduino IDE normally does through a GUI.

Key workflow commands:
- `arduino-cli board list` — find connected boards and their ports
- `arduino-cli compile --fqbn <board>` — compile the sketch
- `arduino-cli upload -p <port> --fqbn <board>` — upload to a board
- Compile once, upload to multiple boards separately with different `-p` flags (useful for flashing both master and slave ESP32s)
- `sketch.yaml` in the project folder with the correct `default_fqbn` is required for LSP (language server) to work correctly in Neovim
- `arduino-cli monitor -p <port> --config 115200` — open serial monitor for debugging output

---

### Unit Tests

The unit testing philosophy in software directly mirrors the approach used in electronics: isolate the smallest possible piece of behavior, verify it works, then build up. The small-scale test rig (DC motor + H-bridge + joystick + ESP32) was the physical embodiment of a software unit test — one input, one output, controlled conditions.

*[Placeholder — expand with specific test cases, test code structure, and how tests were organized as the project scaled up]*

---

### GitHub

*[Placeholder — final item. All organized project documentation, code, schematics, and notes will be published here.]*

---

---

## Why ESP-NOW?

ESP-NOW was chosen as the wireless communication protocol between the handheld controller and the carrier for several reasons:

- **It comes built into the ESP32.** No additional hardware, no extra modules, no added cost.
- **The ESP32 is cheap and widely available.** Multiple boards can be bought from Amazon for experimentation without worrying about the cost of mistakes.
- **Range is sufficient.** Community testing and YouTube demonstrations show reliable communication at 10+ meters, which easily covers the distance from a front door to the curb.
- **Low latency.** Fast enough for real-time motor control with 20ms packet intervals (50 packets/second).
- **No router required.** ESP-NOW is direct board-to-board — no WiFi network needed, which means it works even if the home network is down or congested.
- **Well-documented by the community.** Plenty of real-world examples, videos, and forum posts show it working reliably in similar projects.
- **Room to grow.** ESP-NOW supports two-way communication and one-master-to-multiple-slaves topologies, which could be useful in later phases without requiring a protocol change.

---

---

## Demos

### Videos & Images

*[Placeholder — add build progress photos, assembly images, and demo videos here once Phase 1 is complete]*

### Documents & Notes

- Session notes from March 3, 2026: joystick bug, ESP-NOW setup, motor control logic, C++ refresher, arduino-cli workflow
- Session notes from March 5, 2026: transition from H-bridge to brushless motor controllers, ZS-X11HV2 specs

---

---

## Phase 2: Gate Opener / Automated System

*[Placeholder — to be written after Phase 1 is complete. Phase 2 will cover full automation: scheduled operation, autonomous navigation from backyard to curb and back, and potentially integration with a gate opener system for homes with enclosed backyards.]*

---
