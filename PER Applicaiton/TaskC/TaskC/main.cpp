//The following code is generated from GPT-4 based on my requests.

#include <iostream>
#include <vector>
#include <map>
#include <string>

// Data types
enum DataType {
    BOOLEAN = 0,
    INTEGER,
    FLOAT,
    DOUBLE
};

struct VariableInfo {
    int id;
    DataType type;
    std::string name;
};

struct DataEntry {
    int timestamp;
    int varID;
    union Value {
        bool bVal;
        int iVal;
        float fVal;
        double dVal;
    } value;
};

// Encoding function
void encode(const std::vector<VariableInfo>& variables, const std::vector<DataEntry>& data, std::ostream& out);

// Decoding function
void decode(std::istream& in, std::vector<VariableInfo>& variables, std::vector<DataEntry>& data);

// Helper functions
void writeInt(int value, std::ostream& out);
int readInt(std::istream& in);

// ... other helper functions for different data types and endianness handling
