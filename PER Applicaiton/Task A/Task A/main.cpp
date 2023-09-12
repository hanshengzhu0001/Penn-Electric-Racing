//The code below is an attempt of debugging from raw GPT-generated code that was very sketchy

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <limits>

//using namespace std;

struct DataEntry {
    long long timestamp; // in milliseconds
    float value;
};

int main() {
    std::ifstream file("/Users/a4818002/Downloads/PER 2023-2024 Electrical Software Challenge Data Files/TaskA.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Read the timestamp from the first line
    std::string line;
    std::getline(file, line);
    std::cout << "Log Timestamp: " << line.substr(18) << std::endl;

    // Extract Value-ID associations
    std::map<int, std::string> idToVariableMap;
    while (std::getline(file, line) && line.find("Value ") == 0) {
        std::stringstream ss(line);
        std::string tmp, variableName;
        int id;

        ss >> tmp >> variableName >> tmp >> tmp >> id;
        idToVariableMap[id] = variableName;
    }

    // Read the data logs
    std::map<std::string, std::vector<DataEntry>> data;
    do {
        std::stringstream ss(line);
        long long timestamp;
        int id;
        float value;

        ss >> timestamp >> id >> value;
        data[idToVariableMap[id]].push_back({timestamp, value});
    } while (std::getline(file, line));

    file.close();

    // Calculate average speed from wheel speeds
    std::vector<std::string> wheelSpeeds = {
        "pcm.wheelSpeeds.frontLeft",
        "pcm.wheelSpeeds.frontRight",
        "pcm.wheelSpeeds.backLeft",
        "pcm.wheelSpeeds.backRight"
    };

    float minSpeed = std::numeric_limits<float>::max();
    float maxSpeed = std::numeric_limits<float>::min();
    float totalSpeed = 0;
    size_t speedCount = 0;

    for (const auto& wheel : wheelSpeeds) {
        if (data.find(wheel) != data.end()) {
            for (const auto& entry : data[wheel]) {
                minSpeed = std::min(minSpeed, entry.value);
                maxSpeed = std::max(maxSpeed, entry.value);
                totalSpeed += entry.value;
                speedCount++;
            }
        }
    }

    if (speedCount == 0) {
        std::cerr << "No Wheel Speed data found!" << std::endl;
        return 1;
    }

    float avgSpeed = totalSpeed / speedCount;

    std::cout << "Min Speed: " << minSpeed << " MPH\n";
    std::cout << "Max Speed: " << maxSpeed << " MPH\n";
    std::cout << "Avg Speed: " << avgSpeed << " MPH\n";

    // Calculate power consumed at each point in time
    std::vector<float> power;
    if (data.find("ams.pack.voltage") != data.end() && data.find("ams.pack.current") != data.end()) {
        for (size_t i = 0; i < data["ams.pack.voltage"].size() && i < data["ams.pack.current"].size(); ++i) {
            power.push_back(data["ams.pack.voltage"][i].value * data["ams.pack.current"][i].value);
        }
    }

    // Use numerical integration (trapezoidal rule) to calculate energy consumed
    float totalEnergy = 0;  // in Joules
    for (size_t i = 1; i < power.size(); ++i) {
        float dt = (data["ams.pack.voltage"][i].timestamp - data["ams.pack.voltage"][i - 1].timestamp) / 1000.0; // convert to seconds
        totalEnergy += (power[i] + power[i - 1]) * dt / 2.0;
    }

    // Convert energy from Joules to kWh
    float totalEnergyKWh = totalEnergy / (3.6e6);  // 1 kWh = 3.6e6 Joules
    std::cout << "Total Energy Consumed: " << totalEnergyKWh << " kWh\n";

    return 0;
}

/*
To compile and run this program:

1. Save the above code to a file, say "TaskA_sol.cpp".
2. Open a command prompt in the directory where the file is saved.
3. Compile the code using the following command
4. Run the compiled program using the command

Note: Ensure that the "TaskA.csv" file is present in the same directory as the compiled program.
*/
