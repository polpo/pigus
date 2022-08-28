# PiISA / PiGUS

![PiISA Logo](piisa-logo.svg)

### *NOTE*: this repo no longer contains the PicoGUS project for the Raspberry Pi Pico. It has been moved to [https://github.com/polpo/picogus](https://github.com/polpo/picogus)! ###

ISA card emulation on the Raspberry Pi 3, 4, and CM4. Initially focusing on [Gravis Ultrasound (GUS)](https://en.wikipedia.org/wiki/Gravis_Ultrasound) sound card emulation.

This is a work in progress in the experimentation phase. It can emulate a GUS and produce sound but things are nowhere near perfect! Please see [the Wiki](https://github.com/polpo/pigus/wiki) for current status.

## For "big" Raspberry Pi:

![pigus-prototype](https://user-images.githubusercontent.com/1544908/182006165-61aa58a7-d336-4c86-becf-883b1548bee1.jpg)

## Open Source Credits

* [circle](https://github.com/rsta2/circle) 
* [circle-stdlib](https://github.com/smuehlst/circle-stdlib)
* `gus.cpp` from [DOSBox-staging](https://github.com/dosbox-staging/dosbox-staging)
* Initial PCB design and AdLib emulation based on [RPiISA](https://github.com/eigenco/RPiISA)

## License

The hardware portions of this repository (hw/, directory) are licensed under the CERN OHL version 2, permissive.

The software portions of this repository (sw/, pgusinit/ directories) are licensed under the GNU GPL version 2.
