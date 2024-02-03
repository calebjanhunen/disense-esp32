#ifndef THERMISTOR_LOOKUP_TABLE_H
#define THERMISTOR_LOOKUP_TABLE_H

struct ThermistorTableEntry {
    float res; // In kOhms
    int temp;  // in celsius
};

const ThermistorTableEntry thermistorLookupTable[] = {
    {12.099, 20},
    {11.641, 21},
    {11.204, 22},
    {10.785, 23},
    {10.384, 24},
    {10.000, 25},
    {9.632, 26},
    {9.280, 27},
    {8.943, 28},
    {8.620, 29},
    {8.310, 30},
    {8.012, 31},
    {7.728, 32},
    {7.454, 33},
    {7.192, 34},
    {6.940, 35},
    {6.699, 36},
    {6.467, 37},
    {6.244, 38},
    {6.030, 39},
    {5.825, 40},
    {5.628, 41},
    {5.438, 42},
};

const int lookupTableSize = sizeof(thermistorLookupTable) / sizeof(ThermistorTableEntry);

#endif