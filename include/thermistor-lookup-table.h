#ifndef THERMISTOR_LOOKUP_TABLE_H
#define THERMISTOR_LOOKUP_TABLE_H

struct ThermistorTableEntry {
    float res; // In kOhms
    int temp;  // in celsius
};

const ThermistorTableEntry thermistorLookupTable[] = {
    {27.494, 0},
    {26.325, 1},
    {25.212, 2},
    {24.153, 3},
    {23.144, 4},
    {22.184, 5},
    {21.268, 6},
    {20.396, 7},
    {19.564, 8},
    {18.771, 9},
    {18.015, 10},
    {17.294, 11},
    {16.605, 12},
    {15.948, 13},
    {15.320, 14},
    {14.720, 15},
    {14.148, 16},
    {13.600, 17},
    {13.077, 18},
    {12.577, 19},
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