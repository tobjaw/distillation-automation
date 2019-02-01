# Hardware
## Peripherals
### Serial Host Connection
With the ATMega2560 supporting USART at hardware level, we settled on using the serial connection for in- and output.

Setup for the most part just consists of setting the correct registers, and can be seen in the file [uart.c](@ref uart.h).
Note that by redirecting stdout and stdin, we can use stdio functions that operate on those streams, just as if the code was running on a standard PC.
Also note that communication to the scale also uses USART, but on another slot.

### AD7792
To read the temperature using Pt1000 temperature sensors, we employ two AD7792-based ADCs.  
Low-Level communication to the ADCs is implemented via [SPI](@ref SPI.c). SPI is a very simple protocol in the sense that the involved devices just trade one byte at a time.  
The motherboard is functioning as the SPI master, while each AD7792 is selected via a special slave pin.  
With the capabilties to trade raw bytes established, we implement a vendor-specific protocol on top in the file [AD7792](@ref AD7792.c).  
Finally, we hide all the implementation behind the API.

### Scale
As mentioned before, the scale also uses USART for communication.  
The scale sends 6 bytes at a time, with the first 4 bytes ASCII-encoding the decimal digit, the fifth byte being a parity byte and the final byte being a NULL-terminator.  
The [implementation](@ref scale.c) is straight forward - behind the API interface, we read 6 bytes, check for parity and convert the word to a float.  
Note that we cannot rely on parity alone to verify the word - additionally, we have to check the final byte is actually NULL and the first 4 bytes are in the 0-9 range of ASCII chars.

### Heater
The implementation of the heater is the most simple peripheral in the sense that it only requires [reading](@ref getHeaterStatusPhysical) and [setting](@ref setHeaterStatusPhysical) a single bit.  
However, there is some additional error checking by consulting the status of the attached security board, which blocks the heater, if not all security conditions are met.
