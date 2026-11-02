
# Green Guardian — Sensor Data Format

## 1. Purpose
This document defines the JSON data format used to exchange greenhouse sensor readings between the C++ sensor simulator, MQTT communication system, edge processor, and backend server.

## 2. Data Format
Sensor readings are represented using JSON (JavaScript Object Notation). Each message contains information about the sensor node, greenhouse zone, timestamp, and environmental measurements.

## 3. Sensor Fields

| Field | Data Type | Unit | Description |
|---|---|---|---|
| sensor_id | String | N/A | Unique identifier of the sensor node |
| zone_id | String | N/A | Greenhouse zone identifier |
| timestamp | String | UTC ISO 8601 | Time when the reading was generated |
| temperature | Number | °C | Air temperature |
| humidity | Number | % | Relative humidity |
| soil_moisture | Number | % | Soil moisture level |
| co2 | Number | ppm | Carbon dioxide concentration |
| light | Number | lux | Light intensity |
| leaf_wetness | Number | 0–100 normalized scale | Simulated leaf wetness level |

## 4. Example Sensor Message

```json
{
  "sensor_id": "ESP32_001",
  "zone_id": "ZONE_A",
  "timestamp": "2026-11-01T14:30:00Z",
  "temperature": 28.5,
  "humidity": 75.2,
  "soil_moisture": 42.0,
  "co2": 650,
  "light": 12000,
  "leaf_wetness": 35.0
}
```

## 5. Validation Rules
- Each message must be a valid JSON object.
- All nine fields are required.
- `sensor_id` and `zone_id` must be non-empty strings.
- `timestamp` must use ISO 8601 UTC format.
- Environmental measurements must be numeric values, not strings.
- Humidity and soil moisture must be between 0 and 100 percent.
- Leaf wetness uses a simulated normalized scale from 0 to 100.
- CO2 concentration and light intensity cannot be negative.
- Temperature is measured in Celsius and must be a finite numeric value.

## 6. Planned Data Flow
C++ Sensor Simulator → MQTT → Edge Processor → Backend (FastAPI) → MySQL → Farmer Dashboard

The edge processor will validate incoming sensor data and perform local risk checks before forwarding relevant information to the backend.

## 7. Scope
This format is defined for the initial software prototype. Sensor readings will first be simulated in C++. Physical ESP32 sensors may be integrated later.
  
