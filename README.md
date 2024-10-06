# DataReleases

## "Snolab_binned_1" 

This folder contains images corresponding to the hidden (blinded) dataset from the Golden quadrant used in this publication: link to the paper. We calibrate the images using the mean value of a single electron in ADUs

ROOT files contain images separated by exposure time (in seconds). Inside each file, there is a tree:

![image](https://github.com/sensei-skipper/DataReleases/assets/64160793/ece15582-35ff-4381-a0bb-0a93f4520bb6)

X corresponds to the column, Y to the row, ePix to the superpixel charge after calibration, and RunID to the internal image index. The mask contains a binary code where each bit corresponds to a different mask. A "1"-bit indicates that a superpixel is masked with the mask assigned to the bit number. 

For the 1e analysis, we implemented this cut to select the unmasked pixels: !(mask&0x967d). Where:

0x967d = 38525 = 1001011001111101, meaning that active masks are:

* 1-pix cluster cut: if the superpixel has a neighbor of >0.5e- (or epix > 0.5e-)
* Bleeding mask: if the superpixel may be affected by CTI
* High-energy or Halo mask: if the superpixel may be affected by High Energy Events
* Cross-talk: if the superpixel may be affected by cross-talk
* Noisy row: if the electronic noise in the row is too large.
* Edge: if the superpixel is near the edge of the CCD
* Bad pixel: if the superpixel is in a pixel with an excess of single electron events, as analyzed over the commissioning dataset
* Bad Column:  if the superpixel is in a column with an excess of single electron events, as analyzed over the commissioning dataset
* Extended bleed mask: similar to hot columns but calculated only over bleed zones.
* ClusterShape: this doesn't have any impact in the 1-electron analysis since we select 1-superpixel events

Other masks are executed during reconstructionn but are either deprecated or not used in the 1-electron analysis. The images include an overscan of 128 columns and 4 rows.

The plotRates.C macro draws the unmasked superpixel charge histograms for each exposure, fits the 1e-density, builds the 1e-density vs exposure plot, and extracts the rate.

Running ./plotRates.C creates this Canvas:


<img src="https://github.com/user-attachments/assets/3521f902-d15a-4e4f-99c0-86c7b118baa9" width="400">


The charge histograms are in the top panel, and the 1e density vs exposure is in the bottom. It also outputs in the screen the number of masked and unmasked pixels for each exposure and fits the 1e rate:

![image](https://github.com/user-attachments/assets/2fc0e709-c6d8-49d6-b52e-dd1985ae317b)




