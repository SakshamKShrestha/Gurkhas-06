#include <iostream>
#include <iomanip>
#include <random>  
#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>
#include <mqtt/client.h>

using namespace std;

const string SERVER_ADDRESS = "tcp://localhost:1883";
const string CLIENT_ID = "mqtt_publisher";
const string TOPIC = "green-guardian/sensors";

double randomvalue(double min, double max)
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

string getTimestamp()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm utcTime{};
    gmtime_s(&utcTime, &currentTime);

    ostringstream timestamp;
    timestamp << put_time(&utcTime, "%Y-%m-%dT%H:%M:%SZ");

    return timestamp.str();
}
 int main()
{
    mqtt::client client(SERVER_ADDRESS, CLIENT_ID);
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);

    try
    {
        client.connect(connOpts);
        cout << "Connected to MQTT broker at " << SERVER_ADDRESS << endl;

        string sensorId = "ESP32_001";
        string zoneId = "ZONE_A";

        while (true)
        {
            double temperature = randomvalue(20.0, 35.0);
            double humidity = randomvalue(40.0, 95.0);
            double soilMoisture = randomvalue(20.0, 80.0);
            int co2 = static_cast<int>(randomvalue(400.0, 1200.0));
            int light = static_cast<int>(randomvalue(2000.0, 20000.0));
            double leafWetness = randomvalue(0.0, 100.0);

            ostringstream payload;
            payload << fixed << setprecision(2);
            payload << "{\n";
            payload << "  \"sensor_id\": \"" << sensorId << "\",\n";
            payload << "  \"zone_id\": \"" << zoneId << "\",\n";
            payload << "  \"timestamp\": \"" << getTimestamp() << "\",\n";
            payload << "  \"temperature\": " << temperature << ",\n";
            payload << "  \"humidity\": " << humidity << ",\n";
            payload << "  \"soil_moisture\": " << soilMoisture << ",\n";
            payload << "  \"co2\": " << co2 << ",\n";
            payload << "  \"light\": " << light << ",\n";
            payload << "  \"leaf_wetness\": " << leafWetness << "\n";
            payload << "}";

            mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, payload.str());
            pubmsg->set_qos(1);
            client.publish(pubmsg);

            cout << "Published message to topic: " << TOPIC << endl;
            cout << payload.str() << endl;

            this_thread::sleep_for(chrono::seconds(5));
        }
    } 
    catch (const mqtt::exception& exc)
    {
        cerr << "MQTT Error: " << exc.what() << endl;
        return 1;

    }
    return 0;   
}
