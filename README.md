# DataReleases

## "Snolab_binned_1" 

This folder contains images corresponding to the hidden (blinded) dataset from the Golden quadrant used in this publication: arxiv. The images are calibrated using the mean value of a single electrón in ADUs

The images are contained in root files separated by exposure time (in seconds).
Inside the root files there is a tree:

![image](https://github.com/sensei-skipper/DataReleases/assets/64160793/ece15582-35ff-4381-a0bb-0a93f4520bb6)

Where X corresponds to the column, Y to the row, ePix to the pixel charge after calibration. The mask corresponds to a binary code where each bit corresponds to a different mask. "1" indicates that a pixel was masked with the mask in that bit number. For example:

256 + 4 = 260 In binary is: 0000000100000100. Which indicated that the pixel is masked by the Halo and Bleeding cuts.

For the 1e analysis we used this cut to select the unmasked pixels: !(mask&0x967d). 

0x967d = 38525 = 1001011001111101. Which means that we only used the ...... masks.

Other masks are executed during reconstraction but are either deprecated or not used in the 1-electron analysis. RunID corresponds to the internal image index number.
