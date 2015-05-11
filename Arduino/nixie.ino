#include <EtherCard.h>
#include <Time.h>
#include <Timezone.h>

#define N_SER 2
#define N_SCK 3
#define N_RCK 5
#define N_SCL 4
#define N_G   6

#define HV_SHDN 7

// Timezone: US East
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  //UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   //UTC - 5 hours
Timezone usEastern(usEDT, usEST);

// Ethernet
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xB1};
#define BUFFER_SIZE 800
byte Ethernet::buffer[BUFFER_SIZE];

// NTP
const char ntp_servname[] PROGMEM = "0.pool.ntp.org";
//uint8_t ntp_srv[] = {129,  6,   15,   29};

unsigned long millis_offset = 0;
uint8_t clientPort = 123;

// Tick scheduling
unsigned long update_interval = 1000L * 60L * 60L; // One hour
uint32_t lastUpdate = 0;
unsigned long update_display_interval = 1000L * 5L; // 5 seconds
uint32_t lastDisplayUpdate = 0;
uint32_t timeLong;

// Function prototypes
void write_595_time(uint8_t hours, uint8_t minutes, bool dp);

unsigned long getNtpTime(bool idle_anim) {
    unsigned long timeFromNTP;
    const unsigned long seventy_years = 2208988800UL;

    if (!ether.dnsLookup(ntp_servname)){
        Serial.println("DNS lookup failed");
        return 0;
    }

    ether.ntpRequest(ether.hisip, clientPort);

    int hours = 0;
    int minutes = 0;
    unsigned long idle_tick = 0;
    while(true){
        if (idle_anim && (millis() - idle_tick > 250)){
            idle_tick = millis();
            write_595_time(hours, minutes, false);
            hours = (hours + 1) % 24;
            minutes = (minutes + 1) % 60;
        }
        word length = ether.packetReceive();
        ether.packetLoop(length);
        if(length > 0 && ether.ntpProcessAnswer(&timeFromNTP, clientPort)) {
            return usEastern.toLocal(timeFromNTP - seventy_years);
        }
    }
    return 0;
}


void write_595_time(uint8_t hours, uint8_t minutes, bool dp){
    /*
       Shift register bit meanings:

    0, 1   : 10H Zero
    0, 2   : 10H Two
    0, 4   : 10H One
    0, 8   :  1H Zero
    0, 16  :  1H Nine
    0, 32  :  1H Eight
    0, 64  :  1H Seven
    0, 128 :  1H Six

    1, 1   :  1H Five
    1, 2   :  1H Four
    1, 4   :  1H Three
    1, 8   :  1H Two
    1, 16  :  1H One
    1, 32  :  1H Decimal
    1, 64  : 10M Zero
    1, 128 : 10M Five

    2, 1   : 10M Four
    2, 2   : 10M Three
    2, 4   : 10M Two
    2, 8   : 10M One
    2, 16  :  1M Zero
    2, 32  :  1M Nine
    2, 64  :  1M Eight
    2, 128 :  1M Seven

    2, 1   :  1M Six
    2, 2   :  1M Five
    2, 4   :  1M Four
    2, 8   :  1M Three
    2, 16  :  1M Two
    2, 32  :  1M One
    2, 64  :  1M Decimal
    2, 128 :  NC
    */


    uint8_t out[] = { 0, 0, 0, 0 };

    // Hour tens
    switch (hours / 10){
        case 0: out[0] |= 0x01; break;
        case 1: out[0] |= 0x04; break;
        case 2: out[0] |= 0x02; break;
    }

    // Hour ones
    switch (hours % 10){
        case 0: out[0] |= 0x08; break;
        case 9: out[0] |= 0x10; break;
        case 8: out[0] |= 0x20; break;
        case 7: out[0] |= 0x40; break;
        case 6: out[0] |= 0x80; break;
        case 5: out[1] |= 0x01; break;
        case 4: out[1] |= 0x02; break;
        case 3: out[1] |= 0x04; break;
        case 2: out[1] |= 0x08; break;
        case 1: out[1] |= 0x10; break;
    }

    // Minute tens
    switch (minutes / 10){
        case 0: out[1] |= 0x40; break;
        case 5: out[1] |= 0x80; break;
        case 4: out[2] |= 0x01; break;
        case 3: out[2] |= 0x02; break;
        case 2: out[2] |= 0x04; break;
        case 1: out[2] |= 0x08; break;
    }

    // Minute ones
    switch (minutes % 10){
        case 0: out[2] |= 0x10; break;
        case 9: out[2] |= 0x20; break;
        case 8: out[2] |= 0x40; break;
        case 7: out[2] |= 0x80; break;
        case 6: out[3] |= 0x01; break;
        case 5: out[3] |= 0x02; break;
        case 4: out[3] |= 0x04; break;
        case 3: out[3] |= 0x08; break;
        case 2: out[3] |= 0x10; break;
        case 1: out[3] |= 0x20; break;
    }

    if (dp){
        out[1] |= 0x20;
        out[3] |= 0x40;
    }

    Serial.print("Bit data: ");
    Serial.print(out[0], BIN);
    Serial.print(" ");
    Serial.print(out[1], BIN);
    Serial.print(" ");
    Serial.print(out[2], BIN);
    Serial.print(" ");
    Serial.println(out[3], BIN);

    // Output the data to the shift regs
    digitalWrite(N_RCK, LOW);
    shiftOut(N_SER, N_SCK, MSBFIRST, out[3]);
    shiftOut(N_SER, N_SCK, MSBFIRST, out[2]);
    shiftOut(N_SER, N_SCK, MSBFIRST, out[1]);
    shiftOut(N_SER, N_SCK, MSBFIRST, out[0]);
    digitalWrite(N_RCK, HIGH);
}

