#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#include <MLEDScroll.h>

MLEDScroll matrix;

const char * ssid = "lrs";
const char * password = "<password>";
static int nextPos = 0;

AsyncUDP udp;

struct UDPPacket
{
    float time; //Time
    float laptime; // Time of Current Lap
    float lapdistance; // Distance Driven on Current Lap
    float totaldistance; // Distance Driven Overall
    float x; // Position X
    float y; // Position Y
    float z; // Position Z
    float speed; // Velocity (Speed) [m/s]
    float vx; // Velocity X
    float vy; // Velocity Y
    float vz; // Velocity Z
    float rx; // Roll Vector X
    float ry; // Roll Vector Y
    float rz; // Roll Vector Z
    float px; // Pitch Vector X
    float py; // Pitch Vector Y
    float pz; // Pitch Vector Z
    float srl; // Position of Suspension Rear Left
    float srr;// Position of Suspension Rear Right
    float sfl; // Position of Suspension Front Left
    float sfr; // Position of Suspension Front Right
    float vsrl; // Velocity of Suspension Rear Left
    float csrr; // Velocity of Suspension Rear Right
    float vsfl; // Velocity of Suspension Front Left
    float vsfr; // Velocity of Suspension Front Right
    float vwrl; // Velocity of Wheel Rear Left
    float vwrr; // Velocity of Wheel Rear Right
    float vwfl; // Velocity of Wheel Front Left
    float vwfr; // Velocity of Wheel Front Right
    float throttle; // Position Throttle
    float steer; // Position Steer
    float brake; // Position Brake
    float clutch; // Position Clutch
    float gear; // Gear [0 = Neutral, 1 = 1, 2 = 2, ..., 10 = Reverse]
    float glat; // G-Force Lateral
    float glong; // G-Force Longitudinal
    float currentlap; // Current Lap
    float rpmdiv10; // Speed of Engine [rpm / 10]
    float h1;// ?
    float h2;// ?
    float h3;// ?
    float h4;// ?
    float h5;// ?
    float h6;// ?
    float h7;// ?
    float h8;// ?
    float h9;// ?
    float h10;// ?
    float h11;// ?
    float h12;// ?
    float h13;// ?
    float tbrl;// Temperature Brake Rear Left ?
    float tbrr;// Temperature Brake Rear Right ?
    float tbfl;// Temperature Brake Front Left ?
    float tbfr;// Temperature Brake Front Right ?
    float h14;// ?
    float h15;// ?
    float h16;// ?
    float h17;// ?
    float h18;// ?
    float numlaps; // Number of Laps in Total ?
    float tracklength;// Length of Track in Total
    float h19;// ?
    float maxrpmdiv10;// Maximum rpm / 10
};

UDPPacket blaat;

void updateLeds(float currentGear) {
    if (currentGear == 0) {
        matrix.character("R");
    } else if (currentGear == 1) {
        matrix.character("N");
    } else {
        char chrGear[1];
        dtostrf(currentGear - 1.0, 1, 0, chrGear);
        matrix.character(chrGear); 
    }
}

void setup()
{    
    // Initialize the matrix
    matrix.begin();
    // serial en wifi init
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    matrix.character("X");
    Serial.println("start udp handler");
    // delay(1000);
    // udp ontvang magic en decode data packet
    if(udp.listen(20777)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            memcpy(&blaat, packet.data(), sizeof(blaat));

            // Update the gear indicator
            updateLeds(blaat.gear);
        });
    }
}

void loop() {
}


// for extradata = 3: output = 64 floats
// byteformat: little endian

// Position Info | Content Info
// No.	Byte	| Format Value

