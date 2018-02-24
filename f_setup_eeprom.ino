/************************************ EEPROM setup ******************************************/
void setup_EEPROM() {
  EEPROM.begin(512);
}

// Reads a string out of memory
String read_EEPROM(int l, int p) {
  String temp;
  for (int n = p; n < l + p; ++n)
  {
    if (char(EEPROM.read(n)) != ';') {
      if (isWhitespace(char(EEPROM.read(n)))) {
        //do nothing
      } else temp += String(char(EEPROM.read(n)));

    } else n = l + p;

  }
  return temp;
}

// write to memory
void write_EEPROM(String x, int pos) {
  for (int n = pos; n < x.length() + pos; n++) {
    EEPROM.write(n, x[n - pos]);
  }
}
