# DataReleases

## "Snolab_binned_1" 

This folder contains images corresponding to the hidden (blinded) dataset from the Golden quadrant used in this publication: arxiv. We calibrate the images using the mean value of a single electron in ADUs

The images are contained in root files separated by exposure time (in seconds).
Inside the root files there is a tree:

![image](https://github.com/sensei-skipper/DataReleases/assets/64160793/ece15582-35ff-4381-a0bb-0a93f4520bb6)

Where X corresponds to the column, Y to the row, ePix to the superpixel charge after calibration, and RunID corresponds to the an internal image index. The mask contains a binary code where each bit corresponds to a different mask. "1" indicates that a supeerpixel is masked with the mask assigned to the bit number. For example:

256 + 4 = 260 In binary is: 0000000100000100. Which indicated that the pixel is masked by the Halo and Bleeding cuts.

For the 1e analysis, we implemented this cut to select the unmasked pixels: !(mask&0x967d). 

0x967d = 38525 = 1001011001111101. Which means that active masks are:

* 1 pix cluster cut: if the superpixel has a neighbour of >0.5e- (or epix > 0.5e-)
* Bleeding mask: if the superpixel may be affected by CTI
* High-energy or Halo mask: if the superpixel may be affected by High Energy Events
* Cross-talk: if the superpixel may be affected by cross talk
* Noisy row: if the electronic noise in the row too large.
* Edge: if the superpixel is near the edge of the CCD
* Bad pixel: if the superpixel is in a pixel with an excess of single electron events, as analyzed over a certain dataset
* Bad Column:  if the superpixel is in a column with an excess of single electron events, as analyzed over a certain dataset
* Extended bleed mask: does not apply?
* ClusterShape: this doesn't have any impact in the 1-electron analysis since we selec 1 superpixel events

Other masks are executed during reconstraction but are either deprecated or not used in the 1-electron analysis. The images include an overscan of 128 columns and 4 rows.

The macross plotRates.C draws the charge histograms for each exposure and fit the 1e- density to build the 1e- density vs exposure plot and extract de rate.

Running ./plotRates.C creates this Canvas:

![Plots](https://github.com/user-attachments/assets/c3090005-a487-411d-bd6c-bebd136b71db)

With the histograms in the upper panel, and 1e density vs exposure at the bottom. It also outputs in the screen the number of masked and unmasked pixels for each exposure and the fittes 1e rate:

![screeoutput](https://github.com/user-attachments/assets/7c483dfb-5f44-4740-bc90-92b2715440eb)

