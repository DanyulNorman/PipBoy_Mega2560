#ifndef PIPBOY_TEXT_H
#define PIPBOY_TEXT_H
#include <avr/pgmspace.h>
const char * const stat_SPEC[90] PROGMEM = {"Strength      ","Perception    ","Endurance     ","Charisma      ","Intelligence  ","Agility       ","Luck          "};
const char * const stat_SPEC_val[15] PROGMEM = {"10","12","11","13","10","15","12"};
const char * const stat_SEC_STAT[90] PROGMEM = {" STATUS "," EFFECTS "," SPECIAL "};
const char * const stat_SEC_ITEM[90] PROGMEM = {" APPAREL "," WEAPONS "," AID ", " MISC "};
const char * const stat_SEC_DATA[90] PROGMEM = {" MAIN "," SIDE "," DAILY "};

extern "C" const unsigned char vaultboy_status[] PROGMEM;
extern "C" const unsigned char laser_pistol[] PROGMEM;
extern "C" const unsigned char radiation[] PROGMEM;
extern "C" const unsigned char gun[] PROGMEM;
extern "C" const unsigned char shield[] PROGMEM;
extern "C" const unsigned char aim[] PROGMEM;
extern "C" const unsigned char voltage[] PROGMEM;
extern "C" const unsigned char helmet[] PROGMEM;
extern "C" const unsigned char caps[] PROGMEM;
extern "C" const unsigned char wgt[] PROGMEM;
extern "C" const unsigned char strength[] PROGMEM;
extern "C" const unsigned char perception[] PROGMEM;
extern "C" const unsigned char endurance[] PROGMEM;
extern "C" const unsigned char charisma[] PROGMEM;
extern "C" const unsigned char intelligence[] PROGMEM;
extern "C" const unsigned char agility[] PROGMEM;
extern "C" const unsigned char luck[] PROGMEM;
extern "C" const unsigned char vault_tec_logo[] PROGMEM;
extern "C" const unsigned char VaultBoy2[] PROGMEM;

const char * const stat_SPEC_desc[] PROGMEM = {
    "Measures your raw physical power.",
    "It affects your ability in combat\,",
    "as well as your carry weight.",
    " ",

    "Your sense of environmental",
    "awareness. It affects your ability",
    "to detect threats\, find items\,",
    "and navigate the world.",

    "Measures your overall physical",
    "fitness. It affects your health",
    "points and your resistance to",
    "environmental hazards.",

    "Your ability to work with others.",
    "It affects your interactions with",
    "NPCs and your effectiveness in",
    "certain dialogue options.",

    "Measures your overall mental",
    "acuity. It affects your ability",
    "to learn new skills and your",
    "effectiveness with technology.",

    "Measures your overall fitness and",
    "reflexes. It affects your agility",
    "in combat and your ability to sneak.",
    " ",

    "Measures your general good fortune.",
    "It affects your chances of success",
    "in various actions and finding",
    "rare items."
};

// Weapons
const char * const WEAP[] PROGMEM = {"Gauss Rifle", "Tesla Rifle", "Plasma Pistol", "Combat Shotgun", "Assault Rifle"};
// Define an array of arrays for weapon stats
const char * const DAMAGE[] PROGMEM = {
    "Damage: 55",
    "Damage: 70",
    "Damage: 35",
    "Damage: 80",
    "Damage: 40"
};

const char * const RANGE[] PROGMEM = {
    "Range: Long",
    "Range: Long",
    "Range: Medium",
    "Range: Short",
    "Range: Medium"
};

const char * const WEIGHT[] PROGMEM = {
    "Weight: 10",
    "Weight: 8",
    "Weight: 5",
    "Weight: 12",
    "Weight: 9"
};

const char  * const AMMO[] PROGMEM = {
    "Ammo: 5.56mm",
    "Ammo: MF Cell",
    "Ammo: Plasma Cart",
    "Ammo: Shotgun Shell",
    "Ammo: 5.56mm"
};
const char  * const WEAP_IMG[] PROGMEM = {
    laser_pistol,laser_pistol,laser_pistol,laser_pistol,laser_pistol,laser_pistol
};
// Create an array of pointers to the above arrays
const char* const WEAP_STATS[] PROGMEM = { 
    (const char*)DAMAGE,
    (const char*)RANGE,
    (const char*)WEIGHT,
    (const char*)AMMO
};


