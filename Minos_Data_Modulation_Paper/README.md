# Data Dictionary — `SENSEI_modulation_data.xlsx`

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
