void setup() {
    Serial1.begin(115200);
}

void loop() {
    Serial1.write(7);
    delay(1000);
    Serial1.write(128);
    delay(5000);
    Serial1.write(131);
    delay(5000);
    Serial1.write(132);
    delay(1000);
    Serial1.write(146);
    Serial1.write(255);
    Serial1.write(128);
    Serial1.write(255);
    Serial1.write(128);
    delay(5000);
    Serial1.write(173);
    delay(4000);
}
