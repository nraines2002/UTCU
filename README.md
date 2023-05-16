# UTCU
349 Spring 2023 UTCU Arduino Project

## Timing Notes
(timing in scaled unit seconds)


### Function Timing:
#### -- Preamble --

00832 us | Pre-data 0's total time to send (13 bits) <br />
01152 us | Barker code sent (00320 us, 5 bits) <br />
01600 us | Function ID sent (00448 us, 7 bits) <br />

#### -- Azimuth -- 

01984 us | BOCI sent (128 us total, 3 parallel bits, clocked once high, then clocked off) <br />
02112 us | LOCI sent (128 us total) <br />
02240 us | ROCI sent (128 us total) <br />
02496 us | to scan sent (pause for 128 us prior for unused test) <br />
08696 us | to scan finished, to_fro pause sent, pauses for 600 us (to scan ran for 6200 us) <br />
09296 us | fro scan sent <br />
15496 us | fro scan finished <br />

#### -- Back Azimuth --

01984 us | BOCI sent (128 us total, 3 parallel bits, clocked once high, then clocked off) <br />
02112 us | LOCI sent (128 us total) <br />
02240 us | ROCI sent (128 us total) <br />
02496 us | to scan sent (pause for 128 us prior for unused test) <br />
06696 us | to scan finished, to_fro pause sent, pauses for 600 us (to scan ran for 4200 us) <br />
07296 us | fro scan sent <br />
11496 us | fro scan finished <br />

#### -- Elevation -- 

