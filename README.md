# Smart Beam

> **"A responsive lighting solution that activates based on human movement."**

Smart Beam (aka **Motion Sensing Light**) is an automated lighting system
designed using **Object-Oriented Programming** concepts. An ultrasonic sensor
detects the presence or movement of a person; when motion is detected the
light automatically turns ON, and when no motion is sensed the light turns
OFF. The project demonstrates efficient energy usage and the practical
application of OOP principles in real-life systems.

The room is divided into zones — **bed area, cupboard area, and study area** —
and each zone has its own sensor and relay-driven light, so only the area a
person is actually in gets illuminated.

## Features

- Automatic ON/OFF lighting based on detected motion — no manual switching
- Per-zone control — bed, cupboard, and study areas respond independently
- Adjustable detection range (default 50 cm) and check interval (200 ms)
- Energy efficient — a light is on only where someone is present
- Built with OOP — each zone is a self-contained `MotionZone` object
- Simple, low-cost hardware (all components are common and inexpensive)

## Hardware Components

### HC-SR04 Ultrasonic Sensor
The main detector. It emits an ultrasonic burst and measures the time taken
for the echo to return, giving the distance to the nearest object. When an
object comes within the preset distance range, it signals motion to the
Arduino.

### Arduino Uno
The main controller. It receives distance data from the ultrasonic sensor,
decides when to activate the relay, and sends the signal indicating motion.
Based on this decision it turns the light ON or OFF automatically.

### Relay Module
Used to control the light safely. When Arduino sends a signal, the relay
switches ON the light; when no motion is detected, it switches the light OFF.
The relay lets the low-voltage Arduino circuit safely control the light load.

### Light Bulb / LED
The output device — it illuminates only while its zone is occupied.

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

## Working Principle

```
        START
          │
          ▼
  Sensor Sends Pulse
          │
          ▼
   Echo Received ──NO──► (timeout, retry)
          │ YES
          ▼
  Distance Calculated
          │
          ▼
 Is Distance < 50 cm? ──NO──► LED OFF
          │ YES
          ▼
        LED ON
          │
          ▼
   Repeat Process (every 200 ms)
```

1. **Initialize** — `setup()` begins serial monitoring at 9600 baud and calls
   `begin()` on every zone to configure pins and force lights OFF.
2. **Measure distance** — each zone sends a 10 µs pulse on TRIG. The sensor
   emits an ultrasonic burst and raises ECHO for exactly as long as the echo
   takes to return. `pulseIn()` measures that HIGH time (30 ms timeout).
3. **Calculate distance** — sound travels ~0.034 cm per microsecond, and the
   measured time covers a round trip, so `distance = duration * 0.034 / 2`.
4. **Detect motion** — if `0 < distance <= 50 cm`, the zone is occupied and
   its relay turns its light ON; otherwise the light turns OFF. The
   `occupied` flag ensures switching only happens on state changes, avoiding
   constant relay chatter.
5. **Repeat** — `loop()` updates every zone every 200 ms for continuous,
   independent monitoring.

## Code Explanation

The sketch is organized around a single OOP class:

**1. Class `MotionZone`** — encapsulates everything about one area: its name,
pins, enabled flag, occupancy state, and the logic to measure distance,
detect motion, and report status over Serial.

**2. `begin()`** — sets TRIG as OUTPUT, ECHO as INPUT, relay as OUTPUT, and
ensures the light starts OFF.

**3. `update()`** — the per-zone main logic: skip disabled zones, measure
distance, compare against `DETECTION_DISTANCE`, and switch the relay only on
state changes (`occupied` flag prevents constant switching).

**4. `measureDistance()`** — sends the 10 µs trigger pulse, times the echo
with `pulseIn()`, returns −1 on timeout, otherwise converts microseconds to
centimeters.

**5. `setup()` / `loop()`** — `setup()` starts Serial and initializes all
zones; `loop()` calls `update()` on each zone and delays 200 ms.

### Zone configuration

Zones are defined as objects at the top of the sketch — name, pins, and an
enable flag:

```cpp
MotionZone zones[NUM_ZONES] = {
  MotionZone("Bed Area",      2, 3, 4,  false),
  MotionZone("Cupboard Area", 5, 6, 7,  false),
  MotionZone("Study Area",    8, 9, 10, true )
};
```

The demo model used a single sensor covering the study area, so only that
zone is enabled. For the full model with a sensor per area, set the flags to
`true` — or add more `MotionZone` entries. No other code changes are needed.

## Uploading to Arduino

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Connect the Arduino Uno via USB.
3. Open `motion_sensor_light/motion_sensor_light.ino`.
4. Select **Tools > Board > Arduino Uno** and the correct **Port**.
5. Click **Upload**.
6. Open **Tools > Serial Monitor** (9600 baud) to watch zone detections live:

```
Smart Beam - Zoned Motion Lighting Ready
Detection range: 50 cm per zone
Study Area - Motion Detected! Distance: 32 cm - LIGHT ON
Study Area - Clear. - LIGHT OFF
```

## Advantages

| # | Advantage | Description |
|---|-----------|-------------|
| 01 | **Energy Saving** | LED only turns on when motion is detected, reducing electricity usage. |
| 02 | **Automatic Operation** | No need to manually switch the light ON/OFF. |
| 03 | **Easy to Use** | Simple setup with Arduino and ultrasonic sensor. |
| 04 | **Enhances Safety** | Lights up when someone enters a room or area, preventing accidents. |
| 05 | **Cost-Effective** | Uses basic components like Arduino, relay, and LED. |

## Future Scope

- **Time Tracking** — monitor light ON duration to optimize energy usage.
- **Smart Control** — control lights remotely via app or IoT devices.
- **Adjustable Sensitivity** — set detection range and response time.
- **Wider Coverage** — use multiple or movable sensors for large areas.

## Project Synopsis

This project demonstrates an automatic motion-sensing light system using
Arduino that improves energy efficiency, safety, and automation. By dividing
a room into zones — bed area, cupboard area, and study area — each with its
own sensor and light, the system illuminates only the area a person is
actually in, while the OOP-based design keeps the code modular and scalable.



