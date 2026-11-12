// ============================================================
// Smart Beam - Zoned Automatic Motion-Sensing Lighting
// Arduino Uno + HC-SR04 Ultrasonic Sensors + Relays + LEDs
// Designed using Object-Oriented Programming (OOP) concepts.
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

// ---------- Global parameters ----------
const int DETECTION_DISTANCE = 50;  // Detection range in cm
const int CHECK_INTERVAL = 200;     // Check every 200 ms

// ============================================================
// Class: MotionZone
// Represents one area of the room with its own ultrasonic
// sensor and relay-controlled light.
// ============================================================
class MotionZone {
  public:
    const char* name;
    int trigPin;
    int echoPin;
    int relayPin;
    bool enabled;

    MotionZone(const char* zoneName, int trig, int echo, int relay, bool isEnabled)
      : name(zoneName), trigPin(trig), echoPin(echo), relayPin(relay), enabled(isEnabled) {
      occupied = false;
    }

    // Configure pins and ensure the light starts OFF
    void begin() {
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
      pinMode(relayPin, OUTPUT);
      digitalWrite(relayPin, LOW);
    }

    // Measure distance, detect motion, and switch the light
    void update() {
      if (!enabled) return;

      int distance = measureDistance();
      bool detected = (distance > 0 && distance <= DETECTION_DISTANCE);

      // Only act on state changes to avoid constant switching
      if (detected != occupied) {
        occupied = detected;
        digitalWrite(relayPin, occupied ? HIGH : LOW);
        report(distance);
      }
    }

  private:
    bool occupied;

    // Send a 10 us pulse on TRIG, time the ECHO pulse,
    // and convert the round-trip time into centimeters
    int measureDistance() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);

      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      long duration = pulseIn(echoPin, HIGH, 30000);  // 30 ms timeout
      if (duration == 0) return -1;                  // No echo received

      // Sound travels ~0.034 cm per microsecond; /2 for round trip
      return duration * 0.034 / 2;
    }

    void report(int distance) {
      Serial.print(name);
      Serial.print(occupied ? " - Motion Detected! " : " - Clear. ");
      if (occupied) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.print(" cm");
      }
      Serial.println(occupied ? " - LIGHT ON" : "- LIGHT OFF");
    }
};

// ============================================================
// Zone instances
// Pin assignments: TRIG / ECHO / RELAY per zone.
// Demo model: only the study area was active.
// ============================================================
const int NUM_ZONES = 3;

MotionZone zones[NUM_ZONES] = {
  MotionZone("Bed Area",      2, 3, 4,  false),
  MotionZone("Cupboard Area", 5, 6, 7,  false),
  MotionZone("Study Area",    8, 9, 10, true )
};

// ---------- One-time setup ----------
void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_ZONES; i++) {
    zones[i].begin();
  }

  Serial.println("Smart Beam - Zoned Motion Lighting Ready");
  Serial.print("Detection range: ");
  Serial.print(DETECTION_DISTANCE);
  Serial.println(" cm per zone");
}

// ---------- Main loop ----------
void loop() {
  for (int i = 0; i < NUM_ZONES; i++) {
    zones[i].update();
  }
  delay(CHECK_INTERVAL);
}