// 0.	0	float	Time
// 1.	4	float	Time of Current Lap
// 2.	8	float	Distance Driven on Current Lap
// 3.	12	float	Distance Driven Overall
// 4.	16	float	Position X
// 5.	20	float	Position Y
// 6.	24	float	Position Z
// 7.	28	float	Velocity (Speed) [m/s]
// 8.	32	float	Velocity X
// 9.	36	float	Velocity Y
// 10.	40	float Velocity Z
// 11.	44	float	Roll Vector X
// 12.	48	float	Roll Vector Y
// 13.	52	float	Roll Vector Z
// 14.	56	float	Pitch Vector X
// 15.	60	float	Pitch Vector Y
// 16.	64	float	Pitch Vector Z
// 17.	68	float	Position of Suspension Rear Left
// 18.	72	float	Position of Suspension Rear Right
// 19.	76	float	Position of Suspension Front Left
// 20.	80	float	Position of Suspension Front Right
// 21.	84	float	Velocity of Suspension Rear Left
// 22.	88	float	Velocity of Suspension Rear Right
// 23.	92	float	Velocity of Suspension Front Left
// 24.	96	float	Velocity of Suspension Front Right
// 25.	100	float	Velocity of Wheel Rear Left
// 26.	104	float	Velocity of Wheel Rear Right
// 27.	108	float	Velocity of Wheel Front Left
// 28.	112	float	Velocity of Wheel Front Right
// 29.	116	float	Position Throttle
// 30.	120	float	Position Steer
// 31.	124	float	Position Brake
// 32.	128	float	Position Clutch
// 33.	132	float	Gear [0 = Neutral, 1 = 1, 2 = 2, ..., 10 = Reverse]
// 34.	136	float	G-Force Lateral
// 35.	140	float	G-Force Longitudinal
// 36.	144	float	Current Lap
// 37.	148	float	Speed of Engine [rpm / 10]
// 38.	152	float	?
// 39.	156	float	?
// 40.	160	float	?
// 41.	164	float	?
// 42.	168	float	?
// 43.	172	float	?
// 44.	176	float	?
// 45.	180	float	?
// 46.	184	float	?
// 47.	188	float	?
// 48.	192	float	?
// 49.	196	float	?
// 50.	200	float	?
// 51.	204	float	Temperature Brake Rear Left ?
// 52.	208	float	Temperature Brake Rear Right ?
// 53.	212	float	Temperature Brake Front Left ?
// 54.	216	float	Temperature Brake Front Right ?
// 55.	220	float	?
// 56.	224	float	?
// 57.	228	float	?
// 58.	232	float	?
// 59.	236	float	?
// 60.	240	float	Number of Laps in Total ?
// 61.	244	float	Length of Track in Total
// 62.	248	float	?
// 63.	252	float	Maximum rpm / 10

// http://forums.codemasters.com/discussion/46726/d-box-and-udp-telemetry-information
// struct UDPPacket
// {
//     float m_time;
//     float m_lapTime;
//     float m_lapDistance;
//     float m_totalDistance;
//     float m_x;      // World space position
//     float m_y;      // World space position
//     float m_z;      // World space position
//     float m_speed;
//     float m_xv;      // Velocity in world space
//     float m_yv;      // Velocity in world space
//     float m_zv;      // Velocity in world space
//     float m_xr;      // World space right direction
//     float m_yr;      // World space right direction
//     float m_zr;      // World space right direction
//     float m_xd;      // World space forward direction
//     float m_yd;      // World space forward direction
//     float m_zd;      // World space forward direction
//     float m_susp_pos_bl;
//     float m_susp_pos_br;
//     float m_susp_pos_fl;
//     float m_susp_pos_fr;
//     float m_susp_vel_bl;
//     float m_susp_vel_br;
//     float m_susp_vel_fl;
//     float m_susp_vel_fr;
//     float m_wheel_speed_bl;
//     float m_wheel_speed_br;
//     float m_wheel_speed_fl;
//     float m_wheel_speed_fr;
//     float m_throttle;
//     float m_steer;
//     float m_brake;
//     float m_clutch;
//     float m_gear;
//     float m_gforce_lat;
//     float m_gforce_lon;
//     float m_lap;
//     float m_engineRate;
//     float m_sli_pro_native_support; // SLI Pro support
//     float m_car_position;   // car race position
//     float m_kers_level;    // kers energy left
//     float m_kers_max_level;   // kers maximum energy
//     float m_drs;     // 0 = off, 1 = on
//     float m_traction_control;  // 0 (off) - 2 (high)
//     float m_anti_lock_brakes;  // 0 (off) - 1 (on)
//     float m_fuel_in_tank;   // current fuel mass
//     float m_fuel_capacity;   // fuel capacity
//     float m_in_pits;    // 0 = none, 1 = pitting, 2 = in pit area
//     float m_sector;     // 0 = sector1, 1 = sector2; 2 = sector3
//     float m_sector1_time;   // time of sector1 (or 0)
//     float m_sector2_time;   // time of sector2 (or 0)
//     float m_brakes_temp[4];   // brakes temperature (centigrade)
//     float m_wheels_pressure[4];  // wheels pressure PSI
//     float m_team_info;    // team ID 
//     float m_total_laps;    // total number of laps in this race
//     float m_track_size;    // track size meters
//     float m_last_lap_time;   // last lap time
//     float m_max_rpm;    // cars max RPM, at which point the rev limiter will kick in
//     float m_idle_rpm;    // cars idle RPM
//     float m_max_gears;    // maximum number of gears
//     float m_sessionType;   // 0 = unknown, 1 = practice, 2 = qualifying, 3 = race
//     float m_drsAllowed;    // 0 = not allowed, 1 = allowed, -1 = invalid / unknown
//     float m_track_number;   // -1 for unknown, 0-21 for tracks
//     float m_vehicleFIAFlags;  // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
//  };