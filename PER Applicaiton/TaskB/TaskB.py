import struct

# Read the file line by line
with open("/Users/a4818002/Downloads/PER 2023-2024 Electrical Software Challenge Data Files/TaskB.txt", 'r') as file:
    lines = file.readlines()

# Function to convert bytes to floating-point values
def bytes_to_float(byte_values):
    byte_sequence = bytes(byte_values)
    return struct.unpack('<f', byte_sequence)[0]

# Lists to store extracted data
timestamps = []
can_ids = []
data_values = []

# Process each line
for line in lines[1:]:  # Skip the header
    parts = line.strip().split()
    
    # Ensure the line has the expected number of parts
    if len(parts) < 6:  # At least TIMESTAMP, CAN_ID, BYTE_COUNT and some bytes
        continue
    
    # Extract timestamp and CAN ID
    timestamp = parts[0]
    can_id = int(parts[1], 16) * 256 + int(parts[2], 16)
    
    # If the CAN ID is relevant, process the data
    if can_id in [0x313, 0x311, 0x222]:
        byte_values = [int(byte.replace(',', '').replace('[', '').replace(']', '')) for byte in parts[5:]]
        
        if len(byte_values) == 4:
            values = [bytes_to_float(byte_values)]
        elif len(byte_values) == 8:
            values = [bytes_to_float(byte_values[:4]), bytes_to_float(byte_values[4:])]
        else:
            continue
        
        timestamps.append(timestamp)
        can_ids.append(can_id)
        data_values.append(values)

# Given wheel diameter in inches
wheel_diameter = 20.5
wheel_circumference = 3.141592653589793 * wheel_diameter
wheel_circumference_miles = wheel_circumference / (12 * 5280)

# Calculate speeds in MPH for each entry
speeds_mph = [value[0] * wheel_circumference_miles * 60 for value in data_values]

# Filter speeds to remove anomalous values
filtered_speeds = [speed for speed in speeds_mph if 0 <= speed <= 200]

# Calculate minimum, maximum, and average speed in MPH after filtering
min_speed = min(filtered_speeds)
max_speed = max(filtered_speeds)
avg_speed = sum(filtered_speeds) / len(filtered_speeds)

# Create the content for the README file
readme_content = f"""
# FSAE 2015 Endurance Run Analysis

## Speed Analysis

- **Minimum Speed:** {min_speed:.2f} MPH
- **Maximum Speed:** {max_speed:.2f} MPH
- **Average Speed:** {avg_speed:.2f} MPH

## Data Processing Notes

Given that some of the data points in the dataset had anomalous values for speed, these values were filtered out to ensure a more accurate analysis. The speeds were approximated based on the given wheel diameter and assuming the data represents RPM or a similar rotation-related metric.
"""

# Save the content to a README file (this step writes to a file, which you may not need to reproduce)
with open("README.txt", 'w') as file:
    file.write(readme_content)

"""
Instructions to Run the Code:

1. Use a Python IDE instead of XCode
2. Save this script as `TaskB.py`.
3. Place the `TaskB.txt` file in the same directory.
4. Run the script
5. Check the generated `README.txt` for results.


Speed Calculation:
The speed of the vehicle was approximated based on the assumption that the dataset provides rotation-related metrics (e.g., RPM of a wheel).
Given the wheel diameter, the circumference of the wheel was calculated.
The speed in MPH was then approximated using this formula:
Speed (MPH)= RPM Value (from dataset) × Wheel Circumference (in miles) × 60
Speed (MPH)=RPM Value (from dataset)×Wheel Circumference (in miles)×60
Min, Max, and Average Speeds:
After calculating the speed for each relevant data point, the minimum, maximum, and average speeds were computed from the list of calculated speeds.
"""
