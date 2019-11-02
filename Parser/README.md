# Parser

Note that labels are zero-indexed (VIN0, VIN1, ..., VCC0, VCC1) and indices are mandatory for these components


## TODO: UPDATE!!! OUTDATED!!

### Supported Netlist Grammar:
```
Pot      := POTLABEL _ NODE _ NODE _ Value _ ANYTHING
Passive  := PLABEL LABEL _ NODE _ NODE _ Value _ ANYTHING
Input    := ILABEL LABEL _ NODE _ NODE _ ANYTHING
Source   := SLABEL LABEL _ NODE _ NODE _ ANYTHING
Diode    := DLABEL LABEL _ NODE _ NODE _ MODEL _ ANYTHING
Bjt      := TODO
Jfet     := TODO
Mosfet   := TODO

Number   := ([0-9]+(.[0-9]+)|.[0-9]+) SUFFIX1 SUFFIX2
Value    := Number SUFFIX1 SUFFIX2

```

#### End symbols
```
POTLABEL := R POT [0-9]+ [LIN|LOG|[]]
PLABEL   := R|L|C
ILABEL   := VIN
SLABEL   := VCC
DLABEL   := D
LABEL    := [A-Za-z0-9_]+
NODE     := [A-Za-z0-9_]+
ANYTHING := .*$
MODEL    := [A-Za-z0-9]+                # Subject to change
SUFFIX1  := t|g|meg|k|m|u|n|[]          # Case insensitive
SUFFIX2  := ohm|ohms|f|farads|h|henry|[]   # Case insensitive
```

Does not handle + signs (split lines)