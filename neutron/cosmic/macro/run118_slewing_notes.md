# Run 118: slewing correction and detector variance separation

```sh
root -l -b -q 'slewing118.C()'
```

From `macro/`, the command writes `result/run118_slewing_resolution.pdf`, `run118_slewing.root`, and `run118_slewing_summary.txt`. No CSV. Arguments: inputFile, outputDir, tdcNs (0.025), adcUpper (3800). The former equalDetectors argument has been removed because no equality constraint is used.

## Selection and slewing

All six ADCs simultaneously exceed lower cuts Neutron 1-1/1-2/2-1/2-2/Trig 1/Trig 2 > 1130/1040/1000/1000/350/260 and are below adcUpper. TDCs for all six PMTs and reference TDC15 must be valid (not -9999). Selected events are shuffled with seed 118 and split equally into training/evaluation samples (currently 1858 each).

All six PMTs, including both Trig PMTs, are independently slewing-corrected using the cosmic_analysis.cc target/function convention:

```
relative_time_i = TDC_i - TDC15
f_i(Q) = a_i/sqrt(Q) - b_i
corrected_time_i = (relative_time_i - f_i(Q)) * tdcNs
```

The fit is implemented as c+s*(sqrt(1500/Q)-1) for conditioning, with a=s*sqrt(1500), b=s-c. Training uses 15 Huber IRLS iterations; no held-out event is used to estimate coefficients. Original timing offsets are subtracted, so corrected means are not absolute flight times. No pedestal subtraction or timing-window event selection. The 25 ps/count scale follows the existing macro, not a new calibration. This preserves the original fit target/function but improves numerical fitting and retains a held-out evaluation sample.

## Detector times and all pair fits

```
ND1 = (Neutron_1-1 + Neutron_1-2)/2
ND2 = (Neutron_2-1 + Neutron_2-2)/2
T1 = Trig_1
T2 = Trig_2
```

All six pairs ND1-ND2, ND1-T1, ND1-T2, ND2-T1, ND2-T2, T1-T2 are Gaussian fitted both before and after correction on the same held-out events. Every fit is displayed. Windows are each distribution's median +/- 2.5*(1.4826 MAD); histogram limits are +/-8 robust widths, with tails retained in underflow/overflow. Core widths need not obey exact full-variance identities when distributions are non-Gaussian.

Assuming zero covariance and negligible flight-path variation, pair variance V_ij=v_i+v_j. Thus, for reference T:

```
v_ND1 = (V_ND1,ND2 + V_ND1,T - V_ND2,T)/2
v_ND2 = (V_ND1,ND2 + V_ND2,T - V_ND1,T)/2
sigma_i = sqrt(v_i)
```

These are evaluated separately for Trig1 and Trig2. No equal-ND resolution assumption. Also included is the equal-weight least-squares solution of all six equations in four unknown variances. Its closure residuals are displayed to test consistency. Negative inferred variances are reported as nonphysical and never clipped to zero.

Statistical uncertainty is estimated with 200 paired bootstrap resamples of held-out events (seed 1182026): all six pair Gaussian fits are repeated on the same resample, preserving their statistical dependence. Calibration stays fixed, so coefficient/model/cut/time-scale/geometry systematics are excluded. Errors on sigma use the delta method from the bootstrap variance error and can be unreliable near zero variance. No claim of a unique intrinsic neutron-interaction resolution is made: these are cosmic-run detector estimates under the zero-covariance/path assumptions.

## Current reference-specific result

| Reference | Neutron 1 sigma [ps] | Neutron 2 sigma [ps] |
|---|---:|---:|
| Trig 1 | 82.0 +/- 11.0 | 60.4 +/- 14.8 |
| Trig 2 | 74.9 +/- 6.9 | 69.0 +/- 7.8 |

Corrected pair widths: ND1-ND2 101.8 ps, ND1-T1 364.0 ps, ND1-T2 285.8 ps, ND2-T1 359.8 ps, ND2-T2 284.3 ps, T1-T2 419.6 ps.

The six-pair consistency solution gives ND1 102.3 +/- 7.9 ps, ND2 92.2 +/- 10.1 ps, T1 342.3 +/- 8.6 ps, T2 259.7 +/- 7.1 ps, but pair-variance closure is imperfect (largest individual standardized residual about 3.1). This is not a global goodness-of-fit significance because residuals are correlated. It indicates limitations of the zero-covariance/core-width model; do not treat this joint solution as an exact intrinsic measurement or silently replace the reference-specific results by it.

## Output contents

The nine-page PDF contains selection/summary, six-PMT ADC versus reference-relative TDC before and after correction, ND1-ND2 width comparison, position diagnostics, all six pair Gaussian fits before correction, all six after correction, separated-resolution formulas/results, and closure diagnostics. Rainbow Z colors and actual detector names are used.

The ROOT `selected` tree stores source_entry, training, qdc[32], tdc[16], pmt_time_corrected_ns[6] (Neutron 1-1, 1-2, 2-1, 2-2, Trig1, Trig2), detector_time_corrected_ns[4] (ND1, ND2, Trig1, Trig2), and raw/corrected ND time differences. Fit coefficients, pair histograms/functions, and six-pair variance estimates/errors are saved. Reruns replace outputs with the same names.
