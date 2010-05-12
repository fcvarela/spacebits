#include <avr/interrupt.h>
#include <NewSoftSerial.h>
#include <NMEANewSoftSerial.h>

// SMS timer control + counters
uint8_t tcnt2;
static uint16_t ticks = 0;
uint8_t run_task_flag = 0;

// initial timeout = 5 seconds
uint16_t timeout = 5000;
uint8_t task = 0;

// nmea handler
NewSoftSerial nmea(3, 2);
NMEANewSoftSerial nmeaHandler(&nmea);


void setup() {
    // setup led
    pinMode(13, OUTPUT);

    // let modem boot up
    delay(3000);

    // setup serial ports and flush read buffers
    Serial.begin(115200);
    nmea.begin(4800);
    Serial.flush();
    nmea.flush();

    // start HW counter
    start_timer2();
}


void loop() {
    // give the nmea parser some cpu
    nmeaHandler.packetReady();

    // check if we're just waiting and clear buffers
    if (run_task_flag == 0) {
        Serial.flush();
        return;
    }

    // not waiting. do something
    run_task();
}


void run_task() {
    switch (task) {
    case 0:
        reset_modem();
        timeout = 2000;
        break;
        
    case 1: 
        setup_modem();
        timeout = 2000;
        break;
        
    case 2:
        send_at();
        timeout = 2000;
        break;
        
    case 3:
        send_sms_header();
        timeout = 2000;
        break;
        
    case 4:
        send_sms_body();
        timeout = 30000;
        break;
    }
    
    if (++task == 5) task = 0;
    
    run_task_flag = 0;
}


void reset_modem() {
    Serial.write("ATZ");
    Serial.write(13);
}


void setup_modem() {
    Serial.write("AT+CMGF=1");
    Serial.write(13);
}


void send_at() {
    Serial.write("AT");
    Serial.write(13);
}


void send_sms_header() {
    digitalWrite(13, HIGH);
    Serial.write("AT+CMGS=");
    Serial.write(34);
    Serial.write("44100438320323");
    Serial.write(34);
    Serial.write(13);
}


void send_sms_body() {
    Serial.write("http://maps.google.com/maps?q=");
    Serial.print(nmeaHandler.f_latitude, 4);
    Serial.write(",");
    Serial.println(nmeaHandler.f_longitude, 4);
    Serial.write(" Altitude: ");
    Serial.print(nmeaHandler.u_altitude);
    Serial.write(" Satellites: ");
    Serial.print(nmeaHandler.u_satellites);
    Serial.write(" FIX Strength: ");
    switch (nmeaHandler.u_valid) {
        case 0: Serial.write("invalid"); break;
        case 1: Serial.write("valid sps"); break;
        case 2: Serial.write("valid dgps"); break;
        case 3: Serial.write("valid pps"); break;
    }
    Serial.write(26);
}


ISR(TIMER2_OVF_vect) {
    TCNT2 = tcnt2;
    ticks++;

    if (ticks != timeout)
        return;

    run_task_flag = 1;
    ticks = 0;
}


void start_timer2() {
    TIMSK2 &= ~(1<<TOIE2);
    TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
    TCCR2B &= ~(1<<WGM22);
    ASSR &= ~(1<<AS2);
    TIMSK2 &= ~(1<<OCIE2A);
    TCCR2B |= (1<<CS22) | (1<<CS20);
    TCCR2B &= ~(1<<CS21);
    tcnt2 = 131;
    TCNT2 = tcnt2;
    TIMSK2 |= (1<<TOIE2);
}
