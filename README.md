# Smart Beam

**A responsive lighting solution that activates based on human movement.**

Smart Beam is an automatic motion-sensing lighting system built with Arduino.
The room is divided into zones — bed area, cupboard area, and study area —
and each zone has its own HC-SR04 ultrasonic sensor and relay-driven light.
When a person enters a zone, that zone's light turns ON automatically; when
they leave, it turns OFF. No manual switching needed.

## Features

- Per-zone automatic lighting — each area responds independently
- Adjustable detection range (default 50 cm)
- Continuous monitoring every 200 ms
- Energy efficient — lights are on only where someone is present
- Scalable — supports any number of zones (demo used 3)
- Simple, low-cost hardware (all components are common and inexpensive)

## Hardware

| Component        | Quantity | Notes                                |
|------------------|----------|--------------------------------------|
| Arduino Uno      | 1        |                                      |
| HC-SR04 sensor   | 1 per zone | Ultrasonic distance sensor         |
| Relay module     | 1 per zone | 5V relay, drives the light/load    |
| LED / bulb       | 1 per zone |                                |
| Jumper wires     | several  |                                      |
| USB cable        | 1        | For power / programming              |

## Circuit / Wiring

Each zone uses one HC-SR04 and one relay. Pin assignments used in the code:

| Zone          | HC-SR04 TRIG | HC-SR04 ECHO | Relay IN |
|---------------|--------------|--------------|----------|
| Bed Area      | D2           | D3           | D4       |
| Cupboard Area | D5           | D6           | D7       |
| Study Area    | D8           | D9           | D10      |

For every sensor: **VCC → 5V, GND → GND**.

Each light is wired through its relay's **COM** and **NO** (Normally Open)
contacts, so the Arduino only switches a low-voltage control signal while the
relay handles the actual load.

## How It Works

1. **Initialize** — `setup()` configures TRIG/ECHO/relay pins for every zone,
   starts serial monitoring at 9600 baud, and forces all lights OFF.
2. **Measure distance** — for each enabled zone, `measureDistance()` sends a
   10 µs pulse on TRIG. The sensor emits an ultrasonic burst and raises ECHO
   for as long as the echo takes to return. `pulseIn()` measures that HIGH
   time (30 ms timeout).
3. **Calculate distance** — sound travels ~0.034 cm per microsecond, and the
   measured time covers a round trip, so `distance = duration * 0.034 / 2`.
4. **Zone control** — if `0 < distance <= 50 cm`, the zone is "occupied" and
   its relay turns its light ON. Otherwise the light turns OFF. Because each
   zone is tracked independently (`zoneOccupied[]`), the bed and cupboard
   lights stay off while only the study light responds.
5. **Repeat** — `loop()` runs every 200 ms, continuously monitoring all
   enabled zones.

## Zone Configuration

Zones are defined at the top of the sketch — name, pins, and an on/off flag:

```cpp
const char* zoneNames[NUM_ZONES] = { "Bed Area", "Cupboard Area", "Study Area" };
const int trigPins[NUM_ZONES]  = { 2, 5, 8 };
const int echoPins[NUM_ZONES]  = { 3, 6, 9 };
const int relayPins[NUM_ZONES] = { 4, 7, 10 };

// Demo model: only the study area was active
const bool zoneEnabled[NUM_ZONES] = { false, false, true };
```

To activate more zones (e.g., the full model with a sensor per area), set
their flag to `true` — or add more entries to the arrays. `NUM_ZONES` is
computed from the array size, so no other changes are needed.

## Uploading to Arduino

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Connect the Arduino Uno via USB.
3. Open `motion_sensor_light/motion_sensor_light.ino`.
4. Select **Tools > Board > Arduino Uno** and the correct **Port**.
5. Click **Upload**.
6. Open **Tools > Serial Monitor** (9600 baud) to watch zone
   detections live, e.g.:

```
Smart Beam - Zoned Motion Lighting Ready
Detection range: 50 cm per zone
Study Area - Motion Detected! Distance: 32 cm - LIGHT ON
Study Area - Clear. - LIGHT OFF
```

## Project Synopsis

This project demonstrates an automatic motion-sensing light system using
Arduino that improves energy efficiency, safety, and automation. By dividing
a room into zones — bed area, cupboard area, and study area — each with its
own sensor and light, the system lights only the area a person is actually
in.

**Advantages:**

- Saves energy — lights turn ON only where motion is detected.
- Fully automatic — no manual switching required.
- Zone-aware — different areas are controlled independently.
- Easy to use and simple to implement.
- Enhances safety by lighting up areas when someone enters.
- Cost-effective — uses basic, easily available components.

## Authors

- Shazia Fayaz

*Teammates can be added as collaborators on GitHub — their contributions
will show on their own profiles.*
