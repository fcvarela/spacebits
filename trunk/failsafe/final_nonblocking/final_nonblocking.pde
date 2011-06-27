#define BALLOON_IDX 6

#include <avr/interrupt.h>
#include <NewSoftSerial.h>
#include <NMEA.h>

// SMS timer control + counters
uint8_t tcnt2;
static uint64_t ticks = 0;
uint8_t run_task_flag = 0;

// initial timeout = 5 seconds
uint64_t timeout = 5000;
uint8_t task = 0;

// nmea handler
NewSoftSerial gsm(2, 3);
NMEA nmeaHandler(&Serial);

// gsm status
uint8_t gsm_registered_pin = 7;
uint8_t gsm_ready_pin = 8;

// gsm status buffer
char gsm_buff[80];
uint8_t gsm_buff_idx;
uint8_t gsm_registered = 0;
uint8_t gsm_ready = 0;
uint8_t gsm_ring = 0;

void setup() {
    // setup led
    pinMode(13, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(gsm_registered_pin, OUTPUT);
    pinMode(gsm_ready_pin, OUTPUT);

    // let modem boot up
    delay(3000);

    // setup serial ports and flush read buffers
    Serial.begin(4800);
    gsm.begin(9600);
    Serial.flush();
    gsm.flush();

    // start HW counter
    start_timer2();
    
    // init buff idx
    gsm_buff_idx = 0;
}


void loop() {
    // give the nmea parser some cpu
    nmeaHandler.packetReady();

    // if we're just waiting, take the time to check the gsm status
    if (run_task_flag == 0) {
        parse_gsm();
        return;
    }

    // not waiting. do something
    run_task();
    run_task_flag = 0;
}

void parse_gsm() {
    char c;
    
    while (gsm.available() > 0) {
        c = gsm.read();
        if (gsm_buff_idx >= 80) {
            // overflow... reset
            gsm_buff_idx = 0;
        }
        gsm_buff[gsm_buff_idx++] = c;
        gsm_buff[gsm_buff_idx] = 0;
    }
    
    if (gsm_buff[gsm_buff_idx-1] == '\n') {
        if (strstr(gsm_buff, "SIND: 11") != 0) {
            //Serial.println("REGISTERED");
            gsm_registered = 1;
        }
        
        if (strstr(gsm_buff, "SIND: 4") != 0) {
            //Serial.println("READY");
            gsm_ready = 1;
        }
        
        if (strstr(gsm_buff, "RING") != 0) {
            //Serial.println("RINGING");
            gsm_ring = 1;
        }
            
        gsm_buff_idx = 0;
    }
    
    if (gsm_registered) digitalWrite(gsm_registered_pin, HIGH);
    else digitalWrite(gsm_registered_pin, LOW);
    
    if (gsm_ready) digitalWrite(gsm_ready_pin, HIGH);
    else digitalWrite(gsm_ready_pin, LOW);
}

void run_task() {
    if (gsm_ready == 0) {
        timeout = 2000;
        return;
    }
    
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
        timeout = 20000;
        break;
    }
    
    task++;
    if (task == 5)
        task = 3;
}

void reset_modem() {
    gsm.print("AT+VTD=255");
    gsm.print(13, BYTE);
}

void setup_modem() {
    gsm.print("AT+CMGF=1");
    gsm.print(13, BYTE);
}

void send_at() {
    gsm.print("AT+SBAND=0");
    gsm.print(13, BYTE);
}


void send_sms_header() {
    digitalWrite(13, HIGH);
    digitalWrite(9, HIGH);
    gsm.print("AT+CMGS=\"+351968252265\"");
    gsm.print(13, BYTE);
}


void send_sms_body() {
    gsm.print(BALLOON_IDX, DEC);
    gsm.print(",");
    gsm.print(nmeaHandler.f_latitude, 4);
    gsm.print(",");
    gsm.print(nmeaHandler.f_longitude, 4);
    gsm.print(",");
    gsm.print(nmeaHandler.u_altitude, DEC);
    gsm.print(",");
    gsm.print(nmeaHandler.u_satellites, DEC);
    gsm.print(",");
    gsm.print(1, DEC);
    gsm.print(26, BYTE);
    digitalWrite(13, LOW);
    digitalWrite(9, LOW);
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
