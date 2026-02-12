# Correct DHT22 Wiring Diagram

## Complete Setup Diagram

```
                      ┌─────────────────────────┐
                      │    Arduino UNO R4       │
                      │                         │
                      │  5V    GND    Pin2      │
                      │   │     │      │        │
                      │   │     │      │        │
                      └───┼─────┼──────┼────────┘
                          │     │      │
        ┌───────────────┐  │     │      │
        │               │  │     │      │
        │   100µF       │  │     │      │
        │  Capacitor    │  │     │      │
        │   │││││       │  │     │      │
        │   │││││       │  │     │      │
        │    (C)        │  │     │      │
        │   ─────       │  │     │      │
        │     │         │  │     │      │
        └─────┼─────────┘  │     │      │
              │            │     │      │
              │    ┌───────┴─────┴──────┘
              │    │
              │    │   ┌─────────────────────┐
              │    │   │    Pull-up Resistor │
              │    │   │      4.7kΩ (R)      │
              │    │   │      ═══════════    │
              │    │   │   Two ends shown:   │
        DHT22 │    │   │                     │
        ─────────  │   │   ┌─────────────┐   │
        │ │ │ │   │   │   │ Resistor R  │   │
        └─┼─┼─┼───┘   │   │    ╭───╮    │   │
          │ │ │       │   │    │   │    │   │
        1 2 3 4       │   │    │   │    │   │
        │ │   │       │   └────┤───┤────┘   │
        │ │   └───────┼─────→  ╰───╯        │
VCC ─────┴─┴───────────┤     (soldered     │
          │            │      between)      │
         DATA ─────────┼─────────────────────┘
          │            │
        GND ───────────┘
          │
```

## Simplified Connection Table

| DHT22 Pin | Label | Arduino Connection | Notes |
|-----------|-------|-------------------|-------|
| 1 | VCC | 5V | Power supply (3.3-5V) |
| 2 | DATA | Pin 2 | Data line (requires pull-up) |
| 3 | NC | Not used | Leave disconnected |
| 4 | GND | GND | Ground |

## Component Reference

### 1. Pull-up Resistor (CRITICAL)
- **Type**: 4.7kΩ resistor (1/4W minimum)
- **Location**: Between VCC and DATA pin
- **Why**: DHT22 data line is open-drain, needs pull-up
- **Alternatives**: 4.7kΩ, 10kΩ (4.7kΩ preferred)
- **Visual**: 
  ```
  ┌──────────────┐
  │   Resistor   │  Color bands (4.7kΩ):
  │  ╭─────────╮ │  Yellow-Purple-Red-Brown
  │  │         │ │  or check with multimeter
  │  ╰─────────╯ │
  └──────────────┘
  ```

### 2. Capacitor (Optional but Recommended)
- **Type**: 100µF electrolytic capacitor
- **Voltage**: 10V or higher rating
- **Location**: Between VCC and GND (near DHT22)
- **Why**: Stabilizes power supply, reduces noise
- **Visual**:
  ```
  ┌────────────┐
  │   100µF    │
  │   │││││    │  Long leg = Positive (+)
  │   │││││    │  Short leg = Negative (-)
  │   ─────    │
  └────────────┘
  ```

### 3. DHT22 Sensor Package
- **Dimensions**: ~25mm x 11mm
- **Pins**: 4 pins, spacing 2.54mm (standard breadboard)
- **Pin order**: VCC, DATA, NC, GND (left to right)
- **Visual**:
  ```
  ┌─────────────────┐
  │     DHT22       │  Front view
  │  (top of chip)  │
  │                 │
  │ 1  2  3  4      │ Pin numbering
  └─────────────────┘
  │ │  │  │  │
  │ │  │  │  └─ GND
  │ │  │  └──── NC (not used)
  │ │  └─────── DATA
  │ └────────── VCC
  ```

## Breadboard Layout

```
     J1  J2  J3  J4  J5  J6  J7  J8  J9  J10
A    │   │   │   │   │   │   │   │   │   │
     ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
B    │   │   │VCC│   │   │   │5V │   │   │
     ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
C    │   │   │DAT│   │   │   │P2 │   │   │
     ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
D    │   │   │ R │   │   │   │   │   │   │ ← Resistor between DATA and VCC
     ├───┼───┼───┼───┼───┼───┼───┼───┼───┤
E    │   │   │GND│   │   │   │GND│   │   │
     └───┴───┴───┴───┴───┴───┴───┴───┴───┘

Key:
VCC = DHT22 Pin 1 (VCC) → Arduino 5V
DAT = DHT22 Pin 2 (DATA) → Arduino Pin 2
GND = DHT22 Pin 4 (GND) → Arduino GND
R = Pull-up Resistor (4.7kΩ)
P2 = Arduino Pin 2
```

## Step-by-Step Wiring Instructions

1. **Power connections**:
   - DHT22 VCC (pin 1) → Arduino 5V
   - DHT22 GND (pin 4) → Arduino GND
   - Add 100µF capacitor across VCC and GND (optional)

2. **Data line**:
   - DHT22 DATA (pin 2) → Arduino Pin 2
   - Install 4.7kΩ resistor from DATA to VCC
   - **This resistor is CRITICAL for operation**

3. **Verification**:
   - All connections should be tight in breadboard
   - Resistor should measure ~4700Ω with multimeter
   - VCC should measure 5V with multimeter

## Troubleshooting by Visual Inspection

| Issue | Visual Check |
|-------|--------------|
| No power to sensor | VCC wire not inserted in breadboard hole |
| Data line floating | Pull-up resistor missing or loose |
| Intermittent errors | Loose jumper wire connections |
| Capacitor backwards | Check polarity marks (longer leg is +) |

---

**Remember**: The pull-up resistor between DATA and VCC is essential!
Without it, the DHT22 cannot function properly.

Generated: February 2026