void print_time(){
    unsigned long time;
    time = (millis()/1000) + millis_offset;
    time = time % (60L * 60L * 24L);
    unsigned hours = (time / 60 / 60) % 24;
    unsigned minutes = (time / 60) % 60;
    unsigned seconds = time % 60;
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(".");
    Serial.println(seconds);
}

void update_offset(bool idle_anim){
    unsigned long time = getNtpTime(idle_anim);
    if (time){
        millis_offset = (time - (millis()/1000));
    }
}

bool dp = true;
void loop(){
    ether.packetLoop(ether.packetReceive());

    // Update the NTP offset each hour
    if(lastUpdate +  update_interval < millis()) {
        lastUpdate = millis();
        update_offset(false);
    }

    // Update the readout every ten seconds
    if (lastDisplayUpdate + update_display_interval < millis()){
        lastDisplayUpdate = millis();

        // Get the hours & minutes
        unsigned long time;
        time = (millis()/1000) + millis_offset;
        time = time % (60L * 60L * 24L);
        uint8_t hours = (time / 60 / 60) % 24;
        uint8_t minutes = (time / 60) % 60;

        // Print the time
        print_time();
        write_595_time(hours, minutes, dp);
        dp = !dp;
    }
}

void setup(){
    Serial.begin(19200);

    // HV Control pin, start with shutdown high
    pinMode(HV_SHDN, OUTPUT);
    digitalWrite(HV_SHDN, HIGH);

    // Shift register setup
    pinMode(N_SER, OUTPUT);
    pinMode(N_SCK, OUTPUT);
    pinMode(N_SCL, OUTPUT);
    pinMode(N_RCK, OUTPUT);
    pinMode(N_G, OUTPUT);
    digitalWrite(N_SCL, HIGH);
    digitalWrite(N_G, LOW);


    // Ethernet
    if (ether.begin(sizeof Ethernet::buffer, mac, 9) == 0){
        Serial.println("Couldn't init eth");
        while (1);
    }

    Serial.print("DHCP...");
    //ether.staticSetup(myip, gwip, gwip);
    //static byte dnsip[] = {8,8,8,8};
    //ether.copyIp(ether.dnsip, dnsip);
    if (!ether.dhcpSetup()){
        Serial.println(" failed.");
        while(1);
    }
    Serial.println("done.");

    lastUpdate = millis();
    Serial.println("Setup done");

    // Enable the HV system
    digitalWrite(HV_SHDN, LOW);

    // Get an initial NTP offset
    update_offset(true);
}
