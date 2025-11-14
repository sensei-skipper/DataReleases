# Data Dictionary — `SENSEI_diurnal_modulation_data.csv`

**This repository contains the processed dataset used in the SENSEI daily-modulation paper.**  
The data are provided in `data.csv` and correspond to the **processed** (post-masking) sample, as defined and motivated in the **“Data Reconstruction & Selection”** section of the manuscript.  
Preprint: [arXiv:2510.20889](https://arxiv.org/pdf/2510.20889)  
Data DOI (Zenodo): [10.5281/zenodo.17590389](https://doi.org/10.5281/zenodo.17590389)

**Format:** CSV (.csv).  
**Sheet:** `Sheet1`.  
**Granularity:** One row per image / data point.

## Columns

- **exposure** — Exposure time for the interval (seconds).
- **timestart** — Start time in ISO-8601 format `YYYY-MM-DDThh:mm:ss` (UTC).
- **timeend** — End time in ISO-8601 format (UTC).
- **ccd** — CCD identifier (integer).

For each quadrant `q ∈ {1,2,3,4}` the following appear:

- **`quadq_1ecounts`** — Number of measured single-electron (1e) events in quadrant `q` (integer).
- **`quadq_unmaskedpix`** — Count of unmasked (usable) pixels in quadrant `q` (integer).
- **`quadq_1erate`** — Mean 1e rate per pixel in quadrant `q` over the interval  (units: **events per pixel**)
