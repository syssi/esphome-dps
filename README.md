# esphome-dps

## Schematics

```

┌──────────┐                ┌─────────┐
│          │<----- RX ----->│         │
│ DPS5020  │<----- TX ----->│ ESP32/  │
│          │<----- GND ---->│ ESP8266 │<-- 3.3V
│          │                │         │<-- GND
└──────────┘                └─────────┘



│            o GND ── GND                │
│            o TXD ── GPIO5 (`rx_pin`)   │
│            o RXD ── GPIO4 (`tx_pin`)   │
│            o VCC                       │
└─[oooooooo]─────────────────[oooooooo]──┘

```

The connector is a 4 Pin GH Molex Pico 1.25mm.

## References

* https://github.com/rfinnie/rdserialtool
* https://github.com/AntaresAdroit/RDTech_PS_Comm
