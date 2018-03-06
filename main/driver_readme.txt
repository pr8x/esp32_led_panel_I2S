This is example code to driver a p3(2121)64*32 -style RGB LED display. These types of displays do not have memory and need to be refreshed
continuously. The display has 2 RGB inputs, 4 inputs to select the active line, a pixel clock input, a latch enable input and an output-enable
input. The display can be seen as 2 64x16 displays consisting of the upper half and the lower half of the display. Each half has a separate 
RGB pixel input, the rest of the inputs are shared.

Each display half can only show one line of RGB pixels at a time: to do this, the RGB data for the line is input by setting the RGB input pins
to the desired value for the first pixel, giving the display a clock pulse, setting the RGB input pins to the desired value for the second pixel,
giving a clock pulse, etc. Do this 64 times to clock in an entire row. The pixels will not be displayed yet: until the latch input is made high, 
the display will still send out the previously clocked in line. Pulsing the latch input high will replace the displayed data with the data just 
clocked in.

The 4 line select inputs select where the currently active line is displayed: when provided with a binary number (0-15), the latched pixel data
will immediately appear on this line. Note: While clocking in data for a line, the *previous* line is still displayed, and these lines should
be set to the value to reflect the position the *previous* line is supposed to be on.

Finally, the screen has an OE input, which is used to disable the LEDs when latching new data and changing the state of the line select inputs:
doing so hides any artifacts that appear at this time. The OE line is also used to dim the display by only turning it on for a limited time every
line.

All in all, an image can be displayed by 'scanning' the display, say, 100 times per second. The slowness of the human eye hides the fact that
only one line is showed at a time, and the display looks like every pixel is driven at the same time.

Now, the RGB inputs for these types of displays are digital, meaning each red, green and blue subpixel can only be on or off. This leads to a
color palette of 8 pixels, not enough to display nice pictures. To get around this, we use binary code modulation.

Binary code modulation is somewhat like PWM, but easier to implement in our case. First, we define the time we would refresh the display without
binary code modulation as the 'frame time'. For, say, a four-bit binary code modulation, the frame time is divided into 15 ticks of equal length.

We also define 4 subframes (0 to 3), defining which LEDs are on and which LEDs are off during that subframe. (Subframes are the same as a 
normal frame in non-binary-coded-modulation mode, but are showed faster.)  From our (non-monochrome) input image, we take the (8-bit: bit 7 
to bit 0) RGB pixel values. If the pixel values have bit 7 set, we turn the corresponding LED on in subframe 3. If they have bit 6 set,
we turn on the corresponding LED in subframe 2, if bit 5 is set subframe 1, if bit 4 is set in subframe 0.

Now, in order to (on average within a frame) turn a LED on for the time specified in the pixel value in the input data, we need to weigh the
subframes. We have 15 pixels: if we show subframe 3 for 8 of them, subframe 2 for 4 of them, subframe 1 for 2 of them and subframe 1 for 1 of
them, this 'automatically' happens. (We also distribute the subframes evenly over the ticks, which reduces flicker.)


In this code, we use the I2S peripheral in parallel mode to achieve this. Essentially, first we allocate memory for all subframes. This memory
contains a sequence of all the signals (2xRGB, line select, latch enable, output enable) that need to be sent to the display for that subframe.
Then we ask the I2S-parallel driver to set up a DMA chain so the subframes are sent out in a sequence that satisfies the requirement that
subframe x has to be sent out for (2^x) ticks. Finally, we fill the subframes with image data.

We use a frontbuffer/backbuffer technique here to make sure the display is refreshed in one go and drawing artifacts do not reach the display.
In practice, for small displays this is not really necessarily.

Finally, the binary code modulated intensity of a LED does not correspond to the intensity as seen by human eyes. To correct for that, a
luminance correction is used. See val2pwm.c for more info.

Note: Because every subframe contains one bit of grayscale information, they are also referred to as 'bitplanes' by the code below.