// Aid (Items)
const char * const AID[] PROGMEM = {"Stimpak", "Rad-X", "Psycho", "Med-X", "Buffout", "RadAway", "Jet", "Stealth Boy", "Nuka-Cola"};
const char * const AID_STATS[] PROGMEM = {
    "Heals: 50 HP|Weight: 0.1|Value: 25 caps|Addiction: None",
    "Reduces Radiation by 25%|Weight: 0.2|Value: 20 caps|Addiction: None",
    "Increases Damage by 25% for 1 minute|Weight: 0.2|Value: 30 caps|Addiction: Psycho addiction",
    "Reduces Damage taken by 50% for 1 minute|Weight: 0.3|Value: 35 caps|Addiction: None",
    "Increases Strength and Endurance by 2 for 1 minute|Weight: 0.2|Value: 40 caps|Addiction: None",
    "Removes 50 Radiation|Weight: 0.2|Value: 30 caps|Addiction: None",
    "Increases Action Points by 50 for 1 minute|Weight: 0.2|Value: 35 caps|Addiction: Jet addiction",
    "Grants invisibility for 1 minute|Weight: 0.3|Value: 50 caps|Addiction: None",
    "Quenches thirst and restores 20 HP|Weight: 0.3|Value: 15 caps|Addiction: None"
};

// Misc/Junk
const char * const MISC[] PROGMEM = {"Abraxo Cleaner", "Nuka-Cola Quantum", "Pre-War Money", "Duct Tape", "Wonderglue", "Teddy Bear", "Deathclaw Hand"};
const char * const MISC_STATS[] PROGMEM = {
    "Weight: 0.5|Value: 2 caps",
    "Weight: 1|Value: 10 caps",
    "Weight: 0.1|Value: 5 caps",
    "Weight: 0.2|Value: 3 caps",
    "Weight: 0.2|Value: 3 caps",
    "Weight: 0.5|Value: 1 cap",
    "Weight: 1.5|Value: 15 caps"
};

// Quests
const char * const QUEST[] PROGMEM = {"Reclamation Day", "Following in His Footsteps", "The Waters of Life", "Into the Fire", "Find a Bathroom", "The Silver Shroud", "Kid in a Fridge"};
const char * const QUEST_DESC[] PROGMEM = {
    "Begin your journey from Vault 76.",
    "Follow the footsteps of your father.",
    "Retrieve the Garden of Eden Creation Kit.",
    "Join the Fire Breathers.",
    "Locate a clean restroom.",
    "Become the Silver Shroud.",
    "Rescue a child trapped in a refrigerator."
};


const char * const RAD_0[13] PROGMEM = {"Turbo Kid Radio"};
const char * const RAD_0_desc[15] PROGMEM {"Sounds of the wasteland."};


const char * const BOOTTEXT[] PROGMEM = {
    "*************** PIP-05 (R) V7 .1.0.8 ***************",
    " ",
    " ",
    " ",
    "COPYRIGHT 2075 ROBCO(R)",
    "LOADER VI.1",
    "EXEC VERSION 41.10",
    "264k RAM SYSTEM",
    "BYTES FREE 200k",
    "NO HOLOTAPE FOUND",
    "LOAD ROM(1): DEITRIX 303"
};
const char * const STARTUPTEXT[] PROGMEM =  {
  "*1 0 0X0000A4 0x000000000000000000X0000A4 0x000000 00000000000",
  "300 X0000 A4 0x000000000 000000000X0000A4 0x0000000000 0000000" ,
  "50 0X 0000A4 0x00000000000000 0000X0000A4 0x000 00000000000000",
  "700X 000 0A4 0x00000000000000000 0X0000A4 0x000000000 0000000*",
  "700X 000 0A4 0x00000000000000000 0X0000A4 0x000000000 0000000*",
  "700X 000 0A4 0x00000000000000000 0X0000A4 0x000000000 0000000*",
  "700X 000 0A4 0x00000000000000000 0X0000A4 0x000000000 0000000*"};
#endif