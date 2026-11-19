# Measured AmBe reference

The red curve in `spectrum.pdf` represents an experimentally measured AmBe
neutron emission spectrum, not a fitted theoretical curve or this simulation.

Source: Research Centre Rez, **ALARM-CF-CU-SHIELD-001**, Revision 1,
March 3, 2022, printed page 117 (PDF page 119), **Figure B.5**, red curve
labelled **AmBe**. The blue ISO curve in the original figure is not used.

https://www-nds.iaea.org/INDEN/data/ALARM-CF-CU-SHIELD-001-FINAL.pdf

The report cites the original measurement:
M. Kostal et al., *Validation of heavy water cross section using AmBe neutron
source*, EPJ Web of Conferences **239**, 18008 (2020).
https://doi.org/10.1051/epjconf/202023918008

`reference_measured.dat` contains 100 bins, 1.0--11.0 MeV, extracted from the
PDF's vector step curve. These are **graph-digitized values, not the authors'
raw experimental data**. Columns are lower edge, upper edge, arbitrary density,
and uncertainty extracted from the plotted vertical error bars. Only the central
reference curve is drawn. No missing low-energy bins were invented.

The PDF coordinates used for calibration (points, origin at page upper left) are
x=111.029999 at 0 MeV, x=513.330017 at 11 MeV, y=280.289978 at zero density,
and y=72.829956 at unit density. Horizontal red segments were mapped to
0.1 MeV bins; segments outside the visible 1--11 MeV interval were excluded.

Both plotted spectra have unit area in the **common interval 1--11 MeV**.
The simulation retains its 120 bins over 0--12 MeV. Blue error bars are counting
errors; no formal goodness-of-fit is calculated from the digitized reference.
This is a comparison with a published source, not a measurement of our source.

Run the C++ ROOT macro from this directory:

```sh
root -l -b -q spectrum.C
```
