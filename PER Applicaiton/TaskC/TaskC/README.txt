##Data Format Design (largely referenced from Google):
Header:
Magic number: 4 bytes, used to identify the dataset
Variable ID: 2 bytes
Data type (1 byte): 0 for boolean, 1 for integer, 2 for float, 3 for double.
Variable name length: 1 byte
Variable name: use ASCII representation

Data:
Timestamp (4 bytes): Relative to the start, in milliseconds.
Variable ID (2 bytes)
Data value (size depends on its data type: 1 byte for boolean, 4 bytes for integer and
float, 8 bytes for double)

Encoder:
Write the magic number
For each variable, write its ID, data type, name length, and name.
For each data entry, write the timestamp, variable ID, and data value.

Decoder:
Identify and validate the magic number
For each variable, read its ID, data type, name length, and name.
For each data entry, read the timestamp, variable ID, and data value.

Tools (see main for sample c++ structures and functions):
A "csv_to_custom" function to encode the data into the custom binary format (CSV -> binary).
A "custom_to_csv" function to decode the binary data back into a structured format (binary -> CSV).

##Data Format Design Explanation:
Extensibility: the header provides meta-data for each variable, so no external data is required for decoding

Space efficiency: binary representatiom is used for the custom data format, which requires less space than CSV

Maintainability: I guess debugging is easier cuz there is a clear distinction between header and data.

Portability: does not rely on system-specific features

Corruption: can employ checksums at regular intervals in the code, e.g., sum up the values of all the bytes and calculate the modulus with 256 during writing and reading to see if they match
