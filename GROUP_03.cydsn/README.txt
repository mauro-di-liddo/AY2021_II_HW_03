-------AY2021_II_HW_03; Di Liddo Mauro, Goshen Davide-------

BCP configuration:

We set as default the value of 5 samples in CONTEMP_MODE to generate each value visualized in the chart, the user is free to change the desired value up
to 15 samples modyfing the bit devoted to that configuration (last .iic instruction):

w 08 00 17 p

in particular the user should be interested in changing the last number (i.e. 17 in hex) that codyfies both the number of sample and the function modality:
-bit 7 - (not read by the program)
-bit 6 - (not read by the program)
-bit 5:2 number of samples (4 bits to codify the number of samples in binary, i.e. 0101 codifies 5 samples)
-bit 1:0 modality (00 shut down of the device; 01 only temperature sensor on; 10 only light sensor on; 11 both sensors on)


REMEMBER: if you want to change the number of samples, in order to garauntee a fixed F trasmission rate, you have to change the  timer period value
according to this formula:

5000/(F*new_sample_number) --- 5000Hz is our f_clock

the new period value should be inserted in the register_2 by accessing via .iic in BCP:

w 08 01 xx p

whwere xx is the .hex value of the new period.