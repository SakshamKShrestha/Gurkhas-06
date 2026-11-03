#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <cmath>
#include <mqtt/client.h>

using namespace std;

// MQTT connection settings
const string SERVER_ADDRESS = "tcp://localhost:1883";
const string CLIENT_ID = "green_guardian_edge_processor";
const string TOPIC = "green-guardian/sensors";

// Extract a string field from JSON
bool getStringField(const string& json, const string& key, string& value)
{
regex pattern("\"" + key + R"REGEX("\s*:\s*"((?:\\.|[^"\\])*)")REGEX");
    smatch match;

    if (regex_search(json, match, pattern))
    {
        value = match[1].str();
        return !value.empty();
    }

    return false;
}

// Extract a numeric field from JSON
bool getNumberField(const string& json, const string& key, double& value)
{
    string numberPattern =
        R"((-?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?))";

    regex pattern("\"" + key + R"("\s*:\s*)" +
                  numberPattern + R"(\s*(?=[,}]))");

    smatch match;

    if (regex_search(json, match, pattern))
    {
        try
        {
            value = stod(match[1].str());
            return isfinite(value);
        }
        catch (const exception&)
        {
            return false;
        }
    }

    return false;
}

// Validate and process an incoming sensor message
void processSensorData(const string& payload)
{
    string zoneId;
    string timestamp;

    double temperature;
    double humidity;
    double soilMoisture;

    // Check required fields
    if (!getStringField(payload, "zone_id", zoneId) ||
        !getStringField(payload, "timestamp", timestamp) ||
        !getNumberField(payload, "temperature", temperature) ||
        !getNumberField(payload, "humidity", humidity) ||
        !getNumberField(payload, "soil_moisture", soilMoisture))
    {
        cout << "\n[INVALID] Missing or incorrectly formatted sensor fields."
             << endl;
        return;
    }

    // Validate timestamp format
    regex timestampPattern(
        R"(^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}Z$)");

    if (!regex_match(timestamp, timestampPattern))
    {
        cout << "\n[INVALID] Incorrect timestamp format." << endl;
        return;
    }

    // Validate sensor readings
    if (temperature < -50 || temperature > 100)
    {
        cout << "\n[INVALID] Temperature reading out of range." << endl;
        return;
    }

    if (humidity < 0 || humidity > 100)
    {
        cout << "\n[INVALID] Humidity reading out of range." << endl;
        return;
    }

    if (soilMoisture < 0 || soilMoisture > 100)
    {
        cout << "\n[INVALID] Soil moisture reading out of range." << endl;
        return;
    }

    // Display processed readings
    cout << "\n========================================" << endl;
    cout << "       GREEN GUARDIAN EDGE PROCESSOR" << endl;
    cout << "========================================" << endl;

    cout << fixed << setprecision(2);

    cout << "Status: VALID" << endl;
    cout << "Zone ID: " << zoneId << endl;
    cout << "Timestamp: " << timestamp << endl;
    cout << "Temperature: " << temperature << " C" << endl;
    cout << "Humidity: " << humidity << " %" << endl;
    cout << "Soil Moisture: " << soilMoisture << " %" << endl;

    cout << "========================================" << endl;
}

int main()
{
    try
    {
        mqtt::client client(SERVER_ADDRESS, CLIENT_ID);

        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);

        cout << "Connecting to MQTT broker..." << endl;

        client.connect(connOpts);

        cout << "Connected to MQTT broker." << endl;

        client.start_consuming();
        client.subscribe(TOPIC, 1);

        cout << "Subscribed to topic: " << TOPIC << endl;
        cout << "Waiting for greenhouse sensor readings..." << endl;

        while (true)
        {
            mqtt::const_message_ptr message = client.consume_message();

            if (!message)
            {
                if (!client.is_connected())
                {
                    cerr << "MQTT connection lost." << endl;
                    break;
                }

                continue;
            }

            if (message->get_topic() == TOPIC)
            {
                processSensorData(message->to_string());
            }
        }

        client.stop_consuming();
        client.disconnect();
    }
    catch (const mqtt::exception& exc)
    {
        cerr << "MQTT Error: " << exc.what() << endl;
        return 1;
    }

    return 0;
}
