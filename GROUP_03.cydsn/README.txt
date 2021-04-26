-------AY2021_II_HW_03; Di Liddo Mauro, Goshen Davide-------

BCP configuration:

We set as default the value of 5 samples in CONTEMP_MODE to generate each value visualized in the chart, the user is free to change the desired value from almost 2 up
to almost 10 samples modifying the bit devoted to that configuration (last .iic instruction, please do not modify the other lines but only the last one 'w 08 00 17 p'):

w 08 03 p
r 08 @1ldr p
w 08 04 p
r 08 @0ldr p
w 08 05 p
r 08 @1tmp p
w 08 06 p
r 08 @0tmp p
w 08 00 17 p
w 08 01 14 p

in particular the user should be interested in changing the last number (i.e. 17 in hex) that codifies both the number of sample and the function modality:
-bit 7 - (not read by the program)
-bit 6 - (not read by the program)
-bit 5:2 number of samples (4 bits to codify the number of samples in binary, i.e. 0101 codifies 5 samples)
-bit 1:0 modality (00 shut down of the communication; 01 only temperature sensor on; 10 only light sensor on; 11 both sensors on)


REMEMBER: if the user wants to change the number of samples, in order to guarantee a desired 'F' trasmission rate, the timer period value has to be changed
according to this formula:

new_period_value = 5000/(F*new_sample_number); --- 5000Hz is our f_clock

the new_period_value should be inserted in the register_2 by accessing via .iic in BCP:

w 08 01 xx p

whwere xx is the .hex value of the new period. It can vary from 10 Hz to 100 Hz.