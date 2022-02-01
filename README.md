# ESPBiquad

This library is a component for the ESP-IDF to process discrete (especially audio) signals with biquads.

## Features
Multiple filter types, dynamic parameters, and fast executing.
It uses a structure called Biquad, which store every parameters needed to compute the filters

## Functions
| Function | Parameters | Description | 
|--|--|--|
|**init_biquad**| Biquad*, type, Cutoff frequency, Q, Peak gain, Sample Rate| Proprely inits the Biquad structure |
| **process_biquad** | Biquad*, value | Process input value through the biquad |
| **calc_biquad** | Biquad* | Recalculates each coefficients after perimeters were modified, allowing to edit Biquads on the fly |

## Biquad types
| Type | Name |
|--|--|
| Low Pass Filter | bq_type_lowpass |
| High Pass Filter | bq_type_highpass |
| Band Pass Filter | bq_type_bandpass |
| Notch | bq_type_notch |
| Peak | bq_type_peak |
| Low Shelf Filter | bq_type_lowshelf |
| High Shelf Filter | bq_type_highshelf | 

## Notes
Some filters don't require a Q or a Peak Gain, you can set them to any value.
Setting a Q over 0.7 could lead to saturation.
