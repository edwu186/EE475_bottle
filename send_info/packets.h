// Packet Structure
// 0                     15      17   18   19
// |         GUID        |  BID  | i1 | i2 |

#define PACKET_LEN 10

static const byte UUID[2] = { 0xFF, 0xFF };

static const byte BOTTLE_ID[4] = {0x00, 0x00, 0x00, 0x00}; // TODO: store in EEPROM

static const byte BEACON_P[10]    = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x00, 0x00};
static const byte REGISTER_P[10]  = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x01, 0x00};
static const byte CLOSE_P[10]     = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x02, 0x00};
static const byte OPEN_P[10]      = {UUID[0], UUID[1], BOTTLE_ID[0], BOTTLE_ID[1], BOTTLE_ID[2], BOTTLE_ID[3], 0x02, 0x00};