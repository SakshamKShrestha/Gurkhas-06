
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>

using namespace std;

double randomValue(double min, double max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

string getTimestamp() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm utcTime{};
    gmtime_s(&utcTime, &currentTime);

    ostringstream timestamp;
    timestamp << put_time(&utcTime, "%Y-%m-%dT%H:%M:%SZ");

    return timestamp.str();
}

int main() {
    string sensorId = "ESP32_001";
    string zoneId = "ZONE_A";
    while (true) {
    double temperature = randomValue(20.0, 35.0);
    double humidity = randomValue(40.0, 95.0);
    double soilMoisture = randomValue(20.0, 80.0);
    int co2 = static_cast<int>(randomValue(400.0, 1200.0));
    int light = static_cast<int>(randomValue(2000.0, 20000.0));
    double leafWetness = randomValue(0.0, 100.0);

    cout << fixed << setprecision(2);

    cout << "{\n";
    cout << "  \"sensor_id\": \"" << sensorId << "\",\n";
    cout << "  \"zone_id\": \"" << zoneId << "\",\n";
    cout << "  \"timestamp\": \"" << getTimestamp() << "\",\n";
    cout << "  \"temperature\": " << temperature << ",\n";
    cout << "  \"humidity\": " << humidity << ",\n";
    cout << "  \"soil_moisture\": " << soilMoisture << ",\n";
    cout << "  \"co2\": " << co2 << ",\n";
    cout << "  \"light\": " << light << ",\n";
    cout << "  \"leaf_wetness\": " << leafWetness << "\n";
    cout << "}\n";

        cout << endl;
    this_thread::sleep_for(chrono::seconds(5));
    }
}