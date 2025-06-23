#include <Arduino.h>
#include <ACS712.h>

ACS712 sensor(A0, 5.0, 1023); // pin A0, Vref = 5.0V, ADC 10 bits

// Variables pour filtre de Kalman
float estimate = 0.0;
float error_estimate = 1.0;
float error_measure = 5.0;   // bruit de mesure estimé (à ajuster)
float q = 0.1;               // bruit système (à ajuster)

void setup() {
    Serial.begin(9600);
    sensor.setmVperAmp(100.0);   // Pour module 20A (100 mV/A)
    sensor.autoMidPoint();       // Auto calibration à vide
}

// Fonction de filtrage Kalman
float kalmanFilter(float measurement) {
    float kalman_gain = error_estimate / (error_estimate + error_measure);
    estimate = estimate + kalman_gain * (measurement - estimate);
    error_estimate = (1 - kalman_gain) * error_estimate + fabs(estimate - measurement) * q;
    return estimate;
}

// Moyennage simple sur N échantillons
float moyenneDC(int n = 100) {
    float somme = 0;
    for (int i = 0; i < n; i++) {
        somme += sensor.mA_DC();
        delay(1);
    }
    return somme / n;
}

void loop() {
    float courant_brut = moyenneDC();           // Moyenne simple
    float courant_filtre = kalmanFilter(courant_brut);  // Filtre Kalman

    // Affichage des deux pour Serial Plotter
    Serial.print(courant_brut);
    Serial.print(", ");
    Serial.println(courant_filtre);

    delay(1000);
}
