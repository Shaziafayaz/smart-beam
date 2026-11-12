// ============================================================
// Smart Beam - Zoned Automatic Motion-Sensing Lighting
// Arduino Uno + HC-SR04 Ultrasonic Sensors + Relays + LEDs
//
// The room is divided into zones (bed area, cupboard area,
// study area). Each zone has its own ultrasonic sensor and
// relay-driven light. When a person enters a zone, that zone's
// light turns ON; when they leave, it turns OFF automatically.
//
// Demo model used a single sensor covering the study area.
//
// Author: Shazia Fayaz
// ============================================================

// ---------- Zone definitions ----------
// Each zone: name, TRIG pin, ECHO pin, RELAY pin
const int NUM_ZONES = 3;

const char* zoneNames[NUM_ZONES] = { "Bed Area", "Cupboard Area", "Study Area" };
const int trigPins[NUM_ZONES]  = { 2, 5, 8 };   // HC-SR04 TRIG pins
const int echoPins[NUM_ZONES]  = { 3, 6, 9 };   // HC-SR04 ECHO pins
const int relayPins[NUM_ZONES] = { 4, 7, 10 };  // Relay IN pins

// Enable/disable zones. Demo model: only the study area was used.
const bool zoneEnabled[NUM_ZONES] = { false, false, true };

// ---------- Detection parameters ----------
const int DETECTION_DISTANCE = 50;  // Detection range in cm
const int CHECK_INTERVAL = 200;     // Check every 200 ms

bool zoneOccupied[NUM_ZONES] = { false, false, false };

// ---------- One-time setup ----------
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_ZONES; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);  // All lights start OFF
  }

  Serial.println("Smart Beam - Zoned Motion Lighting Ready");
  Serial.print("Detection range: ");
  Serial.print(DETECTION_DISTANCE);
  Serial.println(" cm per zone");
}

// ---------- Main loop ----------
void loop() {
  for (int i = 0; i < NUM_ZONES; i++) {
    if (!zoneEnabled[i]) continue;

    int distance = measureDistance(trigPins[i], echoPins[i]);
    bool occupied = (distance > 0 && distance <= DETECTION_DISTANCE);

    if (occupied != zoneOccupied[i]) {
      zoneOccupied[i] = occupied;
      digitalWrite(relayPins[i], occupied ? HIGH : LOW);

      Serial.print(zoneNames[i]);
      Serial.print(occupied ? " - Motion Detected! " : " - Clear. ");
      if (occupied) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.print(" cm");
      }
      Serial.println(occupied ? " - LIGHT ON" : "- LIGHT OFF");
    }
  }

  delay(CHECK_INTERVAL);
}

// ---------- Ultrasonic distance measurement ----------
int measureDistance(int trigPin, int echoPin) {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 us pulse to trigger the sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read how long the echo pin stays HIGH (in microseconds)
  long duration = pulseIn(echoPin, HIGH, 30000);  // 30 ms timeout

  // No echo received -> no valid measurement
  if (duration == 0) {
    return -1;
  }

  // Sound travels ~0.034 cm per microsecond; divide by 2 for round trip
  return duration * 0.034 / 2;
}
