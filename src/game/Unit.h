/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __UNIT_H
#define __UNIT_H

#include "Common.h"
#include "Object.h"
#include "Opcodes.h"
#include "SpellAuraDefines.h"
#include "UpdateFields.h"
#include "SharedDefines.h"
#include "ThreatManager.h"
#include "HostileRefManager.h"
#include "FollowerReference.h"
#include "FollowerRefManager.h"
#include "MapManager.h"
#include "MotionMaster.h"
#include "DBCStructure.h"
#include "Path.h"
#include "WorldPacket.h"
#include "SpellAuras.h"
#include "Timer.h"
#include <list>
#include "StateMgr.h"

enum SpellInterruptFlags
{
    SPELL_INTERRUPT_FLAG_MOVEMENT     = 0x01,
    SPELL_INTERRUPT_FLAG_DAMAGE       = 0x02,
    SPELL_INTERRUPT_FLAG_UNK3         = 0x04,
    SPELL_INTERRUPT_FLAG_INTERRUPT    = 0x08,
    SPELL_INTERRUPT_FLAG_ABORT_ON_DMG = 0x10,               // _complete_ interrupt on direct damage
    //SPELL_INTERRUPT_UNK             = 0x20                // unk, 564 of 727 spells having this spell start with "Glyph"
};

enum SpellChannelInterruptFlags
{
    CHANNEL_FLAG_DAMAGE      = 0x0002,
    CHANNEL_FLAG_MOVEMENT    = 0x0008,
    CHANNEL_FLAG_TURNING     = 0x0010,
    CHANNEL_FLAG_DAMAGE2     = 0x0080,
    CHANNEL_FLAG_DELAY       = 0x4000
};

enum SpellAuraInterruptFlags
{
    AURA_INTERRUPT_FLAG_UNK0                        = 0x00000001,   // 0    removed when getting hit by a negative spell?
    AURA_INTERRUPT_FLAG_DAMAGE                      = 0x00000002,   // 1    removed by any damage
    AURA_INTERRUPT_FLAG_CAST                        = 0x00000004,   // 2
    AURA_INTERRUPT_FLAG_MOVE                        = 0x00000008,   // 3    removed by any movement
    AURA_INTERRUPT_FLAG_TURNING                     = 0x00000010,   // 4    removed by any turning
    AURA_INTERRUPT_FLAG_ENTER_COMBAT                = 0x00000020,   // 5    removed by entering combat
    AURA_INTERRUPT_FLAG_NOT_MOUNTED                 = 0x00000040,   // 6    removed by unmounting
    AURA_INTERRUPT_FLAG_NOT_ABOVEWATER              = 0x00000080,   // 7    removed by entering water
    AURA_INTERRUPT_FLAG_NOT_UNDERWATER              = 0x00000100,   // 8    removed by leaving water
    AURA_INTERRUPT_FLAG_NOT_SHEATHED                = 0x00000200,   // 9    removed by unsheathing
    AURA_INTERRUPT_FLAG_ACTION                      = 0x00000400,   // 10   removed by any action (talk/loot/gossip)
    AURA_INTERRUPT_FLAG_USE                         = 0x00000800,   // 11   removed by casting a spell
    AURA_INTERRUPT_FLAG_MELEE_ATTACK                = 0x00001000,   // 12   removed by attack?
    AURA_INTERRUPT_FLAG_SPELL_ATTACK                = 0x00002000,   // 13
    AURA_INTERRUPT_FLAG_UNK14                       = 0x00004000,   // 14
    AURA_INTERRUPT_FLAG_TRANSFORM                   = 0x00008000,   // 15   removed by casting a transform spell?
    AURA_INTERRUPT_FLAG_UNK16                       = 0x00010000,   // 16
    AURA_INTERRUPT_FLAG_MOUNTING                    = 0x00020000,   // 17   removed by mounting
    AURA_INTERRUPT_FLAG_NOT_SEATED                  = 0x00040000,   // 18   removed by standing up (used by food and drink mostly and sleep/Fake Death like)
    AURA_INTERRUPT_FLAG_CHANGE_MAP                  = 0x00080000,   // 19   leaving map/getting teleported
    AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION    = 0x00100000,   // 20   removed by auras that make you invulnerable, or make other to loose selection on you
    AURA_INTERRUPT_FLAG_UNK21                       = 0x00200000,   // 21
    AURA_INTERRUPT_FLAG_TELEPORTED                  = 0x00400000,   // 22
    AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT            = 0x00800000,   // 23   removed by entering pvp combat
    AURA_INTERRUPT_FLAG_DIRECT_DAMAGE               = 0x01000000,   // 24   removed by any direct damage
    AURA_INTERRUPT_FLAG_LANDING                     = 0x02000000,   // 25   removed by hitting the ground
    AURA_INTERRUPT_FLAG_UNK26                       = 0x04000000,   // 26
    AURA_INTERRUPT_FLAG_DAMAGE2                     = 0x08000000,   // 27   removed by damage spells + removed by damage, other than .. (diseases, Censure)
    AURA_INTERRUPT_FLAG_ENTER_COMBAT2               = 0x10000000,   // 28
    AURA_INTERRUPT_FLAG_UNK29                       = 0x20000000,   // 29
    AURA_INTERRUPT_FLAG_UNK30                       = 0x40000000,   // 30
    AURA_INTERRUPT_FLAG_UNK31                       = 0x80000000,   // 31
};

enum SpellModOp
{
    SPELLMOD_DAMAGE                 = 0,
    SPELLMOD_DURATION               = 1,
    SPELLMOD_THREAT                 = 2,
    SPELLMOD_EFFECT1                = 3,
    SPELLMOD_CHARGES                = 4,
    SPELLMOD_RANGE                  = 5,
    SPELLMOD_RADIUS                 = 6,
    SPELLMOD_CRITICAL_CHANCE        = 7,
    SPELLMOD_ALL_EFFECTS            = 8,
    SPELLMOD_NOT_LOSE_CASTING_TIME  = 9,
    SPELLMOD_CASTING_TIME           = 10,
    SPELLMOD_COOLDOWN               = 11,
    SPELLMOD_EFFECT2                = 12,
    SPELLMOD_IGNORE_ARMOR           = 13,
    SPELLMOD_COST                   = 14,
    SPELLMOD_CRIT_DAMAGE_BONUS      = 15,
    SPELLMOD_RESIST_MISS_CHANCE     = 16,
    SPELLMOD_JUMP_TARGETS           = 17,
    SPELLMOD_CHANCE_OF_SUCCESS      = 18,                   // Only used with SPELL_AURA_ADD_FLAT_MODIFIER and affects proc spells
    SPELLMOD_ACTIVATION_TIME        = 19,
    SPELLMOD_EFFECT_PAST_FIRST      = 20,
    SPELLMOD_GLOBAL_COOLDOWN        = 21,
    SPELLMOD_DOT                    = 22,
    SPELLMOD_EFFECT3                = 23,
    SPELLMOD_SPELL_BONUS_DAMAGE     = 24,
    // spellmod 25 unused
    SPELLMOD_FREQUENCY_OF_SUCCESS   = 26,                   // Only used with SPELL_AURA_ADD_PCT_MODIFIER and affects used on proc spells
    SPELLMOD_MULTIPLE_VALUE         = 27,
    SPELLMOD_RESIST_DISPEL_CHANCE   = 28,
    SPELLMOD_UNK29                  = 29,                   // used by single test spell 45365
    SPELLMOD_COST_ON_HIT_FAIL       = 30
};

#define MAX_SPELLMOD 32

enum SpellFacingFlags
{
    SPELL_FACING_FLAG_INFRONT = 0x0001
};

#define DEFAULT_COMBAT_REACH   1.5f

#define BASE_MELEERANGE_OFFSET 1.33f
#define BASE_MINDAMAGE 1.0f
#define BASE_MAXDAMAGE 2.0f
#define BASE_ATTACK_TIME 2000
#define BASE_BLOCK_DAMAGE_PERCENT 30

// byte value (UNIT_FIELD_BYTES_1,0)
enum UnitStandStateType
{
    UNIT_STAND_STATE_STAND             = 0,
    UNIT_STAND_STATE_SIT               = 1,
    UNIT_STAND_STATE_SIT_CHAIR         = 2,
    UNIT_STAND_STATE_SLEEP             = 3,
    UNIT_STAND_STATE_SIT_LOW_CHAIR     = 4,
    UNIT_STAND_STATE_SIT_MEDIUM_CHAIR  = 5,
    UNIT_STAND_STATE_SIT_HIGH_CHAIR    = 6,
    UNIT_STAND_STATE_DEAD              = 7,
    UNIT_STAND_STATE_KNEEL             = 8,
    UNIT_STAND_STATE_CUSTOM            = 9                  // Depends on model animation. Submerge, freeze, hide, hibernate, rest
};

#define MAX_UNIT_STAND_STATE             10

// byte flags value (UNIT_FIELD_BYTES_1,1)
// This corresponds to free talent points (pet case)

// byte flags value (UNIT_FIELD_BYTES_1,2)
enum UnitStandFlags
{
    UNIT_STAND_FLAGS_UNK1         = 0x01,
    UNIT_STAND_FLAGS_CREEP        = 0x02,
    UNIT_STAND_FLAGS_UNTRACKABLE  = 0x04,
    UNIT_STAND_FLAGS_UNK4         = 0x08,
    UNIT_STAND_FLAGS_UNK5         = 0x10,
    UNIT_STAND_FLAGS_ALL          = 0xFF
};

// byte flags value (UNIT_FIELD_BYTES_1,3)
enum UnitBytes1_Flags
{
    UNIT_BYTE1_FLAG_ALWAYS_STAND = 0x01,
    UNIT_BYTE1_FLAG_HOVER        = 0x02,                    // Creature that can fly and are not on the ground appear to have this flag. If they are on the ground, flag is not present.
    UNIT_BYTE1_FLAG_UNK_3        = 0x04,
    UNIT_BYTE1_FLAG_ALL          = 0xFF
};

// byte value (UNIT_FIELD_BYTES_2,0)
enum SheathState
{
    SHEATH_STATE_UNARMED  = 0,                              // non prepared weapon
    SHEATH_STATE_MELEE    = 1,                              // prepared melee weapon
    SHEATH_STATE_RANGED   = 2                               // prepared ranged weapon
};

#define MAX_SHEATH_STATE    3

// byte flags value (UNIT_FIELD_BYTES_2,1)
enum UnitPVPStateFlags
{
    UNIT_BYTE2_FLAG_PVP         = 0x01,
    UNIT_BYTE2_FLAG_UNK1        = 0x02,
    UNIT_BYTE2_FLAG_FFA_PVP     = 0x04,
    UNIT_BYTE2_FLAG_SANCTUARY   = 0x08,
    UNIT_BYTE2_FLAG_UNK4        = 0x10,
    UNIT_BYTE2_FLAG_UNK5        = 0x20,
    UNIT_BYTE2_FLAG_UNK6        = 0x40,
    UNIT_BYTE2_FLAG_UNK7        = 0x80
};

// byte flags value (UNIT_FIELD_BYTES_2,2)
enum UnitRename
{
    UNIT_CAN_BE_RENAMED     = 0x01,
    UNIT_CAN_BE_ABANDONED   = 0x02,
};

#define CREATURE_MAX_SPELLS     10

enum Swing
{
    NOSWING                    = 0,
    SINGLEHANDEDSWING          = 1,
    TWOHANDEDSWING             = 2
};

enum VictimState
{
    VICTIMSTATE_UNAFFECTED     = 0,                         // seen in relation with HITINFO_MISS
    VICTIMSTATE_NORMAL         = 1,
    VICTIMSTATE_DODGE          = 2,
    VICTIMSTATE_PARRY          = 3,
    VICTIMSTATE_INTERRUPT      = 4,
    VICTIMSTATE_BLOCKS         = 5,
    VICTIMSTATE_EVADES         = 6,
    VICTIMSTATE_IS_IMMUNE      = 7,
    VICTIMSTATE_DEFLECTS       = 8
};

enum HitInfo
{
    HITINFO_NORMALSWING         = 0x00000000,
    HITINFO_UNK0                = 0x00000001,               // req correct packet structure
    HITINFO_AFFECTS_VICTIM      = 0x00000002,
    HITINFO_OFFHAND             = 0x00000004,
    HITINFO_UNK3                = 0x00000008,
    HITINFO_MISS                = 0x00000010,
    HITINFO_ABSORB              = 0x00000020,               // absorbed
    HITINFO_PARTIAL_ABSORB      = 0x00000040,               // absorbed at least some damage
    HITINFO_RESIST              = 0x00000080,               // resisted
    HITINFO_PARTIAL_RESIST      = 0x00000100,               // resisted at least some damage
    HITINFO_CRITICALHIT         = 0x00000200,               // critical hit
    // 0x00000400
    // 0x00000800
    // 0x00001000
    HITINFO_BLOCK               = 0x00002000,               // blocked damage
    HITINFO_NODAMAGE            = 0x00004000,               // Hides worldtext for 0 damage
    // 0x00008000
    HITINFO_GLANCING            = 0x00010000,
    HITINFO_CRUSHING            = 0x00020000,
    HITINFO_NOACTION            = 0x00040000,               // guessed
    // 0x00080000
    // 0x00100000
    HITINFO_SWINGNOHITSOUND     = 0x00200000,               // guessed
    // 0x00400000
    HITINFO_RAGE_GAIN           = 0x00800000
};

enum CustomSpellCastFlags
{
    CUSTOM_SPELL_FLAG_NONE                      = 0x00,
    CUSTOM_SPELL_FLAG_NO_COST                   = 0x01,
    CUSTOM_SPELL_FLAG_IGNORE_EQUIPPED_ITEM_REQ  = 0x02,
    CUSTOM_SPELL_FLAG_AURA_DURATION             = 0x04,
};

struct CustomSpellData
{
    CustomSpellData() { Clear(); }

    void SetFlag(uint32 flag) { customCastFlags |= flag; }
    bool HasFlag(uint32 flag) const { return (customCastFlags & flag) != 0; }
    void SetCustomDuration(int32 duration) { SetFlag(CUSTOM_SPELL_FLAG_AURA_DURATION); customDuration = duration; }
    void Clear() { customCastFlags = 0; customDuration = 0; }

    uint32 customCastFlags;
    int32 customDuration;
};

//i would like to remove this: (it is defined in item.h
enum InventorySlot
{
    NULL_BAG                   = 0,
    NULL_SLOT                  = 255
};

struct FactionTemplateEntry;
struct Modifier;
struct SpellEntry;
struct SpellEntryExt;

class Creature;
class Spell;
class DynamicObject;
class GameObject;
class SpellCastTargets;
class Item;
class Pet;
class PetAura;
class Totem;
class VehicleInfo;

typedef int8 SeatId;

struct SpellImmune
{
    uint32 type;
    uint32 spellId;
};

typedef std::list<SpellImmune> SpellImmuneList;

enum UnitModifierType
{
    BASE_VALUE = 0,
    BASE_PCT = 1,
    TOTAL_VALUE = 2,
    TOTAL_PCT = 3,
    NONSTACKING_VALUE_POS = 4,
    NONSTACKING_VALUE_NEG = 5,
    NONSTACKING_PCT = 6,
    NONSTACKING_PCT_MINOR = 7,
    MODIFIER_TYPE_END = 8
};

enum WeaponDamageRange
{
    MINDAMAGE,
    MAXDAMAGE
};

enum DamageTypeToSchool
{
    RESISTANCE,
    DAMAGE_DEALT,
    DAMAGE_TAKEN
};

enum UnitMods
{
    UNIT_MOD_STAT_STRENGTH,                                 // UNIT_MOD_STAT_STRENGTH..UNIT_MOD_STAT_SPIRIT must be in existing order, it's accessed by index values of Stats enum.
    UNIT_MOD_STAT_AGILITY,
    UNIT_MOD_STAT_STAMINA,
    UNIT_MOD_STAT_INTELLECT,
    UNIT_MOD_STAT_SPIRIT,
    UNIT_MOD_HEALTH,
    UNIT_MOD_MANA,                                          // UNIT_MOD_MANA..UNIT_MOD_ALTERNATIVE must be in existing order, it's accessed by index values of Powers enum.
    UNIT_MOD_RAGE,
    UNIT_MOD_FOCUS,
    UNIT_MOD_ENERGY,
    UNIT_MOD_HAPPINESS,
    UNIT_MOD_RUNE,
    UNIT_MOD_RUNIC_POWER,
    UNIT_MOD_SOUL_SHARDS,
    UNIT_MOD_ECLIPSE,
    UNIT_MOD_HOLY_POWER,
    UNIT_MOD_ALTERNATIVE,
    UNIT_MOD_ARMOR,                                         // UNIT_MOD_ARMOR..UNIT_MOD_RESISTANCE_ARCANE must be in existing order, it's accessed by index values of SpellSchools enum.
    UNIT_MOD_RESISTANCE_HOLY,
    UNIT_MOD_RESISTANCE_FIRE,
    UNIT_MOD_RESISTANCE_NATURE,
    UNIT_MOD_RESISTANCE_FROST,
    UNIT_MOD_RESISTANCE_SHADOW,
    UNIT_MOD_RESISTANCE_ARCANE,
    UNIT_MOD_ATTACK_POWER,
    UNIT_MOD_ATTACK_POWER_RANGED,
    UNIT_MOD_DAMAGE_MAINHAND,
    UNIT_MOD_DAMAGE_OFFHAND,
    UNIT_MOD_DAMAGE_RANGED,
    UNIT_MOD_END,
    // synonyms
    UNIT_MOD_STAT_START = UNIT_MOD_STAT_STRENGTH,
    UNIT_MOD_STAT_END = UNIT_MOD_STAT_SPIRIT + 1,
    UNIT_MOD_RESISTANCE_START = UNIT_MOD_ARMOR,
    UNIT_MOD_RESISTANCE_END = UNIT_MOD_RESISTANCE_ARCANE + 1,
    UNIT_MOD_POWER_START = UNIT_MOD_MANA,
    UNIT_MOD_POWER_END = UNIT_MOD_ALTERNATIVE + 1
};

static_assert(UNIT_MOD_POWER_END - UNIT_MOD_POWER_START == MAX_POWERS, "Power-related UnitMods are not updated.");

enum BaseModGroup
{
    CRIT_PERCENTAGE,
    RANGED_CRIT_PERCENTAGE,
    OFFHAND_CRIT_PERCENTAGE,
    SHIELD_BLOCK_DAMAGE_VALUE,
    NONSTACKING_CRIT_PERCENTAGE,
    BASEMOD_END
};

enum BaseModType
{
    FLAT_MOD,
    PCT_MOD
};

#define MOD_END (PCT_MOD+1)

enum DeathState
{
    ALIVE          = 0,                                     // show as alive
    JUST_DIED      = 1,                                     // temporary state at die, for creature auto converted to CORPSE, for player at next update call
    CORPSE         = 2,                                     // corpse state, for player this also meaning that player not leave corpse
    DEAD           = 3,                                     // for creature despawned state (corpse despawned), for player CORPSE/DEAD not clear way switches (FIXME), and use m_deathtimer > 0 check for real corpse state
    JUST_ALIVED    = 4,                                     // temporary state at resurrection, for creature auto converted to ALIVE, for player at next update call
    GHOULED        = 5
};

// internal state flags for some auras and movement generators, other.
enum UnitState
{
    // persistent state (applied by aura/etc until expire)
    UNIT_STAT_MELEE_ATTACKING = 0x00000001,                     // unit is melee attacking someone Unit::Attack
    UNIT_STAT_ATTACK_PLAYER   = 0x00000002,                     // unit attack player or player's controlled unit and have contested pvpv timer setup, until timer expire, combat end and etc
    UNIT_STAT_DIED            = 0x00000004,                     // Unit::SetFeignDeath
    UNIT_STAT_STUNNED         = 0x00000008,                     // Aura::HandleAuraModStun
    UNIT_STAT_ROOT            = 0x00000010,                     // Aura::HandleAuraModRoot
    UNIT_STAT_ISOLATED        = 0x00000020,                     // area auras do not affect other players, Aura::HandleAuraModSchoolImmunity
    UNIT_STAT_CONTROLLED      = 0x00000040,                     // Aura::HandleAuraModPossess

    // persistent movement generator state (all time while movement generator applied to unit (independent from top state of movegen)
    UNIT_STAT_TAXI_FLIGHT     = 0x00000080,                     // player is in flight mode (in fact interrupted at far teleport until next map telport landing)
    UNIT_STAT_DISTRACTED      = 0x00000100,                     // DistractedMovementGenerator active

    // persistent movement generator state with non-persistent mirror states for stop support
    // (can be removed temporary by stop command or another movement generator apply)
    // not use _MOVE versions for generic movegen state, it can be removed temporary for unit stop and etc
    UNIT_STAT_CONFUSED        = 0x00000200,                     // ConfusedMovementGenerator active/onstack
    UNIT_STAT_CONFUSED_MOVE   = 0x00000400,
    UNIT_STAT_ROAMING         = 0x00000800,                     // RandomMovementGenerator/PointMovementGenerator/WaypointMovementGenerator active (now always set)
    UNIT_STAT_ROAMING_MOVE    = 0x00001000,
    UNIT_STAT_CHASE           = 0x00002000,                     // ChaseMovementGenerator active
    UNIT_STAT_CHASE_MOVE      = 0x00004000,
    UNIT_STAT_FOLLOW          = 0x00008000,                     // FollowMovementGenerator active
    UNIT_STAT_FOLLOW_MOVE     = 0x00010000,
    UNIT_STAT_FLEEING         = 0x00020000,                     // FleeMovementGenerator/TimedFleeingMovementGenerator active/onstack
    UNIT_STAT_FLEEING_MOVE    = 0x00040000,

    // custom MMGen (may be removed)
    UNIT_STAT_ON_VEHICLE      = 0x00080000,                     // Unit is on vehicle

    // More room for other MMGens

    // High-Level states (usually only with Creatures)
    UNIT_STAT_NO_COMBAT_MOVEMENT    = 0x01000000,           // Combat Movement for MoveChase stopped
    UNIT_STAT_RUNNING               = 0x02000000,           // SetRun for waypoints and such
    UNIT_STAT_WAYPOINT_PAUSED       = 0x04000000,           // Waypoint-Movement paused genericly (ie by script)
    UNIT_STAT_DELAYED_EVADE         = 0x08000000,           // Creature in delayed evade event
    UNIT_STAT_IGNORE_PATHFINDING    = 0x10000000,           // do not use pathfinding in any MovementGenerator

    // masks (only for check)

    // can't move currently
    UNIT_STAT_CAN_NOT_MOVE    = UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DIED | UNIT_STAT_ON_VEHICLE,

    // stay by different reasons
    UNIT_STAT_NOT_MOVE        = UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DIED |
                                UNIT_STAT_DISTRACTED | UNIT_STAT_ON_VEHICLE,

    // stay or scripted movement for effect( = in player case you can't move by client command)
    UNIT_STAT_NO_FREE_MOVE    = UNIT_STAT_ROOT | UNIT_STAT_STUNNED | UNIT_STAT_DIED |
                                UNIT_STAT_TAXI_FLIGHT |
                                UNIT_STAT_CONFUSED | UNIT_STAT_FLEEING | UNIT_STAT_ON_VEHICLE,

    // not react at move in sight or other
    UNIT_STAT_CAN_NOT_REACT   = UNIT_STAT_STUNNED | UNIT_STAT_DIED |
                                UNIT_STAT_CONFUSED | UNIT_STAT_FLEEING | UNIT_STAT_ON_VEHICLE,

    // AI disabled by some reason
    UNIT_STAT_LOST_CONTROL    = UNIT_STAT_FLEEING | UNIT_STAT_CONTROLLED,

    // above 2 state cases
    UNIT_STAT_CAN_NOT_REACT_OR_LOST_CONTROL  = UNIT_STAT_CAN_NOT_REACT | UNIT_STAT_LOST_CONTROL,

    // masks (for check or reset)

    // for real move using movegen check and stop (except unstoppable flight)
    UNIT_STAT_MOVING          = UNIT_STAT_CONFUSED_MOVE | UNIT_STAT_ROAMING_MOVE | UNIT_STAT_CHASE_MOVE | UNIT_STAT_FOLLOW_MOVE | UNIT_STAT_FLEEING_MOVE,

    UNIT_STAT_RUNNING_STATE   = UNIT_STAT_CHASE_MOVE | UNIT_STAT_FLEEING_MOVE | UNIT_STAT_RUNNING,

    UNIT_STAT_ALL_STATE       = 0xFFFFFFFF,
    UNIT_STAT_ALL_DYN_STATES  = UNIT_STAT_ALL_STATE & ~(UNIT_STAT_NO_COMBAT_MOVEMENT | UNIT_STAT_RUNNING | UNIT_STAT_WAYPOINT_PAUSED | UNIT_STAT_IGNORE_PATHFINDING),
};

enum UnitMoveType
{
    MOVE_WALK           = 0,
    MOVE_RUN            = 1,
    MOVE_RUN_BACK       = 2,
    MOVE_SWIM           = 3,
    MOVE_SWIM_BACK      = 4,
    MOVE_TURN_RATE      = 5,
    MOVE_FLIGHT         = 6,
    MOVE_FLIGHT_BACK    = 7,
    MOVE_PITCH_RATE     = 8
};

#define MAX_MOVE_TYPE     9

#define BASE_CHARGE_SPEED 27.0f

enum CombatRating
{
    CR_WEAPON_SKILL             = 0,
    CR_DEFENSE_SKILL            = 1,                        // obsolete
    CR_DODGE                    = 2,
    CR_PARRY                    = 3,
    CR_BLOCK                    = 4,
    CR_HIT_MELEE                = 5,
    CR_HIT_RANGED               = 6,
    CR_HIT_SPELL                = 7,
    CR_CRIT_MELEE               = 8,
    CR_CRIT_RANGED              = 9,
    CR_CRIT_SPELL               = 10,
    CR_HIT_TAKEN_MELEE          = 11,                       // obsolete
    CR_HIT_TAKEN_RANGED         = 12,                       // obsolete
    CR_HIT_TAKEN_SPELL          = 13,                       // obsolete
    CR_CRIT_TAKEN_MELEE         = 14,                       // COMBAT_RATING_RESILIENCE_CRIT_TAKEN obsolete
    CR_RESILIENCE_DAMAGE_TAKEN  = 15,                       // old CR_CRIT_TAKEN_RANGED
    CR_CRIT_TAKEN_SPELL         = 16,                       // obsolete
    CR_HASTE_MELEE              = 17,
    CR_HASTE_RANGED             = 18,
    CR_HASTE_SPELL              = 19,
    CR_WEAPON_SKILL_MAINHAND    = 20,                       // obsolete
    CR_WEAPON_SKILL_OFFHAND     = 21,                       // obsolete
    CR_WEAPON_SKILL_RANGED      = 22,                       // obsolete
    CR_EXPERTISE                = 23,
    CR_ARMOR_PENETRATION        = 24,
    CR_MASTERY                  = 25
};

#define MAX_COMBAT_RATING         26

/// internal used flags for marking special auras - for example some dummy-auras
enum UnitAuraFlags
{
    UNIT_AURAFLAG_ALIVE_INVISIBLE   = 0x1,                  // aura which makes unit invisible for alive
};

enum UnitVisibility
{
    VISIBILITY_OFF                = 0,                      // absolute, not detectable, GM-like, can see all other
    VISIBILITY_ON                 = 1,
    VISIBILITY_GROUP_STEALTH      = 2,                      // detect chance, seen and can see group members
    VISIBILITY_GROUP_INVISIBILITY = 3,                      // invisibility, can see and can be seen only another invisible unit or invisible detection unit, set only if not stealthed, and in checks not used (mask used instead)
    VISIBILITY_GROUP_NO_DETECT    = 4,                      // state just at stealth apply for update Grid state. Don't remove, otherwise stealth spells will break
    VISIBILITY_REMOVE_CORPSE      = 5                       // special totally not detectable visibility for force delete object while removing a corpse
};

// Value masks for UNIT_FIELD_FLAGS
enum UnitFlags
{
    UNIT_FLAG_UNK_0                 = 0x00000001,
    UNIT_FLAG_NON_ATTACKABLE        = 0x00000002,           // not attackable
    UNIT_FLAG_DISABLE_MOVE          = 0x00000004,
    UNIT_FLAG_PVP_ATTACKABLE        = 0x00000008,           // allow apply pvp rules to attackable state in addition to faction dependent state
    UNIT_FLAG_RENAME                = 0x00000010,
    UNIT_FLAG_PREPARATION           = 0x00000020,           // don't take reagents for spells with SPELL_ATTR_EX5_NO_REAGENT_WHILE_PREP
    UNIT_FLAG_UNK_6                 = 0x00000040,
    UNIT_FLAG_NOT_ATTACKABLE_1      = 0x00000080,           // ?? (UNIT_FLAG_PVP_ATTACKABLE | UNIT_FLAG_NOT_ATTACKABLE_1) is NON_PVP_ATTACKABLE
    UNIT_FLAG_OOC_NOT_ATTACKABLE    = 0x00000100,           // 2.0.8 - (OOC Out Of Combat) Can not be attacked when not in combat. Removed if unit for some reason enter combat (flag probably removed for the attacked and it's party/group only)
    UNIT_FLAG_PASSIVE               = 0x00000200,           // makes you unable to attack everything. Almost identical to our "civilian"-term. Will ignore it's surroundings and not engage in combat unless "called upon" or engaged by another unit.
    UNIT_FLAG_LOOTING               = 0x00000400,           // loot animation
    UNIT_FLAG_PET_IN_COMBAT         = 0x00000800,           // in combat?, 2.0.8
    UNIT_FLAG_PVP                   = 0x00001000,           // changed in 3.0.3
    UNIT_FLAG_SILENCED              = 0x00002000,           // silenced, 2.1.1
    UNIT_FLAG_UNK_14                = 0x00004000,           // 2.0.8
    UNIT_FLAG_UNK_15                = 0x00008000,
    UNIT_FLAG_UNK_16                = 0x00010000,           // removes attackable icon
    UNIT_FLAG_PACIFIED              = 0x00020000,           // 3.0.3 ok
    UNIT_FLAG_STUNNED               = 0x00040000,           // 3.0.3 ok
    UNIT_FLAG_IN_COMBAT             = 0x00080000,
    UNIT_FLAG_TAXI_FLIGHT           = 0x00100000,           // disable casting at client side spell not allowed by taxi flight (mounted?), probably used with 0x4 flag
    UNIT_FLAG_DISARMED              = 0x00200000,           // 3.0.3, disable melee spells casting..., "Required melee weapon" added to melee spells tooltip.
    UNIT_FLAG_CONFUSED              = 0x00400000,
    UNIT_FLAG_FLEEING               = 0x00800000,
    UNIT_FLAG_PLAYER_CONTROLLED     = 0x01000000,           // used in spell Eyes of the Beast for pet... let attack by controlled creature
    UNIT_FLAG_NOT_SELECTABLE        = 0x02000000,
    UNIT_FLAG_SKINNABLE             = 0x04000000,
    UNIT_FLAG_MOUNT                 = 0x08000000,
    UNIT_FLAG_UNK_28                = 0x10000000,
    UNIT_FLAG_UNK_29                = 0x20000000,           // used in Feing Death spell
    UNIT_FLAG_SHEATHE               = 0x40000000,
    UNIT_FLAG_UNK_31                = 0x80000000            // set skinnable icon and also changes color of portrait
};

// Value masks for UNIT_FIELD_FLAGS_2
enum UnitFlags2
{
    UNIT_FLAG2_FEIGN_DEATH          = 0x00000001,
    UNIT_FLAG2_UNK1                 = 0x00000002,           // Hides body and body armor. Weapons and shoulder and head armor still visible
    UNIT_FLAG2_UNK2                 = 0x00000004,
    UNIT_FLAG2_COMPREHEND_LANG      = 0x00000008,
    UNIT_FLAG2_CLONED               = 0x00000010,           // Used in SPELL_AURA_MIRROR_IMAGE
    UNIT_FLAG2_UNK5                 = 0x00000020,
    UNIT_FLAG2_FORCE_MOVE           = 0x00000040,
    UNIT_FLAG2_DISARM_OFFHAND       = 0x00000080,           // also shield case
    UNIT_FLAG2_UNK8                 = 0x00000100,
    UNIT_FLAG2_UNK9                 = 0x00000200,
    UNIT_FLAG2_DISARM_RANGED        = 0x00000400,           // disarm or something
    UNIT_FLAG2_REGENERATE_POWER     = 0x00000800,
    UNIT_FLAG2_GM_MODE              = 0x00040000,           // allows casting spells with AttributesEx7 & SPELL_ATTR_EX7_GM_ONLY
    UNIT_FLAG2_WORGEN_TRANSFORM     = 0x00080000,           // transform to worgen
    UNIT_FLAG2_WORGEN_TRANSFORM2    = 0x00100000,           // transform to worgen, but less animation?
    UNIT_FLAG2_WORGEN_TRANSFORM3    = 0x00200000            // transform to worgen, but less animation?
};

/// Non Player Character flags
enum NPCFlags
{
    UNIT_NPC_FLAG_NONE                  = 0x00000000,
    UNIT_NPC_FLAG_GOSSIP                = 0x00000001,       // 100%
    UNIT_NPC_FLAG_QUESTGIVER            = 0x00000002,       // guessed, probably ok
    UNIT_NPC_FLAG_UNK1                  = 0x00000004,
    UNIT_NPC_FLAG_UNK2                  = 0x00000008,
    UNIT_NPC_FLAG_TRAINER               = 0x00000010,       // 100%
    UNIT_NPC_FLAG_TRAINER_CLASS         = 0x00000020,       // 100%
    UNIT_NPC_FLAG_TRAINER_PROFESSION    = 0x00000040,       // 100%
    UNIT_NPC_FLAG_VENDOR                = 0x00000080,       // 100%
    UNIT_NPC_FLAG_VENDOR_AMMO           = 0x00000100,       // 100%, general goods vendor
    UNIT_NPC_FLAG_VENDOR_FOOD           = 0x00000200,       // 100%
    UNIT_NPC_FLAG_VENDOR_POISON         = 0x00000400,       // guessed
    UNIT_NPC_FLAG_VENDOR_REAGENT        = 0x00000800,       // 100%
    UNIT_NPC_FLAG_REPAIR                = 0x00001000,       // 100%
    UNIT_NPC_FLAG_FLIGHTMASTER          = 0x00002000,       // 100%
    UNIT_NPC_FLAG_SPIRITHEALER          = 0x00004000,       // guessed
    UNIT_NPC_FLAG_SPIRITGUIDE           = 0x00008000,       // guessed
    UNIT_NPC_FLAG_INNKEEPER             = 0x00010000,       // 100%
    UNIT_NPC_FLAG_BANKER                = 0x00020000,       // 100%
    UNIT_NPC_FLAG_PETITIONER            = 0x00040000,       // 100% 0xC0000 = guild petitions, 0x40000 = arena team petitions
    UNIT_NPC_FLAG_TABARDDESIGNER        = 0x00080000,       // 100%
    UNIT_NPC_FLAG_BATTLEMASTER          = 0x00100000,       // 100%
    UNIT_NPC_FLAG_AUCTIONEER            = 0x00200000,       // 100%
    UNIT_NPC_FLAG_STABLEMASTER          = 0x00400000,       // 100%
    UNIT_NPC_FLAG_GUILD_BANKER          = 0x00800000,       // cause client to send 997 opcode
    UNIT_NPC_FLAG_SPELLCLICK            = 0x01000000,       // cause client to send 1015 opcode (spell click), dynamic, set at loading and don't must be set in DB
    UNIT_NPC_FLAG_PLAYER_VEHICLE        = 0x02000000,       // players with mounts that have vehicle data should have it set
    UNIT_NPC_FLAG_MAILBOX               = 0x04000000,       //
    UNIT_NPC_FLAG_REFORGER              = 0x08000000,       // reforging
    UNIT_NPC_FLAG_TRANSMOGRIFIER        = 0x10000000,       // transmogrification
    UNIT_NPC_FLAG_VAULTKEEPER           = 0x20000000,       // void storage
};

// used in most movement packets (send and received), 30 bits in client
enum MovementFlags
{
    MOVEFLAG_NONE               = 0x00000000,
    MOVEFLAG_FORWARD            = 0x00000001,
    MOVEFLAG_BACKWARD           = 0x00000002,
    MOVEFLAG_STRAFE_LEFT        = 0x00000004,
    MOVEFLAG_STRAFE_RIGHT       = 0x00000008,
    MOVEFLAG_TURN_LEFT          = 0x00000010,
    MOVEFLAG_TURN_RIGHT         = 0x00000020,
    MOVEFLAG_PITCH_UP           = 0x00000040,
    MOVEFLAG_PITCH_DOWN         = 0x00000080,
    MOVEFLAG_WALK_MODE          = 0x00000100,               // Walking
    MOVEFLAG_LEVITATING         = 0x00000200,
    MOVEFLAG_ROOT               = 0x00000400,
    MOVEFLAG_FALLING            = 0x00000800,               // jumping
    MOVEFLAG_FALLINGFAR         = 0x00001000,               // falling through air
    MOVEFLAG_PENDINGSTOP        = 0x00002000,
    MOVEFLAG_PENDINGSTRAFESTOP  = 0x00004000,
    MOVEFLAG_PENDINGFORWARD     = 0x00008000,
    MOVEFLAG_PENDINGBACKWARD    = 0x00010000,
    MOVEFLAG_PENDINGSTRAFELEFT  = 0x00020000,
    MOVEFLAG_PENDINGSTRAFERIGHT = 0x00040000,
    MOVEFLAG_PENDINGROOT        = 0x00080000,
    MOVEFLAG_SWIMMING           = 0x00100000,               // appears with fly flag also
    MOVEFLAG_ASCENDING          = 0x00200000,               // swim up also
    MOVEFLAG_DESCENDING         = 0x00400000,               // swim down also
    MOVEFLAG_CAN_FLY            = 0x00800000,               // can fly in 3.3?
    MOVEFLAG_FLYING             = 0x01000000,               // Actual flying mode
    MOVEFLAG_SPLINE_ELEVATION   = 0x02000000,               // used for flight paths
    MOVEFLAG_WATERWALKING       = 0x04000000,               // prevent unit from falling through water
    MOVEFLAG_SAFE_FALL          = 0x08000000,               // active rogue safe fall spell (passive)
    MOVEFLAG_HOVER              = 0x10000000,
    MOVEFLAG_LOCAL_DIRTY        = 0x20000000,

    MOVEFLAG_MASK_MOVING =
        MOVEFLAG_FORWARD | MOVEFLAG_BACKWARD | MOVEFLAG_STRAFE_LEFT | MOVEFLAG_STRAFE_RIGHT |
        MOVEFLAG_PITCH_UP | MOVEFLAG_PITCH_DOWN | MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR | MOVEFLAG_ASCENDING | MOVEFLAG_DESCENDING |
        MOVEFLAG_SPLINE_ELEVATION,

    MOVEFLAG_MASK_TURNING =
        MOVEFLAG_TURN_LEFT | MOVEFLAG_TURN_RIGHT,

    MOVEFLAG_MASK_CREATURE_ALLOWED =
        MOVEFLAG_FORWARD | MOVEFLAG_LEVITATING | MOVEFLAG_ROOT | MOVEFLAG_SWIMMING |
        MOVEFLAG_CAN_FLY | MOVEFLAG_WATERWALKING | MOVEFLAG_SAFE_FALL | MOVEFLAG_HOVER,
};

// flags that use in movement check for example at spell casting
MovementFlags const movementFlagsMask = MovementFlags(
    MOVEFLAG_FORWARD |MOVEFLAG_BACKWARD  |MOVEFLAG_STRAFE_LEFT |MOVEFLAG_STRAFE_RIGHT|
    MOVEFLAG_PITCH_UP|MOVEFLAG_PITCH_DOWN|MOVEFLAG_ROOT        |
    MOVEFLAG_FALLING |MOVEFLAG_FALLINGFAR|MOVEFLAG_ASCENDING   |
    MOVEFLAG_FLYING  |MOVEFLAG_SPLINE_ELEVATION
);

MovementFlags const movementOrTurningFlagsMask = MovementFlags(
    movementFlagsMask | MOVEFLAG_TURN_LEFT | MOVEFLAG_TURN_RIGHT
);

// 12 bits in client
enum MovementFlags2
{
    MOVEFLAG2_NONE              = 0x0000,
    MOVEFLAG2_NO_STRAFE         = 0x0001,
    MOVEFLAG2_NO_JUMPING        = 0x0002,
    MOVEFLAG2_FULLSPEEDTURNING  = 0x0004,
    MOVEFLAG2_FULLSPEEDPITCHING = 0x0008,
    MOVEFLAG2_ALLOW_PITCHING    = 0x0010,
    MOVEFLAG2_UNK4              = 0x0020,
    MOVEFLAG2_UNK5              = 0x0040,
    MOVEFLAG2_UNK6              = 0x0080,                   // transport related
    MOVEFLAG2_UNK7              = 0x0100,
    MOVEFLAG2_INTERP_MOVEMENT   = 0x0200,
    MOVEFLAG2_INTERP_TURNING    = 0x0400,
    MOVEFLAG2_INTERP_PITCHING   = 0x0800,
    MOVEFLAG2_INTERP_MASK       = MOVEFLAG2_INTERP_MOVEMENT | MOVEFLAG2_INTERP_TURNING | MOVEFLAG2_INTERP_PITCHING
};

enum SplineFlags
{
    SPLINEFLAG_NONE         = 0x00000000,
    SPLINEFLAG_FORWARD      = 0x00000001,
    SPLINEFLAG_BACKWARD     = 0x00000002,
    SPLINEFLAG_STRAFE_LEFT  = 0x00000004,
    SPLINEFLAG_STRAFE_RIGHT = 0x00000008,
    SPLINEFLAG_LEFT         = 0x00000010,
    SPLINEFLAG_RIGHT        = 0x00000020,
    SPLINEFLAG_PITCH_UP     = 0x00000040,
    SPLINEFLAG_PITCH_DOWN   = 0x00000080,
    SPLINEFLAG_DONE         = 0x00000100,
    SPLINEFLAG_FALLING      = 0x00000200,
    SPLINEFLAG_NO_SPLINE    = 0x00000400,
    SPLINEFLAG_TRAJECTORY   = 0x00000800,
    SPLINEFLAG_WALKMODE     = 0x00001000,
    SPLINEFLAG_FLYING       = 0x00002000,
    SPLINEFLAG_KNOCKBACK    = 0x00004000,
    SPLINEFLAG_FINALPOINT   = 0x00008000,
    SPLINEFLAG_FINALTARGET  = 0x00010000,
    SPLINEFLAG_FINALFACING  = 0x00020000,
    SPLINEFLAG_CATMULLROM   = 0x00040000,
    SPLINEFLAG_CYCLIC       = 0x00080000,
    SPLINEFLAG_ENTER_CYCLE  = 0x00100000,
    SPLINEFLAG_ANIMATION    = 0x00200000,
    SPLINEFLAG_FROZEN       = 0x00400000,
    SPLINEFLAG_TRANSPORT    = 0x00800000,
    SPLINEFLAG_TRANSPORT_EXIT = 0x01000000,
    SPLINEFLAG_UNKNOWN7     = 0x02000000,
    SPLINEFLAG_UNKNOWN8     = 0x04000000,
    SPLINEFLAG_ORIENTATION_INVERCED = 0x08000000,
    SPLINEFLAG_UNKNOWN10    = 0x10000000,
    SPLINEFLAG_UNKNOWN11    = 0x20000000,
    SPLINEFLAG_UNKNOWN12    = 0x40000000
};

enum SplineMode
{
    SPLINEMODE_LINEAR       = 0,
    SPLINEMODE_CATMULLROM   = 1,
    SPLINEMODE_BEZIER3      = 2
};

enum SplineType
{
    SPLINETYPE_NORMAL       = 0,
    SPLINETYPE_STOP         = 1,
    SPLINETYPE_FACINGSPOT   = 2,
    SPLINETYPE_FACINGTARGET = 3,
    SPLINETYPE_FACINGANGLE  = 4
};

class MovementInfo
{
    friend class AntiCheat;

    public:
        MovementInfo() : moveFlags(MOVEFLAG_NONE), moveFlags2(MOVEFLAG2_NONE), time(0),
            t_time(0), t_seat(-1), t_seatInfo(NULL), t_time2(0), s_pitch(0.0f), fallTime(0), splineElevation(0.0f), byteParam(0) {}

        // Read/Write methods
        void Read(ByteBuffer& data, uint16 opcode);
        void Write(ByteBuffer& data, uint16 opcode) const;

        // Movement flags manipulations
        void AddMovementFlag(MovementFlags f) { moveFlags |= f; }
        void RemoveMovementFlag(MovementFlags f) { moveFlags &= ~f; }
        bool HasMovementFlag(MovementFlags f) const { return moveFlags & f; }
        bool HasMovementFlag2(MovementFlags2 f) const { return moveFlags2 & f; }
        MovementFlags GetMovementFlags() const { return MovementFlags(moveFlags); }
        void SetMovementFlags(MovementFlags f) { moveFlags = f; }
        MovementFlags2 GetMovementFlags2() const { return MovementFlags2(moveFlags2); }
        void AddMovementFlag2(MovementFlags2 f) { moveFlags2 |= f; }

        // Position manipulations
        Position const* GetPos() const { return &pos; }
        void SetTransportData(ObjectGuid guid, Position const& pos, uint32 time, SeatId seat, VehicleSeatEntry const* seatInfo = NULL)
        {
            t_guid = guid;
            t_pos = pos;
            t_time = time;
            t_seat = seat;
            t_seatInfo = seatInfo;
        }
        void ClearTransportData()
        {
            moveFlags2 = MOVEFLAG2_NONE;
            t_guid = ObjectGuid();
            t_pos.x = 0.0f;
            t_pos.y = 0.0f;
            t_pos.z = 0.0f;
            t_pos.o = 0.0f;
            t_time = 0;
            t_seat = -1;
            t_seatInfo = NULL;
        }
        ObjectGuid const& GetGuid() const { return guid; }
        ObjectGuid const& GetGuid2() const { return guid2; }
        ObjectGuid const& GetTransportGuid() const { return t_guid; }
        Position const* GetTransportPos() const { return &t_pos; }
        SeatId GetTransportSeat() const { return t_seat; }

        uint32 GetTransportDBCSeat() const { return t_seatInfo ? t_seatInfo->m_ID : 0; }
        uint32 GetVehicleSeatFlags() const { return t_seatInfo ? t_seatInfo->m_flags : 0; }

        uint32 GetTime() const { return time; }
        uint32 GetTransportTime() const { return t_time; }
        uint32 GetTransportTime2() const { return t_time2; }
        uint32 GetFallTime() const { return fallTime; }
        int8 GetByteParam() const { return byteParam; }

        void ChangeOrientation(float o) { pos.o = o; }
        void ChangePosition(float x, float y, float z, float o) { pos.x = x; pos.y = y; pos.z = z; pos.o = o; }
        void ChangeTransportPosition(float x, float y, float z, float o) { t_pos.x = x; t_pos.y = y; t_pos.z = z; t_pos.o = o; }

        void ChangePosition(Position const& _pos) { pos = _pos; }
        void ChangeTransportPosition(Position const& _pos) { t_pos = _pos; }
        Position const& GetPosition() const { return pos; }
        Position const& GetTransportPosition() const { return t_pos; }

        void UpdateTime(uint32 _time) { time = _time; }

        struct JumpInfo
        {
            JumpInfo() : velocity(0.f), sinAngle(0.f), cosAngle(0.f), xyspeed(0.f) {}
            float velocity, sinAngle, cosAngle, xyspeed;
            void Clear() { velocity = sinAngle = cosAngle = xyspeed = 0.0f; }
        };

        // used only for SMSG_PLAYER_MOVE currently
        struct StatusInfo
        {
            StatusInfo() : hasFallData(false), hasFallDirection(false), hasOrientation(false),
                hasPitch(false), hasSpline(false), hasSplineElevation(false),
                hasTimeStamp(false), hasTransportTime2(false), hasTransportTime3(false) { }
            bool hasFallData        : 1;
            bool hasFallDirection   : 1;
            bool hasOrientation     : 1;
            bool hasPitch           : 1;
            bool hasSpline          : 1;
            bool hasSplineElevation : 1;
            bool hasTimeStamp       : 1;
            bool hasTransportTime2  : 1;
            bool hasTransportTime3  : 1;
        };

        JumpInfo const& GetJumpInfo() const { return jump; }
        JumpInfo& GetJumpInfo() { return jump; }

        MovementInfo& operator = (const MovementInfo& targetInfo)
        {
            uint32 moveFlagsTmp = targetInfo.moveFlags;

            moveFlags  = moveFlagsTmp;
            splineElevation = targetInfo.splineElevation;
            time       = targetInfo.time;
            pos        = targetInfo.pos;
            s_pitch    = targetInfo.s_pitch;
            fallTime   = targetInfo.fallTime;
            jump       = targetInfo.jump;

            if (!t_guid || (t_guid && t_guid == targetInfo.t_guid))
            {
                moveFlags2 = targetInfo.moveFlags2;
                t_guid     = targetInfo.t_guid;
                t_pos      = targetInfo.t_pos;
                t_time     = targetInfo.t_time;
                t_seat     = targetInfo.t_seat;
                t_seatInfo = targetInfo.t_seatInfo;
                t_time2    = targetInfo.t_time2;
            }
            return *this;
        }

        StatusInfo const& GetStatusInfo() const { return si; }
        StatusInfo& GetStatusInfo() { return si; }
        float GetSplineElevation() const { return splineElevation; }
        float GetPitch() const { return s_pitch; }

    private:
        // common
        ObjectGuid guid;
        ObjectGuid guid2;
        uint32   moveFlags;                                 // see enum MovementFlags
        uint16   moveFlags2;                                // see enum MovementFlags2
        uint32   time;
        Position pos;
        // transport
        ObjectGuid t_guid;
        Position t_pos;
        uint32   t_time;
        SeatId     t_seat;
        VehicleSeatEntry const* t_seatInfo;
        uint32   t_time2;
        // swimming and flying
        float    s_pitch;
        // last fall time
        uint32   fallTime;
        // jumping
        JumpInfo jump;
        // spline
        float    splineElevation;
        // status info
        StatusInfo si;
        int8 byteParam;
};

inline WorldPacket& operator<< (WorldPacket& buf, MovementInfo const& mi)
{
    mi.Write(buf, buf.GetOpcode());
    return buf;
}

inline WorldPacket& operator>> (WorldPacket& buf, MovementInfo& mi)
{
    mi.Read(buf, buf.GetOpcode());
    return buf;
}

enum DiminishingLevels
{
    DIMINISHING_LEVEL_1             = 0,
    DIMINISHING_LEVEL_2             = 1,
    DIMINISHING_LEVEL_3             = 2,
    DIMINISHING_LEVEL_4             = 3,
    DIMINISHING_LEVEL_5             = 4,
    DIMINISHING_LEVEL_IMMUNE        = 5
};

struct DiminishingReturn
{
    DiminishingReturn(DiminishingGroup group, uint32 t, uint32 count)
        : DRGroup(group), stack(0), hitTime(t), hitCount(count)
    {}

    DiminishingGroup        DRGroup:16;
    uint16                  stack:16;
    uint32                  hitTime;
    uint32                  hitCount;
};

// At least some values expected fixed and used in auras field, other custom
enum MeleeHitOutcome
{
    MELEE_HIT_EVADE     = 0,
    MELEE_HIT_MISS      = 1,
    MELEE_HIT_DODGE     = 2,                                // used as misc in SPELL_AURA_IGNORE_COMBAT_RESULT
    MELEE_HIT_BLOCK     = 3,                                // used as misc in SPELL_AURA_IGNORE_COMBAT_RESULT
    MELEE_HIT_PARRY     = 4,                                // used as misc in SPELL_AURA_IGNORE_COMBAT_RESULT
    MELEE_HIT_GLANCING  = 5,
    MELEE_HIT_CRIT      = 6,
    MELEE_HIT_CRUSHING  = 7,
    MELEE_HIT_NORMAL    = 8,
};

enum DamageFlags
{
    DAMAGE_FREEACTION   = 0,
    DAMAGE_SHARED       = 1,
};

//struct CleanDamage
//struct CalcDamageInfo
//struct SpellNonMeleeDamage

// Struct for use in Unit::CalculateMeleeDamage
// Spell damage info structure based on structure sending in SMSG_SPELLNONMELEEDAMAGELOG opcode
struct DamageInfo
{
    public:
        // Constructors for use with spell and melee damage
        DamageInfo(Unit *_attacker, Unit *_target, uint32 _SpellID, uint32 _damage)
            :  attacker(_attacker), target(_target), m_spellInfo(NULL), SpellID(_SpellID)
        { Reset(_damage); };

        DamageInfo(Unit *_attacker, Unit *_target, SpellEntry const* spellInfo, uint32 _damage = 0)
            :  attacker(_attacker), target(_target), m_spellInfo(spellInfo), SpellID(0)
        { Reset(_damage); };

        // Constructors for use on temporary operation
        DamageInfo(uint32 _damage)
            : attacker(NULL), target(NULL), m_spellInfo(NULL), SpellID(0)
        { Reset(_damage); };

        DamageInfo(uint32 _damage, uint32 _SpellID)
            : attacker(NULL), target(NULL), m_spellInfo(NULL), SpellID(_SpellID)
        { Reset(_damage); };

        DamageInfo(uint32 _damage, SpellEntry const* spellInfo)
            : attacker(NULL), target(NULL), m_spellInfo(spellInfo), SpellID(0)
        { Reset(_damage); };

        // main operations
        void Reset(uint32 _damage = 0);

        // compartibility methods
        void CleanDamage(int32 _signedDamage, uint32 _absorb, WeaponAttackType _attackType, MeleeHitOutcome _hitOutCome)
        {
            cleanDamage = _signedDamage;
            absorb      = _absorb;
            attackType  = _attackType;
            hitOutCome  = _hitOutCome;
        }

        Unit*  attacker;             // Attacker
        Unit*  target;               // Target for damage

        // Spell parameters
        uint32            GetSpellId()    const { return SpellID; }
        SpellEntry const* GetSpellProto() const { return m_spellInfo; }
        SpellSchoolMask   GetSchoolMask() const;

        // Damage types
        union {
            uint32 damage;
            uint32 heal;
        };

        int32  cleanDamage;          // Used for rage and healing calculation

        // Damage calculation
        uint32 baseDamage;
        uint32 bonusCrit;
        int32  bonusDone;
        int32  bonusTaken;
        uint32 reduction;
        uint32 absorb;
        uint32 resist;
        uint32 blocked;
        uint32 Damage() const
        {
            return IsHeal() ?
                (baseDamage + bonusCrit + bonusDone + bonusTaken + reduction + absorb /*+ resist + blocked*/) :
                (baseDamage + bonusCrit + bonusDone + bonusTaken - reduction - absorb - resist - blocked);
        };

        // Various types
        WeaponAttackType attackType;
        DamageEffectType damageType;
        uint32 HitInfo;
        uint32 TargetState;
        MeleeHitOutcome hitOutCome;  // TODO: remove this field (need use TargetState)

        uint32 rage;

        // Proc states
        uint32 procAttacker;
        uint32 procVictim;
        uint32 procEx;

        // Helpers
        bool   durabilityLoss;
        bool   physicalLog;
        bool   unused;
        bool   IsMeleeDamage() const { return !m_spellInfo; };
        bool   IsHeal()        const { return cleanDamage < 0; };

        void           AddFlag(DamageFlags flag)       { m_flags |= (1 << flag); };
        void           RemoveFlag(DamageFlags flag)    { m_flags &= ~(1 << flag); };
        bool           HasFlag(DamageFlags flag) const { return (m_flags & (1 << flag)); };

        // Attention: We now that this to function return the same (Remember the union from heal and damage)
        // Exits Only for better reading!
        uint32 GetRemainingHeal() { return heal; };
        uint32 GetRemainingDamage() { return damage; };

        // absorb
        uint32 AddAbsorb(uint32 addvalue);
        void AddPctAbsorb(float aborbPct);
        uint32 GetAbsorb() const { return absorb; };
        // should not be used, possible for some kinds of hacks
        void SetAbsorb(uint32 value) { absorb = value; };



    private:
        DamageInfo();     // Don't allow plain initialization!
        uint32            m_flags;
        SpellEntry const* m_spellInfo;
        uint32            SpellID;
};


struct SpellPeriodicAuraLogInfo
{
    SpellPeriodicAuraLogInfo(Aura *_aura, uint32 _damage, uint32 _overDamage, uint32 _absorb, uint32 _resist, float _multiplier, bool _critical = false)
        : aura(_aura), damage(_damage), overDamage(_overDamage), absorb(_absorb), resist(_resist), multiplier(_multiplier), critical(_critical) {}

    Aura   *aura;
    uint32 damage;
    uint32 overDamage;                                      // overkill/overheal
    uint32 absorb;
    uint32 resist;
    float  multiplier;
    bool   critical;
};

uint32 createProcExtendMask(DamageInfo *damageInfo, SpellMissInfo missCondition);

enum SpellAuraProcResult
{
    SPELL_AURA_PROC_OK              = 0,                    // proc was processed, will remove charges
    SPELL_AURA_PROC_FAILED          = 1,                    // proc failed - if at least one aura failed the proc, charges won't be taken
    SPELL_AURA_PROC_CANT_TRIGGER    = 2                     // aura can't trigger - skip charges taking, move to next aura if exists
};

typedef SpellAuraProcResult(Unit::*pAuraProcHandler)(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
extern pAuraProcHandler AuraProcHandler[TOTAL_AURAS];

#define MAX_DECLINED_NAME_CASES 5

struct DeclinedName
{
    std::string name[MAX_DECLINED_NAME_CASES];
};

enum CurrentSpellTypes
{
    CURRENT_MELEE_SPELL             = 0,
    CURRENT_GENERIC_SPELL           = 1,
    CURRENT_AUTOREPEAT_SPELL        = 2,
    CURRENT_CHANNELED_SPELL         = 3
};

#define CURRENT_FIRST_NON_MELEE_SPELL 1
#define CURRENT_MAX_SPELL             4

struct GlobalCooldown
{
    explicit GlobalCooldown(uint32 _dur = 0, uint32 _time = 0) : duration(_dur), cast_time(_time) {}

    uint32 duration;
    uint32 cast_time;
};

typedef UNORDERED_MAP<uint32 /*category*/, GlobalCooldown> GlobalCooldownList;

class GlobalCooldownMgr                                     // Shared by Player and CharmInfo
{
    public:
        GlobalCooldownMgr() {}

    public:
        bool HasGlobalCooldown(SpellEntry const* spellInfo) const;
        void AddGlobalCooldown(SpellEntry const* spellInfo, uint32 gcd);
        void CancelGlobalCooldown(SpellEntry const* spellInfo);

        uint32 GetGlobalCooldown(SpellEntry const* spellInfo) const;

    private:
        GlobalCooldownList m_GlobalCooldowns;
};

enum CharmStateType
{
    CHARM_STATE_REACT    = 0,
    CHARM_STATE_COMMAND  = 1,
    CHARM_STATE_ACTIVITY = 2,
    CHARM_STATE_ACTION   = 3,
};

enum ActiveStates
{
    ACT_PASSIVE  = 0x01,                                    // 0x01 - passive
    ACT_CASTABLE = 0x80,                                    // 0x80 - castable
    ACT_DISABLED = 0x81,                                    // 0x80 - castable
    ACT_ACTIVE   = 0xC0,                                    // 0x40 | 0x80 - auto cast + castable
    ACT_ENABLED  = 0xC1,                                    // 0x40 | 0x80 - auto cast + castable
    ACT_COMMAND  = 0x07,                                    // 0x01 | 0x02 | 0x04
    ACT_REACTION = 0x06,                                    // 0x02 | 0x04
    ACT_DECIDE   = 0x00                                     // custom
};

enum ReactStates
{
    REACT_PASSIVE    = 0,
    REACT_DEFENSIVE  = 1,
    REACT_AGGRESSIVE = 2
};

enum CommandStates
{
    COMMAND_STAY    = 0,
    COMMAND_FOLLOW  = 1,
    COMMAND_ATTACK  = 2,
    COMMAND_ABANDON = 3
};

enum ActionStates
{
    ACTIONS_ENABLE  = 0x00,
    ACTIONS_DISABLE = 0x08
};

#define UNIT_ACTION_BUTTON_ACTION(X) (uint32(X) & 0x00FFFFFF)
#define UNIT_ACTION_BUTTON_TYPE(X)   ((uint32(X) & 0xFF000000) >> 24)
#define MAX_UNIT_ACTION_BUTTON_ACTION_VALUE (0x00FFFFFF+1)
#define MAKE_UNIT_ACTION_BUTTON(A,T) (uint32(A) | (uint32(T) << 24))

struct UnitActionBarEntry
{
    UnitActionBarEntry() : packedData(uint32(ACT_DISABLED) << 24) {}

    uint32 packedData;

    // helper
    ActiveStates GetType() const { return ActiveStates(UNIT_ACTION_BUTTON_TYPE(packedData)); }
    uint32 GetAction() const { return UNIT_ACTION_BUTTON_ACTION(packedData); }
    bool IsActionBarForSpell() const
    {
        ActiveStates Type = GetType();
        return Type == ACT_DISABLED || Type == ACT_ENABLED || Type == ACT_PASSIVE;
    }

    void SetActionAndType(uint32 action, ActiveStates type)
    {
        packedData = MAKE_UNIT_ACTION_BUTTON(action,type);
    }

    void SetType(ActiveStates type)
    {
        packedData = MAKE_UNIT_ACTION_BUTTON(UNIT_ACTION_BUTTON_ACTION(packedData),type);
    }

    void SetAction(uint32 action)
    {
        packedData = (packedData & 0xFF000000) | UNIT_ACTION_BUTTON_ACTION(action);
    }
};

typedef UnitActionBarEntry CharmSpellEntry;

enum ActionBarIndex
{
    ACTION_BAR_INDEX_START = 0,
    ACTION_BAR_INDEX_PET_SPELL_START = 3,
    ACTION_BAR_INDEX_PET_SPELL_END = 7,
    ACTION_BAR_INDEX_END = 10,
};

#define MAX_UNIT_ACTION_BAR_INDEX (ACTION_BAR_INDEX_END-ACTION_BAR_INDEX_START)

struct MANGOS_DLL_SPEC CharmInfo
{
    public:
        explicit CharmInfo(Unit* unit);
        uint32 GetPetNumber() const { return m_petnumber; }
        void SetPetNumber(uint32 petnumber, bool statwindow);

        bool HasCommandState(CommandStates state) { return HasState(CHARM_STATE_COMMAND, state); }
        bool HasReactState(ReactStates state) { return HasState(CHARM_STATE_REACT, state); }
        void SetReactState(ReactStates state) { SetState(CHARM_STATE_REACT, state); }

        uint32 GetState() const { return m_State; };
        void   SetState(uint32 state) { m_State = state; };

        uint8  GetState(CharmStateType type);
        bool   HasState(CharmStateType type, uint8 value);
        void   SetState(CharmStateType type, uint8 value);

        void InitPossessCreateSpells();
        void InitVehicleCreateSpells(uint8 seatId = 0);
        void InitCharmCreateSpells();
        void InitPetActionBar();
        void InitEmptyActionBar();
                                                            //return true if successful
        bool AddSpellToActionBar(uint32 spellid, ActiveStates newstate = ACT_DECIDE);
        bool RemoveSpellFromActionBar(uint32 spell_id);
        void LoadPetActionBar(const std::string& data);
        void BuildActionBar(WorldPacket* data);
        void SetSpellAutocast(uint32 spell_id, bool state);
        void SetActionBar(uint8 index, uint32 spellOrAction, ActiveStates type)
        {
            PetActionBar[index].SetActionAndType(spellOrAction, type);
        }

        UnitActionBarEntry const* GetActionBarEntry(uint8 index) const { return &(PetActionBar[index]); }

        void ToggleCreatureAutocast(uint32 spellid, bool apply);

        CharmSpellEntry* GetCharmSpell(uint8 index) { return &(m_charmspells[index]); }

        GlobalCooldownMgr& GetGlobalCooldownMgr() { return m_GlobalCooldownMgr; }

    private:
        Unit* m_unit;
        UnitActionBarEntry PetActionBar[MAX_UNIT_ACTION_BAR_INDEX];
        CharmSpellEntry m_charmspells[CREATURE_MAX_SPELLS];
        uint32          m_State;                // 1st byte - ReactState, 2nd byte - CommandStates, 3d byte - unknown (Activity state), 4d byte - Action state
        uint32          m_petnumber;
        GlobalCooldownMgr m_GlobalCooldownMgr;
};

// used in CallForAllControlledUnits/CheckAllControlledUnits
enum ControlledUnitMask
{
    CONTROLLED_PET       = 0x01,
    CONTROLLED_MINIPET   = 0x02,
    CONTROLLED_GUARDIANS = 0x04,                            // including PROTECTOR_PET
    CONTROLLED_CHARM     = 0x08,
    CONTROLLED_TOTEMS    = 0x10,
};

// for clearing special attacks
#define REACTIVE_TIMER_START 4000

enum ReactiveType
{
    REACTIVE_DEFENSE      = 0,
    REACTIVE_HUNTER_PARRY = 1,
    REACTIVE_OVERPOWER    = 2
};

#define MAX_REACTIVE 3

// Used as MiscValue for SPELL_AURA_IGNORE_UNIT_STATE
enum IgnoreUnitState
{
    IGNORE_UNIT_TARGET_STATE      = 0,                      // target health, aura states, or combopoints
    IGNORE_UNIT_COMBAT_STATE      = 1,                      // ignore caster in combat state
    IGNORE_UNIT_TARGET_NON_FROZEN = 126,                    // ignore absent of frozen state
    IGNORE_UNIT_HEALTH_STATE      = 127,                    // ignore unit pct health state
};

struct SpellCooldown
{
    time_t end;
    uint16 itemid;
};

typedef std::map<uint32, SpellCooldown> SpellCooldowns;

// delay time next attack to prevent client attack animation problems
#define ATTACK_DISPLAY_DELAY 200
#define MAX_PLAYER_STEALTH_DETECT_RANGE 45.0f               // max distance for detection targets by player
#define MAX_CREATURE_ATTACK_RADIUS 45.0f                    // max distance for creature aggro (use with CONFIG_FLOAT_RATE_CREATURE_AGGRO)

// Regeneration defines
#define REGEN_TIME_FULL         2000                        // This determines how often regen value is computed
#define REGEN_TIME_HOLY_POWER   10000                       // This determines how often holy power regen is processed
#define REGEN_TIME_PET_FOCUS    1000
#define REGEN_TIME_PLAYER_FOCUS 1000

// delay time for evading
#define EVADE_TIME_DELAY     500
#define EVADE_TIME_DELAY_MIN 50

#define MAX_OBJECT_SLOT 5

#define MAX_DAMAGE_LOG_SECS     120

enum
{
    DAMAGE_DONE_COUNTER     = 0,
    DAMAGE_TAKEN_COUNTER    = 1,
    HEALING_DONE_COUNTER    = 2,
    MAX_DAMAGE_COUNTERS     = 3,
};

struct SpellProcEventEntry;                                 // used only privately
class  VehicleKit;

#define MAX_OBJECT_SLOT 5

class MANGOS_DLL_SPEC Unit : public WorldObject
{
    public:
        typedef UNORDERED_MULTIMAP<uint32 /*spellId*/, SpellAuraHolderPtr> SpellAuraHolderMap;
        typedef std::pair<SpellAuraHolderMap::iterator, SpellAuraHolderMap::iterator> SpellAuraHolderBounds;
        typedef std::pair<SpellAuraHolderMap::const_iterator, SpellAuraHolderMap::const_iterator> SpellAuraHolderConstBounds;
        typedef std::queue<SpellAuraHolderPtr> SpellAuraHolderQueue;
        typedef std::list<AuraPair> AuraList;
        typedef std::list<DiminishingReturn> Diminishing;
        typedef UNORDERED_SET<ObjectGuid> ComboPointHolderSet;
        typedef std::vector<SpellAuraHolderPtr> VisibleAuraMap;
        typedef UNORDERED_MAP<SpellEntry const*, ObjectGuid> SingleCastSpellTargetMap;
        typedef UNORDERED_MAP<SpellEntry const*, ObjectGuid /*targetGuid*/> TrackedAuraTargetMap;
        typedef UNORDERED_SET<uint32> SpellIdSet;

        virtual ~Unit ( );

        void AddToWorld();
        virtual void RemoveFromWorld(bool remove);

        virtual void CleanupsBeforeDelete();                // used in ~Creature/~Player (or before mass creature delete to remove cross-references to already deleted units)

        float GetObjectBoundingRadius() const override      // overwrite WorldObject version
        {
            return m_floatValues[UNIT_FIELD_BOUNDINGRADIUS];
        }

        DiminishingLevels GetDiminishing(DiminishingGroup  group);
        void IncrDiminishing(DiminishingGroup group);
        void ApplyDiminishingToDuration(DiminishingGroup  group, int32 &duration,Unit* caster, DiminishingLevels Level, int32 limitduration, bool isReflected);
        void ApplyDiminishingAura(DiminishingGroup  group, bool apply);
        void ClearDiminishings() { m_Diminishing.clear(); }

        virtual void Update(uint32 update_diff, uint32 time) override;

        void setAttackTimer(WeaponAttackType type, uint32 time) { m_attackTimer[type] = time; }
        void resetAttackTimer(WeaponAttackType type = BASE_ATTACK);
        uint32 getAttackTimer(WeaponAttackType type) const { return m_attackTimer[type]; }
        bool isAttackReady(WeaponAttackType type = BASE_ATTACK) const { return m_attackTimer[type] == 0; }
        bool haveOffhandWeapon() const;
        bool UpdateMeleeAttackingState();
        bool CanUseEquippedWeapon(WeaponAttackType attackType) const
        {
            if (IsInFeralForm())
                return false;

            switch (attackType)
            {
                default:
                case BASE_ATTACK:
                    return !HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
                case OFF_ATTACK:
                    return !HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISARM_OFFHAND);
                case RANGED_ATTACK:
                    return !HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISARM_RANGED);
            }
        }

        float GetCombatReach(bool forMeleeRange /*=true*/) const;
        float GetCombatReach(Unit const* pVictim, bool forMeleeRange = true, float flatMod = 0.0f) const;
        float GetCombatDistance(Unit const* pVictim, bool forMeleeRange = true) const;
        bool CanReachWithMeleeAttack(Unit const* pVictim, float flatMod = 0.0f) const;

        uint32 m_extraAttacks;

        bool IsInCombat() const { return GetMap() ? GetMap()->IsInCombat(GetObjectGuid()) : false; }
        Unit* getAttackerForHelper();                       // If someone wants to help, who to give them
        bool Attack(Unit *victim, bool meleeAttack);
        void AttackedBy(Unit* attacker);
        void CastStop(uint32 except_spellid = 0);
        bool AttackStop(bool targetSwitch = false);
        void RemoveAllAttackers();
        bool isAttackingPlayer() const;
        Unit* getVictim() const { return m_attackingGuid ? GetMap() ? GetMap()->GetUnit(m_attackingGuid) : NULL : NULL; }
        virtual bool IsInEvadeMode() const { return false; };
        void CombatStop(bool includingCast = false);
        void CombatStopWithPets(bool includingCast = false);
        void StopAttackFaction(uint32 faction_id);
        Unit* SelectRandomUnfriendlyTarget(Unit* except = NULL, float radius = ATTACK_DISTANCE) const;
        Unit* SelectRandomFriendlyTarget(Unit* except = NULL, float radius = ATTACK_DISTANCE) const;
        bool hasNegativeAuraWithInterruptFlag(uint32 flag);
        bool hasPositiveAuraWithInterruptFlag(uint32 flag);
        void SendMeleeAttackStop(Unit* victim);
        void SendMeleeAttackStart(Unit* pVictim);

        void addUnitState(uint32 f) { m_state |= f; }
        bool hasUnitState(uint32 f) const { return (m_state & f); }
        void clearUnitState(uint32 f) { m_state &= ~f; }
        bool CanFreeMove() const
        {
            return !hasUnitState(UNIT_STAT_NO_FREE_MOVE) && !GetOwnerGuid();
        }

        uint32 getLevel() const { return GetUInt32Value(UNIT_FIELD_LEVEL); }
        virtual uint32 GetLevelForTarget(Unit const* /*target*/) const { return getLevel(); }
        void SetLevel(uint32 lvl);
        virtual uint8 getRace() const { return GetByteValue(UNIT_FIELD_BYTES_0, 0); }
        uint32 getRaceMask() const { return getRace() ? 1 << (getRace()-1) : 0; }
        uint8 getClass() const { return GetByteValue(UNIT_FIELD_BYTES_0, 1); }
        uint32 getClassMask() const { return 1 << (getClass()-1); }
        uint8 getGender() const { return GetByteValue(UNIT_FIELD_BYTES_0, 2); }

        float GetStat(Stats stat) const { return float(GetUInt32Value(UNIT_FIELD_STAT0+stat)); }
        void SetStat(Stats stat, int32 val) { SetStatInt32Value(UNIT_FIELD_STAT0+stat, val); }
        uint32 GetArmor() const { return GetResistance(SPELL_SCHOOL_NORMAL) ; }
        void SetArmor(int32 val) { SetResistance(SPELL_SCHOOL_NORMAL, val); }

        uint32 GetResistance(SpellSchools school) const { return GetUInt32Value(UNIT_FIELD_RESISTANCES+school); }
        void SetResistance(SpellSchools school, int32 val) { SetStatInt32Value(UNIT_FIELD_RESISTANCES+school,val); }
        uint32 GetResistance(SpellSchoolMask schoolMask) const;

        uint32 GetHealth()    const { return GetUInt32Value(UNIT_FIELD_HEALTH); }
        uint32 GetMaxHealth() const { return GetUInt32Value(UNIT_FIELD_MAXHEALTH); }
        float GetHealthPercent() const { return (GetHealth()*100.0f) / GetMaxHealth(); }
        void SetHealth(   uint32 val);
        void SetMaxHealth(uint32 val);
        void SetHealthPercent(float percent);
        int32 ModifyHealth(int32 val);

        Powers getPowerType() const { return Powers(GetByteValue(UNIT_FIELD_BYTES_0, 3)); }
        void setPowerType(Powers power);
        int32 GetPower(Powers power) const;
        int32 GetPowerByIndex(uint32 index) const;
        int32 GetMinPowerByIndex(uint32 index) const;
        int32 GetMinPower(Powers power) const;
        int32 GetMaxPower(Powers power) const;
        int32 GetMaxPowerByIndex(uint32 index) const;
        void SetPowerByIndex(uint32 power, int32 val);
        void SetMaxPowerByIndex(uint32 power, int32 val);
        void SetPower(Powers power, int32 val);
        void SetMaxPower(Powers power, int32 val);
        int32 ModifyPower(Powers power, int32 val);
        void ApplyPowerMod(Powers power, uint32 val, bool apply);
        void ApplyMaxPowerMod(Powers power, uint32 val, bool apply);
        void ResetHolyPowerRegenTimer() { m_holyPowerRegenTimer = REGEN_TIME_HOLY_POWER; }

        static uint32 GetPowerIndexByClass(Powers power, uint32 classId);
        static Powers GetPowerTypeByIndex(uint32 index, uint32 classId);
        uint32 GetPowerIndex(Powers power) const { return GetPowerIndexByClass(power, getClass()); }
        Powers getPowerType(uint32 index) const { return GetPowerTypeByIndex(index, getClass()); }

        uint32 GetAttackTime(WeaponAttackType att) const { return (uint32)(GetFloatValue(UNIT_FIELD_BASEATTACKTIME+att)/m_modAttackSpeedPct[att]); }
        void SetAttackTime(WeaponAttackType att, uint32 val) { SetFloatValue(UNIT_FIELD_BASEATTACKTIME+att,val*m_modAttackSpeedPct[att]); }
        void ApplyAttackTimePercentMod(WeaponAttackType att,float val, bool apply);
        void ApplyCastTimePercentMod(float val, bool apply);

        SheathState GetSheath() const { return SheathState(GetByteValue(UNIT_FIELD_BYTES_2, 0)); }
        virtual void SetSheath(SheathState sheathed) { SetByteValue(UNIT_FIELD_BYTES_2, 0, sheathed); }

        // faction template id
        uint32 getFaction() const { return GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE); }
        uint32 GetOriginalFaction() const { return m_originalFaction; }
        void setFaction(uint32 faction) { if (!m_originalFaction) m_originalFaction = faction; SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, faction); }
        FactionTemplateEntry const* getFactionTemplateEntry() const;
        virtual bool IsHostileTo(Unit const* unit) const override;
        bool IsHostileToPlayers() const;
        virtual bool IsFriendlyTo(Unit const* unit) const override;
        bool IsNeutralToAll() const;
        bool IsContestedGuard() const
        {
            if (FactionTemplateEntry const* entry = getFactionTemplateEntry())
                return entry->IsContestedGuardFaction();

            return false;
        }
        bool IsPvP() const { return HasByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_PVP); }
        void SetPvP(bool state);
        bool IsFFAPvP() const { return HasByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP); }
        void SetFFAPvP(bool state);
        uint32 GetCreatureType() const;
        virtual uint32 GetCreatureTypeMask() const
        {
            uint32 creatureType = GetCreatureType();
            return (creatureType >= 1) ? (1 << (creatureType - 1)) : 0;
        }

        uint8 getStandState() const { return GetByteValue(UNIT_FIELD_BYTES_1, 0); }
        bool IsSitState() const;
        bool IsStandState() const;
        void SetStandState(uint8 state);

        void  SetStandFlags(uint8 flags) { SetByteFlag(UNIT_FIELD_BYTES_1, 2,flags); }
        void  RemoveStandFlags(uint8 flags) { RemoveByteFlag(UNIT_FIELD_BYTES_1, 2,flags); }

        bool IsMounted() const { return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNT); }
        uint32 GetMountID() const { return GetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID); }
        void Mount(uint32 mount, uint32 spellId = 0, uint32 vehicleId = 0, uint32 creatureEntry = 0);
        void Unmount(bool from_aura = false);
        MountCapabilityEntry const* GetMountCapability(uint32 mountType) const;

        uint16 GetMaxSkillValueForLevel(Unit const* target = NULL) const { return (target ? GetLevelForTarget(target) : getLevel()) * 5; }
        void DealDamageMods(DamageInfo* damageInfo);
        uint32 DealDamage(Unit *pVictim, uint32 damage, DamageInfo* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellEntry const *spellProto, bool durabilityLoss);
        uint32 DealDamage(Unit* pVictim, DamageInfo* damageInfo, bool durabilityLoss);
        uint32 DealDamage(DamageInfo* damageInfo);
        int32  DealHeal(Unit* pVictim, uint32 addhealth, SpellEntry const* spellProto, bool critical = false, uint32 absorb = 0);
        int32  DealHeal(DamageInfo* healInfo, bool critical = false);

        void PetOwnerKilledUnit(Unit* pVictim);

        void ProcDamageAndSpell(DamageInfo* damageInfo);
        void ProcDamageAndSpellFor( bool isVictim, DamageInfo* damage);
        // wrapper for old proc methods
        void ProcDamageAndSpell(Unit *pVictim, uint32 procAttacker, uint32 procVictim, uint32 procEx, uint32 amount, WeaponAttackType attType = BASE_ATTACK, SpellEntry const *procSpell = NULL);

        void HandleEmote(uint32 emote_id);                  // auto-select command/state
        void HandleEmoteCommand(uint32 emote_id);
        void HandleEmoteState(uint32 emote_id);
        void PlayOneShotAnimKit(uint32 id);
        void AttackerStateUpdate(Unit* pVictim, WeaponAttackType attType = BASE_ATTACK, bool extra = false );

        float MeleeMissChanceCalc(const Unit *pVictim, WeaponAttackType attType) const;

        void CalculateMeleeDamage(DamageInfo *damageInfo);
        void DealMeleeDamage(DamageInfo *damageInfo, bool durabilityLoss);

        bool IsAllowedDamageInArea(Unit* pVictim) const;

        void CalculateSpellDamage(DamageInfo* damageInfo, float DamageMultiplier = 1.0f);
        void DealSpellDamage(DamageInfo* damageInfo, bool durabilityLoss);

        // player or player's pet resilience (-1%)
        uint32 GetCritDamageReduction(uint32 damage) const { return GetCombatRatingDamageReduction(CR_RESILIENCE_DAMAGE_TAKEN, 1.0f, 33.0f, damage); }
        // player or player's pet resilience (-1%), cap 100%
        uint32 GetDamageReduction(uint32 damage) const { return GetCombatRatingDamageReduction(CR_RESILIENCE_DAMAGE_TAKEN, 1.0f, 100.0f, damage); }

        float MeleeSpellMissChance(Unit* pVictim, WeaponAttackType attType, int32 skillDiff, SpellEntry const* spell);
        SpellMissInfo MeleeSpellHitResult(Unit* pVictim, SpellEntry const* spell, bool canMiss = true);
        SpellMissInfo MagicSpellHitResult(Unit* pVictim, SpellEntry const* spell, bool dotDamage = false);
        SpellMissInfo SpellHitResult(Unit* pVictim, SpellEntry const* spell, bool dotDamage = false);
        SpellMissInfo SpellResistResult(Unit* pVictim, SpellEntry const* spell);
        uint32 CalculateBaseSpellHitChance(Unit* pVictim);

        float GetUnitDodgeChance() const;
        float GetUnitParryChance() const;
        float GetUnitBlockChance() const;
        float GetUnitCriticalChance(WeaponAttackType attackType, const Unit* pVictim) const;

        virtual uint32 GetShieldBlockDamageValue() const = 0;
        float GetWeaponProcChance() const;
        float GetPPMProcChance(uint32 WeaponSpeed, float PPM) const;

        MeleeHitOutcome RollMeleeOutcomeAgainst (const Unit *pVictim, WeaponAttackType attType) const;
        MeleeHitOutcome RollMeleeOutcomeAgainst (const Unit *pVictim, WeaponAttackType attType, int32 crit_chance, int32 miss_chance, int32 dodge_chance, int32 parry_chance, int32 block_chance) const;

        bool isVendor()       const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR); }
        bool isTrainer()      const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER); }
        bool isQuestGiver()   const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER); }
        bool isGossip()       const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); }
        bool isTaxi()         const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_FLIGHTMASTER); }
        bool isGuildMaster()  const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PETITIONER); }
        bool isBattleMaster() const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_BATTLEMASTER); }
        bool isBanker()       const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_BANKER); }
        bool isInnkeeper()    const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_INNKEEPER); }
        bool isSpiritHealer() const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER); }
        bool isSpiritGuide()  const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITGUIDE); }
        bool isTabardDesigner()const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TABARDDESIGNER); }
        bool isAuctioner()    const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER); }
        bool isArmorer()      const { return HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_REPAIR); }
        bool isServiceProvider() const
        {
            return HasFlag(UNIT_NPC_FLAGS,
                UNIT_NPC_FLAG_VENDOR | UNIT_NPC_FLAG_TRAINER | UNIT_NPC_FLAG_FLIGHTMASTER |
                UNIT_NPC_FLAG_PETITIONER | UNIT_NPC_FLAG_BATTLEMASTER | UNIT_NPC_FLAG_BANKER |
                UNIT_NPC_FLAG_INNKEEPER | UNIT_NPC_FLAG_SPIRITHEALER |
                UNIT_NPC_FLAG_SPIRITGUIDE | UNIT_NPC_FLAG_TABARDDESIGNER | UNIT_NPC_FLAG_AUCTIONEER );
        }
        bool isSpiritService() const { return HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER | UNIT_NPC_FLAG_SPIRITGUIDE ); }

        bool IsTaxiFlying()  const { return hasUnitState(UNIT_STAT_TAXI_FLIGHT); }

        bool isInCombat()  const { return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT); }
        void SetInCombatState(bool PvP, Unit* enemy = NULL);
        void SetInCombatWith(Unit* enemy);
        void ClearInCombat();
        uint32 GetCombatTimer() const { return m_CombatTimer; }
        virtual bool IsCombatStationary();

        SpellAuraHolderBounds GetSpellAuraHolderBounds(uint32 spell_id)
        {
            return m_spellAuraHolders.equal_range(spell_id);
        }
        SpellAuraHolderConstBounds GetSpellAuraHolderBounds(uint32 spell_id) const
        {
            return m_spellAuraHolders.equal_range(spell_id);
        }

        bool HasAuraType(AuraType auraType) const;
        bool HasAuraTypeWithCaster(AuraType auraType, ObjectGuid casterGuid) const;
        bool HasNegativeAuraType(AuraType auraType) const;
        bool HasAffectedAura(AuraType auraType, SpellEntry const* spellProto) const;
        bool HasAura(uint32 spellId, SpellEffectIndex effIndex) const;
        bool HasAura(uint32 spellId) const
        {
            return m_spellAuraHolders.find(spellId) != m_spellAuraHolders.end();
        }
        bool HasAuraOfDifficulty(uint32 spellId) const;

        virtual bool HasSpell(uint32 /*spellID*/) const { return false; }

        bool HasStealthAura()      const { return HasAuraType(SPELL_AURA_MOD_STEALTH); }
        bool HasInvisibilityAura() const { return HasAuraType(SPELL_AURA_MOD_INVISIBILITY); }
        bool isFeared()  const { return HasAuraType(SPELL_AURA_MOD_FEAR); }
        bool isInRoots() const { return HasAuraType(SPELL_AURA_MOD_ROOT); }
        bool IsPolymorphed() const;

        bool IsCrowdControlled() const;

        bool isFrozen() const;
        bool IsIgnoreUnitState(SpellEntry const *spell, IgnoreUnitState ignoreState);

        bool isTargetableForAttack(bool inversAlive = false) const;
        bool isPassiveToHostile() const { return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE); }

        virtual bool IsInWater() const;
        virtual bool IsUnderWater() const;
        bool isInAccessablePlaceFor(Unit const* unit) const;

        void SendHealSpellLog(Unit *pVictim, uint32 SpellID, uint32 Damage, uint32 OverHeal, bool critical = false, uint32 absorb = 0);
        void SendEnergizeSpellLog(Unit *pVictim, uint32 SpellID, int32 Damage,Powers powertype);
        void EnergizeBySpell(Unit *pVictim, uint32 SpellID, int32 Damage, Powers powertype);
        uint32 SpellNonMeleeDamageLog(Unit *pVictim, uint32 spellID, uint32 damage);
        void CastSpell(Unit* Victim, uint32 spellId, bool triggered, Item *castItem = NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastSpell(Unit* Victim,SpellEntry const *spellInfo, bool triggered, Item *castItem= NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastCustomSpell(Unit* Victim, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item *castItem= NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastCustomSpell(Unit* Victim,SpellEntry const *spellInfo, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item *castItem= NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastSpell(float x, float y, float z, uint32 spellId, bool triggered, Item *castItem = NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastSpell(float x, float y, float z, SpellEntry const *spellInfo, bool triggered, Item *castItem = NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastSpell(WorldLocation const& loc, uint32 spellId, bool triggered, Item *castItem = NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastSpell(WorldLocation const& loc, SpellEntry const *spellInfo, bool triggered, Item *castItem = NULL, Aura const* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastCustomSpell(WorldLocation const& loc, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item *castItem = NULL, Aura* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
        void CastCustomSpell(WorldLocation const& loc, SpellEntry const *spellInfo, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item *castItem = NULL, Aura* triggeredByAura = NULL, ObjectGuid originalCaster = ObjectGuid(), SpellEntry const* triggeredBy = NULL);
 
        void DeMorph();

        void SendAttackStateUpdate(DamageInfo* damageInfo);
        void SendSpellNonMeleeDamageLog(DamageInfo *log);
        void SendSpellNonMeleeDamageLog(Unit *target,uint32 SpellID, uint32 Damage, SpellSchoolMask damageSchoolMask, uint32 AbsorbedDamage, uint32 Resist, bool PhysicalDamage, uint32 Blocked, bool CriticalHit = false);
        void SendPeriodicAuraLog(SpellPeriodicAuraLogInfo *pInfo);
        void SendSpellMiss(Unit *target, uint32 spellID, SpellMissInfo missInfo);
        void SendSpellDamageResist(Unit* target, uint32 spellId);
        void SendSpellDamageImmune(Unit* target, uint32 spellId);

        void NearTeleportTo(WorldLocation const& loc, uint32 options = 0);
        void NearTeleportTo(float x, float y, float z, float orientation, bool casting = false);
        void MonsterMoveToDestination(float x, float y, float z, float o, float speed, float height, bool isKnockBack = false, Unit* target = NULL);
        // recommend use MonsterMove/MonsterMoveWithSpeed for most case that correctly work with movegens
        // if used additional args in ... part then floats must explicitly casted to double
        virtual bool SetPosition(Position const& pos, bool teleport = false);
        virtual void SetFallInformation(uint32 time, float z) {};

        void MonsterMoveWithSpeed(float x, float y, float z, float speed, bool generatePath = true, bool forceDestination = false);
        // recommend use MonsterMove/MonsterMoveWithSpeed for most case that correctly work with movegens
        // if used additional args in ... part then floats must explicitly casted to double
        void SendHeartBeat();
        bool IsFalling() const { return m_movementInfo.HasMovementFlag(MovementFlags(MOVEFLAG_FALLING | MOVEFLAG_FALLINGFAR));};
        bool IsLevitating() const { return m_movementInfo.HasMovementFlag(MOVEFLAG_LEVITATING); }
        bool IsWalking() const { return m_movementInfo.HasMovementFlag(MOVEFLAG_WALK_MODE); }
        bool IsRooted() const { return m_movementInfo.HasMovementFlag(MOVEFLAG_ROOT); }
        bool IsSwimming() const { return m_movementInfo.HasMovementFlag(MOVEFLAG_SWIMMING); }
        bool isMoving() const { return m_movementInfo.HasMovementFlag(movementFlagsMask); }
        bool isMovingOrTurning() const { return m_movementInfo.HasMovementFlag(movementOrTurningFlagsMask); }

        virtual void SetRoot(bool enabled, uint32 val = 0) {}
        virtual void SetWaterWalk(bool enabled, uint32 val = 0) {}

        void SetInFront(Unit const* target);
        void SetFacingTo(float ori);
        void SetFacingToObject(WorldObject* pObject);

        void SendHighestThreatUpdate(HostileReference* pHostileReference);
        void SendThreatClear();
        void SendThreatRemove(HostileReference* pHostileReference);
        void SendThreatUpdate();

        bool isAlive() const { return (m_deathState == ALIVE); };
        bool isDead() const { return ( m_deathState == DEAD || m_deathState == CORPSE ); };
        DeathState getDeathState() const { return m_deathState; };
        virtual void SetDeathState(DeathState s);           // overwritten in Creature/Player/Pet

        ObjectGuid const& GetOwnerGuid() const { return GetGuidValue(UNIT_FIELD_SUMMONEDBY); }
        void SetOwnerGuid(ObjectGuid ownerGuid);
        ObjectGuid const& GetCreatorGuid() const;
        void SetCreatorGuid(ObjectGuid creator) { SetGuidValue(UNIT_FIELD_CREATEDBY, creator); }
        ObjectGuid const& GetPetGuid() const { return GetGuidValue(UNIT_FIELD_SUMMON); }
        void SetPetGuid(ObjectGuid pet) { SetGuidValue(UNIT_FIELD_SUMMON, pet); }
        ObjectGuid const& GetCharmerGuid() const { return GetGuidValue(UNIT_FIELD_CHARMEDBY); }
        void SetCharmerGuid(ObjectGuid owner) { SetGuidValue(UNIT_FIELD_CHARMEDBY, owner); }
        ObjectGuid const& GetCharmGuid() const { return GetGuidValue(UNIT_FIELD_CHARM); }
        void SetCharmGuid(ObjectGuid charm) { SetGuidValue(UNIT_FIELD_CHARM, charm); }
        ObjectGuid const& GetTargetGuid() const { return GetGuidValue(UNIT_FIELD_TARGET); }
        void SetTargetGuid(ObjectGuid targetGuid) { SetGuidValue(UNIT_FIELD_TARGET, targetGuid); }
        ObjectGuid const& GetChannelObjectGuid() const { return GetGuidValue(UNIT_FIELD_CHANNEL_OBJECT); }
        void SetChannelObjectGuid(ObjectGuid targetGuid) { SetGuidValue(UNIT_FIELD_CHANNEL_OBJECT, targetGuid); }

        void SetCritterGuid(ObjectGuid critterGuid) { SetGuidValue(UNIT_FIELD_CRITTER, critterGuid); }
        ObjectGuid const& GetCritterGuid() const { return GetGuidValue(UNIT_FIELD_CRITTER); }

        void RemoveMiniPet();
        Pet* GetMiniPet() const;
        void SetMiniPet(Unit* pet) { SetCritterGuid(pet ? pet->GetObjectGuid() : ObjectGuid()); }

        ObjectGuid const& GetCharmerOrOwnerGuid() const { return GetCharmerGuid() ? GetCharmerGuid() : GetOwnerGuid(); }
        ObjectGuid const& GetCharmerOrOwnerOrOwnGuid() const
        {
            if (ObjectGuid const& guid = GetCharmerOrOwnerGuid())
                return guid;
            return GetObjectGuid();
        }
        bool isCharmedOwnedByPlayerOrPlayer() const { return GetCharmerOrOwnerOrOwnGuid().IsPlayer(); }

        Player* GetSpellModOwner() const;

        Unit* GetOwner() const;
        Pet* GetPet() const;
        Unit* GetCharmer() const;
        Unit* GetCharm() const;
        Unit* GetCreator() const;
        void Uncharm();
        Unit* GetCharmerOrOwner() const { return GetCharmerGuid() ? GetCharmer() : GetOwner(); }
        Unit* GetCharmerOrOwnerOrSelf()
        {
            if (Unit* u = GetCharmerOrOwner())
                return u;

            return this;
        }
        bool IsCharmerOrOwnerPlayerOrPlayerItself() const;
        Player* GetCharmerOrOwnerPlayerOrPlayerItself();
        Player const* GetCharmerOrOwnerPlayerOrPlayerItself() const;

        void SetPet(Pet* pet);
        void SetCharm(Unit* pet);

        void AddPetToList(Pet* pet);
        void RemovePetFromList(Pet* pet);
        GuidSet const& GetPets() const { return m_groupPets; }

        void AddGuardian(Pet* pet);
        void RemoveGuardian(Pet* pet);
        void RemoveGuardians();
        Pet* FindGuardianWithEntry(uint32 entry, bool notdead = false);
        GuidSet const& GetGuardians() const { return m_guardianPets; }
        Pet* GetProtectorPet();                             // expected single case in guardian list

        bool isCharmed() const { return !GetCharmerGuid().IsEmpty(); }

        CharmInfo* GetCharmInfo() { return m_charmInfo; }
        uint8 GetCharmState(CharmStateType type) const { return m_charmInfo ? m_charmInfo->GetState(type) : 0; }
        CharmInfo* InitCharmInfo(Unit* charm);

        void SendCharmState();

        ObjectGuid const& GetTotemGuid(TotemSlot slot) const { return m_TotemSlot[slot]; }
        Totem* GetTotem(TotemSlot slot) const;
        bool IsAllTotemSlotsUsed() const;

        void _AddTotem(TotemSlot slot, Totem* totem);       // only for call from Totem summon code
        void _RemoveTotem(Totem* totem);                    // only for call from Totem class

        template<typename Func>
        void CallForAllControlledUnits(Func const& func, uint32 controlledMask);
        template<typename Func>
        bool CheckAllControlledUnits(Func const& func, uint32 controlledMask) const;

        bool AddSpellAuraHolder(SpellAuraHolderPtr holder);
        void AddAuraToModList(Aura *aura);

        SpellAuraHolderPtr _AddAura(uint32 spellID, uint32 duration = 60000, Unit* caster = NULL);
        float CheckAuraStackingAndApply(Aura *Aur, UnitMods unitMod, UnitModifierType modifierType, float amount, bool apply, int32 miscMask = 0, int32 miscValue = 0);

        // removing specific aura stack
        void RemoveAura(Aura* aura, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAura(uint32 spellId, SpellEffectIndex effindex, Aura* except = NULL);
        void RemoveSpellAuraHolder(SpellAuraHolderPtr holder, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveSingleAuraFromSpellAuraHolder(SpellAuraHolderPtr holder, SpellEffectIndex index, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveSingleAuraFromSpellAuraHolder(uint32 id, SpellEffectIndex index, ObjectGuid casterGuid, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);

        bool AddSpellAuraHolderToRemoveList(SpellAuraHolderPtr holder);

        // removing specific aura stacks by diff reasons and selections
        void RemoveAurasDueToSpell(uint32 spellId, SpellAuraHolderPtr except = SpellAuraHolderPtr(NULL), AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAurasDueToItemSpell(Item* castItem, uint32 spellId);
        void RemoveAurasByCasterSpell(uint32 spellId, ObjectGuid casterGuid, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAurasDueToSpellBySteal(uint32 spellId, ObjectGuid casterGuid, Unit* stealer);
        void RemoveAurasDueToSpellByCancel(uint32 spellId);
        void RemoveAllGroupBuffsFromCaster(ObjectGuid guidCaster);
        bool HasSatedAura() const;

        // removing unknown aura stacks by diff reasons and selections
        void RemoveNotOwnTrackedTargetAuras(uint32 newPhase = 0x0);
        void RemoveAurasAtMechanicImmunity(uint32 mechMask, uint32 exceptSpellId, bool non_positive = false);
        void RemoveAurasByMechanicMask(uint32 mechMask, bool positiveOnly = false, uint32 count = 0);
        void RemoveSpellsCausingAura(AuraType auraType);
        void RemoveSpellsCausingAura(AuraType auraType, SpellAuraHolderPtr except);
        void RemoveSpellsCausingAura(AuraType auraType, ObjectGuid casterGuid);
        void RemoveRankAurasDueToSpell(uint32 spellId);
        bool RemoveNoStackAurasDueToAuraHolder(SpellAuraHolderPtr holder);
        void RemoveAurasWithInterruptFlags(uint32 flags);
        void RemoveAurasWithAttribute(uint32 flags, uint32 exclude = 0);
        void RemoveAurasWithDispelType(DispelType type, ObjectGuid casterGuid = ObjectGuid());
        void RemoveAllAuras(AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveArenaAuras(bool onleave = false);
        void RemoveAllAurasOnDeath();

        void HandleArenaPreparation(bool apply);
        bool RemoveSpellsCausingAuraByCaster(AuraType auraType, ObjectGuid casterGuid, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);

        // Method for enable-disable auras (currently - passive only) affect
        void TriggerPassiveAurasWithAttribute(bool active, uint32 flags);

        // removing specific aura FROM stack by diff reasons and selections
        void RemoveAuraHolderFromStack(uint32 spellId, uint32 stackAmount = 1, ObjectGuid casterGuid = ObjectGuid(), AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAuraHolderDueToSpellByDispel(uint32 spellId, uint32 stackAmount, ObjectGuid casterGuid, Unit *dispeller);

        void DelaySpellAuraHolder(uint32 spellId, int32 delaytime, ObjectGuid casterGuid);

        bool HasMorePoweredBuff(uint32 spellId);

        float GetResistanceBuffMods(SpellSchools school, bool positive) const { return GetFloatValue(positive ? UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + school : UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + school); }
        void SetResistanceBuffMods(SpellSchools school, bool positive, float val) { SetFloatValue(positive ? UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + school : UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + school,val); }
        void ApplyResistanceBuffModsMod(SpellSchools school, bool positive, float val, bool apply) { ApplyModSignedFloatValue(positive ? UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + school : UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + school, val, apply); }
        void ApplyResistanceBuffModsPercentMod(SpellSchools school, bool positive, float val, bool apply) { ApplyPercentModFloatValue(positive ? UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE + school : UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE + school, val, apply); }
        void InitStatBuffMods()
        {
            for (int i = STAT_STRENGTH; i < MAX_STATS; ++i) SetFloatValue(UNIT_FIELD_POSSTAT0+i, 0);
            for (int i = STAT_STRENGTH; i < MAX_STATS; ++i) SetFloatValue(UNIT_FIELD_NEGSTAT0+i, 0);
        }
        void ApplyStatBuffMod(Stats stat, float val, bool apply)
        {
            val *= GetModifierValue(UnitMods(UNIT_MOD_STAT_STRENGTH+stat), TOTAL_PCT);
            ApplyModSignedFloatValue((val > 0 ? UNIT_FIELD_POSSTAT0+stat : UNIT_FIELD_NEGSTAT0+stat), val, apply);
        }
        void ApplyStatPercentBuffMod(Stats stat, float val, bool apply)
        {
            ApplyPercentModFloatValue(UNIT_FIELD_POSSTAT0+stat, val, apply);
            ApplyPercentModFloatValue(UNIT_FIELD_NEGSTAT0+stat, val, apply);
        }
        void SetCreateStat(Stats stat, float val) { m_createStats[stat] = val; }
        void SetCreateHealth(uint32 val) { SetUInt32Value(UNIT_FIELD_BASE_HEALTH, val); }
        uint32 GetCreateHealth() const { return GetUInt32Value(UNIT_FIELD_BASE_HEALTH); }
        void SetCreateMana(uint32 val) { SetUInt32Value(UNIT_FIELD_BASE_MANA, val); }
        uint32 GetCreateMana() const { return GetUInt32Value(UNIT_FIELD_BASE_MANA); }
        int32 GetCreatePowers(Powers power) const;
        int32 GetCreateMaxPowers(Powers power) const;
        float GetPosStat(Stats stat) const { return GetFloatValue(UNIT_FIELD_POSSTAT0+stat); }
        float GetNegStat(Stats stat) const { return GetFloatValue(UNIT_FIELD_NEGSTAT0+stat); }
        float GetCreateStat(Stats stat) const { return m_createStats[stat]; }

        void SetCurrentCastedSpell(Spell * pSpell);
        virtual void ProhibitSpellSchool(SpellSchoolMask /*idSchoolMask*/, uint32 /*unTimeMs*/ ) { }
        void InterruptSpell(CurrentSpellTypes spellType, bool withDelayed = true, bool sendAutoRepeatCancelToClient = true);
        void FinishSpell(CurrentSpellTypes spellType, bool ok = true);

        // set withDelayed to true to account delayed spells as casted
        // delayed+channeled spells are always accounted as casted
        // we can skip channeled or delayed checks using flags
        bool IsNonMeleeSpellCasted(bool withDelayed, bool skipChanneled = false, bool skipAutorepeat = false) const;

        // set withDelayed to true to interrupt delayed spells too
        // delayed+channeled spells are always interrupted
        void InterruptNonMeleeSpells(bool withDelayed, uint32 spellid = 0);

        Spell* GetCurrentSpell(CurrentSpellTypes spellType) const { return m_currentSpells[spellType]; }
        Spell* FindCurrentSpellBySpellId(uint32 spell_id) const;

        bool CheckAndIncreaseCastCounter();
        void DecreaseCastCounter() { if (m_castCounter) --m_castCounter; }

        ObjectGuid m_ObjectSlotGuid[MAX_OBJECT_SLOT];
        uint32 m_detectInvisibilityMask;
        uint32 m_invisibilityMask;

        ShapeshiftForm GetShapeshiftForm() const { return ShapeshiftForm(GetByteValue(UNIT_FIELD_BYTES_2, 3)); }
        void  SetShapeshiftForm(ShapeshiftForm form)
        {
            SetByteValue(UNIT_FIELD_BYTES_2, 3, form);
            // always update this field to prevent problems with shapeshifting
            if (GetTypeId() == TYPEID_PLAYER)
                ForceValuesUpdateAtIndex(UNIT_FIELD_BYTES_2);
        }

        bool IsInFeralForm() const
        {
            ShapeshiftForm form = GetShapeshiftForm();
            return form == FORM_CAT || form == FORM_BEAR;
        }

        bool IsInDisallowedMountForm() const
        {
            ShapeshiftForm form = GetShapeshiftForm();
            return form != FORM_NONE && form != FORM_BATTLESTANCE && form != FORM_BERSERKERSTANCE && form != FORM_DEFENSIVESTANCE &&
                form != FORM_SHADOW && form != FORM_STEALTH;
        }

        virtual uint32 GetModelForForm(SpellShapeshiftFormEntry const* ssEntry) const;
        uint32 GetModelForForm() const;     // for current form

        float m_modMeleeHitChance;
        float m_modRangedHitChance;
        float m_modSpellHitChance;
        int32 m_baseSpellCritChance;

        float m_threatModifier[MAX_SPELL_SCHOOL];
        float m_modAttackSpeedPct[MAX_ATTACK_MOD];
        float m_modSpellSpeedPctNeg;
        float m_modSpellSpeedPctPos;

        // stat system
        bool HandleStatModifier(UnitMods unitMod, UnitModifierType modifierType, float amount, bool apply);
        void SetModifierValue(UnitMods unitMod, UnitModifierType modifierType, float value) { m_auraModifiersGroup[unitMod][modifierType] = value; }
        float GetModifierValue(UnitMods unitMod, UnitModifierType modifierType) const;
        float GetTotalStatValue(Stats stat) const;
        float GetTotalAuraModValue(UnitMods unitMod) const;
        SpellSchools GetSpellSchoolByAuraGroup(UnitMods unitMod) const;
        Stats GetStatByAuraGroup(UnitMods unitMod) const;
        Powers GetPowerTypeByAuraGroup(UnitMods unitMod) const;
        bool CanModifyStats() const { return m_canModifyStats; }
        void SetCanModifyStats(bool modifyStats) { m_canModifyStats = modifyStats; }
        virtual bool UpdateStats(Stats stat) = 0;
        virtual bool UpdateAllStats() = 0;
        virtual void UpdateResistances(uint32 school) = 0;
        virtual void UpdateAllResistances();
        virtual void UpdateArmor() = 0;
        virtual void UpdateMaxHealth() = 0;
        virtual void UpdateMaxPower(Powers power) = 0;
        virtual void UpdateAllMaxPower();
        virtual void UpdateAttackPowerAndDamage(bool ranged = false) = 0;
        virtual void UpdateDamagePhysical(WeaponAttackType attType) = 0;
        float GetTotalAttackPowerValue(WeaponAttackType attType) const;
        float GetWeaponDamageRange(WeaponAttackType attType ,WeaponDamageRange type) const;
        void SetBaseWeaponDamage(WeaponAttackType attType ,WeaponDamageRange damageRange, float value) { m_weaponDamage[attType][damageRange] = value; }

        // Visibility system
        UnitVisibility GetVisibility() const { return m_Visibility; }
        void SetVisibility(UnitVisibility x);
        void UpdateVisibilityAndView() override;            // overwrite WorldObject::UpdateVisibilityAndView()

        // common function for visibility checks for player/creatures with detection code
        bool isVisibleForOrDetect(Unit const* u, WorldObject const* viewPoint, bool detect, bool inVisibleList = false, bool is3dDistance = true, bool skipLOScheck = false) const;
        bool canDetectInvisibilityOf(Unit const* u) const;
        void SetPhaseMask(uint32 newPhaseMask, bool update) override;// overwrite WorldObject::SetPhaseMask
        bool IsVisibleTargetForSpell(WorldObject const* caster, SpellEntry const* spellInfo, WorldLocation const* location = NULL) const;

        // virtual functions for all world objects types
        bool isVisibleForInState(Player const* u, WorldObject const* viewPoint, bool inVisibleList) const override;
        // function for low level grid visibility checks in player/creature cases
        virtual bool IsVisibleInGridForPlayer(Player* pl) const = 0;
        bool isInvisibleForAlive() const;

        TrackedAuraTargetMap&       GetTrackedAuraTargets(TrackedAuraType type)       { return m_trackedAuraTargets[type]; }
        TrackedAuraTargetMap const& GetTrackedAuraTargets(TrackedAuraType type) const { return m_trackedAuraTargets[type]; }
        SpellImmuneList m_spellImmune[MAX_SPELL_IMMUNITY];

        SingleCastSpellTargetMap      & GetSingleCastSpellTargets()       { return m_singleCastSpellTargets; }
        SingleCastSpellTargetMap const& GetSingleCastSpellTargets() const { return m_singleCastSpellTargets; }
        Unit* GetSingleCastSpellTarget(uint32 spellId);

        // Threat related methods
        bool CanHaveThreatList() const;
        void AddThreat(Unit* pVictim, float threat = 0.0f, bool crit = false, SpellSchoolMask schoolMask = SPELL_SCHOOL_MASK_NONE, SpellEntry const *threatSpell = NULL);
        float ApplyTotalThreatModifier(float threat, SpellSchoolMask schoolMask = SPELL_SCHOOL_MASK_NORMAL);
        void DeleteThreatList();
        bool IsSecondChoiceTarget(Unit* pTarget, bool checkThreatArea) const;
        bool SelectHostileTarget(bool withEvade = true);
        bool TauntApply(Unit* pVictim, bool isSingleEffect = false);
        void TauntFadeOut(Unit *taunter);

        void FixateTarget(Unit* pVictim);
        ObjectGuid const& GetFixatedTargetGuid() { return m_fixateTargetGuid; };
        Unit* GetFixatedTarget();

        ThreatManager& getThreatManager() { return m_ThreatManager; }
        ThreatManager const& getThreatManager() const { return m_ThreatManager; }
        void addHatedBy(HostileReference* pHostileReference) { m_HostileRefManager->insertFirst(pHostileReference); };
        void removeHatedBy(HostileReference* /*pHostileReference*/ ) { /* nothing to do yet */ }
        HostileRefManager& getHostileRefManager() { return *m_HostileRefManager; }
        void RemoveUnitFromHostileRefManager(Unit* pUnit);

        SpellAuraHolderPtr GetVisibleAura(uint8 slot) const;
        void SetVisibleAura(uint8 slot, SpellAuraHolderPtr holder);
        VisibleAuraMap const& GetVisibleAuras() const { return m_visibleAuras; }
        uint8 GetVisibleAurasCount() const;

        Aura* GetAura(uint32 spellId, SpellEffectIndex effindex);
        Aura* GetAura(AuraType type, SpellFamily family, ClassFamilyMask const& classMask, ObjectGuid casterGuid = ObjectGuid());
        Aura* GetAuraByEffectMask(AuraType type, SpellFamily family, ClassFamilyMask const& classMask, ObjectGuid casterGuid = ObjectGuid());
        Aura* GetAura(AuraType type, SpellFamily family, uint64 familyFlag, uint32 familyFlag2 = 0, ObjectGuid casterGuid = ObjectGuid())
        {
            return GetAura(type, family, ClassFamilyMask(familyFlag, familyFlag2), casterGuid);
        }
        template <AuraType type, SpellFamily family, CFM_ARGS_1>
        Aura* GetAura(ObjectGuid casterGuid = ObjectGuid())
        {
            return GetAura(type, family, ClassFamilyMask::create<CFM_VALUES_1>(), casterGuid);
        }

        Aura* GetScalingAura(AuraType type, uint32 stat = 0);
        Aura const* GetTriggeredByClientAura(uint32 spellId);

        SpellAuraHolderPtr GetSpellAuraHolder(uint32 spellid) const;
        SpellAuraHolderPtr GetSpellAuraHolder(uint32 spellid, ObjectGuid casterGUID) const;

        SpellAuraHolderMap&       GetSpellAuraHolderMap()       { return m_spellAuraHolders; }
        SpellAuraHolderMap const& GetSpellAuraHolderMap() const { return m_spellAuraHolders; }
        AuraList           const& GetAurasByType(AuraType type) const { return m_modAuras[type]; }
        AuraList&                 GetAurasByType(AuraType type) { return m_modAuras[type]; }
        void ApplyAuraProcTriggerDamage(Aura* aura, bool apply);

        int32 GetTotalAuraModifier(AuraType auratype) const;
        float GetTotalAuraMultiplier(AuraType auratype) const;
        int32 GetMaxPositiveAuraModifier(AuraType auratype, bool nonStackingOnly = false) const;
        int32 GetMaxNegativeAuraModifier(AuraType auratype, bool nonStackingOnly = false) const;

        int32 GetTotalAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask) const;
        float GetTotalAuraMultiplierByMiscMask(AuraType auratype, uint32 misc_mask) const;
        int32 GetMaxPositiveAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask, bool nonStackingOnly = false) const;
        int32 GetMaxNegativeAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask, bool nonStackingOnly = false) const;

        int32 GetTotalAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const;
        float GetTotalAuraMultiplierByMiscValue(AuraType auratype, int32 misc_value) const;
        int32 GetMaxPositiveAuraModifierByMiscValue(AuraType auratype, int32 misc_value, bool nonStackingOnly = false) const;
        int32 GetMaxNegativeAuraModifierByMiscValue(AuraType auratype, int32 misc_value, bool nonStackingOnly = false) const;

        // misc have plain value but we check it fit to provided values mask (mask & (1 << (misc-1)))
        float GetTotalAuraMultiplierByMiscValueForMask(AuraType auratype, uint32 mask) const;

        // Calculating custom multipliers (dummy && class script)
        float GetTotalAuraScriptedMultiplierForDamageTaken(SpellEntry const* spellInfo) const;
        float GetTotalAuraScriptedMultiplierForDamageDone(SpellEntry const* spellInfo) const;

        Aura const* GetDummyAura(uint32 spell_id) const;

        uint32 m_AuraFlags;

        uint32 GetDisplayId() const { return GetUInt32Value(UNIT_FIELD_DISPLAYID); }
        void SetDisplayId(uint32 modelId);
        uint32 GetNativeDisplayId() const { return GetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID); }
        void SetNativeDisplayId(uint32 modelId) { SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, modelId); }
        void setTransForm(uint32 spellid) { m_transform = spellid;}
        uint32 getTransForm() const { return m_transform;}

        // at any changes to scale and/or displayId
        void UpdateModelData();

        DynamicObject* GetEffectiveDynObject(uint32 spellId, SpellEffectIndex effIndex, Unit* pTarget);
        DynamicObject* GetDynObject(uint32 spellId, SpellEffectIndex effIndex);
        DynamicObject* GetDynObject(uint32 spellId);
        void AddDynObject(DynamicObject* dynObj);
        void RemoveDynObject(uint32 spellid);
        void RemoveDynObjectWithGuid(ObjectGuid const& guid) { m_dynObjGuids.remove(guid); }
        void RemoveAllDynObjects();

        GameObject* GetGameObject(uint32 spellId) const;
        void AddGameObject(GameObject* gameObj);
        void AddWildGameObject(GameObject* gameObj);
        void RemoveGameObject(GameObject* gameObj, bool del);
        void RemoveGameObject(uint32 spellid, bool del);
        void RemoveAllGameObjects();

        uint32 CalculateDamage(WeaponAttackType attType, bool normalized);
        float GetAPMultiplier(WeaponAttackType attType, bool normalized);
        void ModifyAuraState(AuraState flag, bool apply);
        bool HasAuraState(AuraState flag) const { return HasFlag(UNIT_FIELD_AURASTATE, 1<<(flag-1)); }
        bool HasAuraStateForCaster(AuraState flag, ObjectGuid casterGuid) const;
        void UnsummonAllTotems();
        Unit* SelectMagnetTarget(Unit *victim, Spell* spell = NULL, SpellEffectIndex eff = EFFECT_INDEX_0);

        int32 SpellBonusWithCoeffs(SpellEntry const *spellProto, int32 total, int32 benefit, int32 ap_benefit, DamageEffectType damagetype, bool donePart, float defCoeffMod = 1.0f);
        int32 SpellBaseDamageBonusDone(SpellSchoolMask schoolMask);
        int32 SpellBaseDamageBonusTaken(SpellSchoolMask schoolMask);

        void SpellDamageBonusDone(DamageInfo* damageInfo, uint32 stack = 1);
        void SpellDamageBonusTaken(DamageInfo* damageInfo, uint32 stack = 1);

        int32 SpellBaseHealingBonusDone(SpellSchoolMask schoolMask);
        int32 SpellBaseHealingBonusTaken(SpellSchoolMask schoolMask);
        uint32 SpellHealingBonusDone(Unit *pVictim, SpellEntry const *spellProto, int32 healamount, DamageEffectType damagetype, uint32 stack = 1);
        uint32 SpellHealingBonusTaken(Unit *pCaster, SpellEntry const *spellProto, int32 healamount, DamageEffectType damagetype, uint32 stack = 1);

        void MeleeDamageBonusDone(DamageInfo* damageInfo, uint32 stack = 1);
        void MeleeDamageBonusTaken(DamageInfo* damageInfo, uint32 stack = 1);

        virtual SpellSchoolMask GetMeleeDamageSchoolMask() const;

        bool   IsSpellBlocked(Unit *pCaster, SpellEntry const *spellProto, WeaponAttackType attackType = BASE_ATTACK);
        bool   IsSpellCrit(Unit *pVictim, SpellEntry const *spellProto, SpellSchoolMask schoolMask, WeaponAttackType attackType = BASE_ATTACK);
        uint32 SpellCriticalDamageBonus(SpellEntry const *spellProto, uint32 damage, Unit *pVictim);
        uint32 SpellCriticalHealingBonus(SpellEntry const *spellProto, uint32 damage, Unit *pVictim);

        bool IsTriggeredAtSpellProcEvent(Unit *pVictim, SpellAuraHolderPtr holder, SpellEntry const* procSpell, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, bool isVictim, SpellProcEventEntry const*& spellProcEvent );
        SpellAuraProcResult IsTriggeredAtCustomProcEvent(Unit *pVictim, SpellAuraHolderPtr holder, SpellEntry const* procSpell, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, bool isVictim, SpellProcEventEntry const*& spellProcEvent );
        // Aura proc handlers
        SpellAuraProcResult HandleDummyAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleHasteAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleIncreaseSpeedAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleSpellCritChanceAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleProcTriggerSpellAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleProcTriggerDamageAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleOverrideClassScriptAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleMendingAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModCastingSpeedNotStackAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleReflectSpellsSchoolAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModPowerCostSchoolAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleMechanicImmuneResistanceAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModDamageFromCasterAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleAddFlatModifierAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleAddPctModifierAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModDamagePercentDoneAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandlePeriodicDummyAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModRating(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleRemoveByDamageProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleSpellMagnetAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleManaShieldAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModResistanceAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleRemoveByDamageChanceProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleAuraProcOnPowerAmount(Unit *pVictim, DamageInfo* damage, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleHasteAllProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleSpellAuraOverrideActionbarSpellsProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleModRangedHasteAuraProc(Unit* pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleNULLProc(Unit* /*pVictim*/, DamageInfo* /*damage*/, Aura const* /*triggeredByAura*/, SpellEntry const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
        {
            // no proc handler for this aura type
            return SPELL_AURA_PROC_OK;
        }
        SpellAuraProcResult HandleCantTrigger(Unit* /*pVictim*/, DamageInfo* /*damage*/, Aura const* /*triggeredByAura*/, SpellEntry const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 /*cooldown*/)
        {
            // this aura type can't proc
            return SPELL_AURA_PROC_CANT_TRIGGER;
        }
        SpellAuraProcResult HandleDamageShieldAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleDropChargeByDamageProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        SpellAuraProcResult HandleIgnoreUnitStateAuraProc(Unit *pVictim, DamageInfo* damageInfo, Aura const* triggeredByAura, SpellEntry const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);

        SpellAuraProcResult HandleVengeanceProc(Unit* pVictim, int32 damage, int32 triggerAmount);

        uint32 GetRegenTimer() const { return m_regenTimer; }

        void SetContestedPvP(Player *attackedPlayer = NULL);

        void ApplySpellImmune(uint32 spellId, uint32 op, uint32 type, bool apply);
        void ApplySpellDispelImmunity(const SpellEntry * spellProto, DispelType type, bool apply);
        virtual bool IsImmuneToSpell(SpellEntry const* spellInfo, bool isFriendly) const;
                                                            // redefined in Creature
        bool IsImmunedToDamage(SpellSchoolMask meleeSchoolMask) const;
        bool IsImmunedToSchool(SpellSchoolMask meleeSchoolMask) const;
        virtual bool IsImmuneToSpellEffect(SpellEntry const* spellInfo, SpellEffectIndex index) const;
                                                            // redefined in Creature

        static bool IsDamageReducedByArmor(SpellSchoolMask damageSchoolMask, SpellEntry const* spellProto = NULL, SpellEffectIndex effIndex = MAX_EFFECT_INDEX);
        uint32 CalcArmorReducedDamage(Unit* pVictim, const uint32 damage, SpellEntry const * spellInfo);
        void CalculateResistance(Unit* pCaster, DamageInfo* damageInfo);
        void CalculateDamageAbsorbAndResist(Unit* pCaster, DamageInfo* damageInfo, bool canReflect = false);
        void CalculateAbsorbResistBlock(Unit* pCaster, DamageInfo* damageInfo, SpellEntry const* spellProto, WeaponAttackType attType = BASE_ATTACK);
        void CalculateHealAbsorb(uint32 heal, uint32* absorb);

        void UpdateSpeed(UnitMoveType mtype, bool forced, float ratio = 1.0f, bool ignoreChange = false);
        float GetSpeed(UnitMoveType mtype) const;
        float GetSpeedRate(UnitMoveType mtype) const { return m_speed_rate[mtype]; }
        void SetSpeedRate(UnitMoveType mtype, float rate, bool forced = false, bool ignoreChange = false);

        void KnockBackFrom(Unit* target, float horizontalSpeed, float verticalSpeed);
        void KnockBackWithAngle(float angle, float horizontalSpeed, float verticalSpeed);

        void _RemoveAllAuraMods();
        void _ApplyAllAuraMods();

        int32 CalculateSpellDamage(Unit const* target, SpellEntry const* spellProto, SpellEffectIndex effect_index, int32 const* basePoints = NULL);

        uint32 CalcNotIgnoreAbsorbDamage(DamageInfo* damageInfo);
        uint32 CalcNotIgnoreDamageReduction(DamageInfo* damageInfo);
        int32 CalculateAuraDuration(SpellEntry const* spellProto, uint32 effectMask, int32 duration, Unit const* caster, Spell const* spell = NULL);

        float CalculateLevelPenalty(SpellEntry const* spellProto) const;

        void addFollower(FollowerReference* pRef) { m_FollowingRefManager.insertFirst(pRef); }
        void removeFollower(FollowerReference* /*pRef*/ ) { /* nothing to do yet */ }

        MotionMaster* GetMotionMaster() { return &i_motionMaster; }
        UnitStateMgr& GetUnitStateMgr() { return m_stateMgr; }
        bool IsInUnitState(UnitActionId state) const { return m_stateMgr.GetCurrentState() == state; }

        bool IsStopped() const { return !(hasUnitState(UNIT_STAT_MOVING)); }
        void StopMoving(bool ignoreMoveState = false);
        void InterruptMoving(bool ignoreMoveState = false);

        void SetFeared(bool apply, ObjectGuid casterGuid = ObjectGuid(), uint32 spellID = 0, uint32 time = 0);
        void SetConfused(bool apply, ObjectGuid casterGuid = ObjectGuid(), uint32 spellID = 0);
        void SetFeignDeath(bool apply, ObjectGuid casterGuid = ObjectGuid(), uint32 spellID = 0);

        void AddComboPointHolder(ObjectGuid guid) { m_ComboPointHolders.insert(guid); }
        void RemoveComboPointHolder(ObjectGuid guid) { m_ComboPointHolders.erase(guid); }
        void ClearComboPointHolders();

        uint8 GetComboPoints() const { return m_comboPoints; }
        ObjectGuid const& GetComboTargetGuid() const { return m_comboTargetGuid; }

        void AddComboPoints(Unit* target, int8 count);
        void ClearComboPoints();

        ///----------Pet responses methods-----------------
        void SendPetActionFeedback(uint8 msg);
        void SendPetTalk(uint32 pettalk);
        void SendPetAIReaction();
        ///----------End of Pet responses methods----------
        void DoPetAction(Player* owner, uint8 flag, uint32 spellid, ObjectGuid petGuid, ObjectGuid targetGuid);
        void DoPetCastSpell(Player *owner, uint8 cast_count, SpellCastTargets* targets, SpellEntry const* spellInfo);
        void DoPetCastSpell(Unit* target, uint32 spellId);

        void propagateSpeedChange() { GetMotionMaster()->propagateSpeedChange(); }

        // reactive attacks
        void ClearAllReactives();
        void StartReactiveTimer( ReactiveType reactive ) { m_reactiveTimer[reactive] = REACTIVE_TIMER_START;}
        void UpdateReactives(uint32 p_time);

        // group updates
        void UpdateAuraForGroup(uint8 slot);

        // pet auras
        typedef std::set<PetAura const*> PetAuraSet;
        PetAuraSet m_petAuras;
        void AddPetAura(PetAura const* petSpell);
        void RemovePetAura(PetAura const* petSpell);

        // Frozen Mod
        inline void SetSpoofSamePlayerFaction(bool b) { m_spoofSamePlayerFaction = b; }
        inline bool IsSpoofSamePlayerFaction(void)    { return m_spoofSamePlayerFaction; }
        // Frozen Mod

        // Movement info
        MovementInfo m_movementInfo;
        MovementInfo const& GetMovementInfo() const { return m_movementInfo; };

        // Transports
        uint32 GetTransTime() const { return m_movementInfo.GetTransportTime(); }
        int8 GetTransSeat() const { return m_movementInfo.GetTransportSeat(); }
        bool IsOnTransport() const { return !m_movementInfo.GetTransportGuid().IsEmpty(); }

        // Vehicle system (over-aura operation)
        void EnterVehicle(Unit* base, int8 seatId = -1);
        void EnterVehicle(VehicleKitPtr vehicle, int8 seatId = -1);
        void ExitVehicle(bool forceDismount = false);
        // Vehicle system (direct operation)
        void _EnterVehicle(VehicleKitPtr vehicle, int8 seatId = -1);
        void _ExitVehicle(bool forceDismount = false);

        void EjectVehiclePassenger(Unit* pPassenger);
        void EjectVehiclePassenger(int8 seatId = -1);

        void ChangeSeat(int8 seatId, bool next = true);
        VehicleKitPtr GetVehicle() const { return m_pVehicle; }
        VehicleKitPtr GetVehicleKit() const { return m_pVehicleKit; }
        void RemoveVehicleKit();

        virtual bool IsTransport() const override { return bool(GetVehicleKit()); };
        TransportBase* GetTransportBase() { return (TransportBase*)(&*GetVehicleKit()); };

        VehicleEntry const* GetVehicleInfo() const;
        virtual bool IsVehicle() const override { return GetVehicleInfo() != NULL; }
        void SetVehicleId(uint32 entry);

        void ScheduleAINotify(uint32 delay);
        bool IsAINotifyScheduled() const { return m_AINotifyScheduled;}
        void _SetAINotifyScheduled(bool on) { m_AINotifyScheduled = on;}       // only for call from RelocationNotifyEvent code
        void OnRelocated();

        bool IsLinkingEventTrigger() const { return m_isCreatureLinkingTrigger; }

        bool IsSplineEnabled() const;

        bool IsInWorgenForm(bool inPermanent = false) const;
        bool HasWorgenForm() const;

        bool IsVisionObscured(Unit* target) const;

        // Packet builders
        void BuildForceMoveRootPacket(WorldPacket* data, bool apply, uint32 value);
        void BuildMoveWaterWalkPacket(WorldPacket* data, bool apply, uint32 value);
        void BuildSendPlayVisualPacket(WorldPacket* data, uint32 value, bool impact);
        void BuildMoveSetCanFlyPacket(WorldPacket* data, bool apply, uint32 value);
        void BuildMoveFeatherFallPacket(WorldPacket* data, bool apply, uint32 value);

        // Cooldown System
        static uint32 const infinityCooldownDelay = MONTH;  // used for set "infinity cooldowns" for spells and check
        static uint32 const infinityCooldownDelayCheck = MONTH/2;
        bool HasSpellCooldown(SpellEntry const* spellInfo) const;
        bool HasSpellCooldown(uint32 spellId) const;
        time_t GetSpellCooldownDelay(SpellEntry const* spellInfo) const;
        SpellCooldowns const* GetSpellCooldownMap() const { return &m_spellCooldowns; }

        void RemoveOutdatedSpellCooldowns();

        void AddSpellCooldown(uint32 spell_id, uint32 itemid, time_t end_time);
        void AddSpellAndCategoryCooldowns(SpellEntry const* spellInfo, uint32 itemId = 0, bool infinityCooldown = false );
        void RemoveSpellCooldown(uint32 spell_id, bool update = false);
        void RemoveAllSpellCooldown();
        void RemoveSpellCategoryCooldown(uint32 cat, bool update = false);

        void KillSelf(uint32 keepHealthPoints = 0); // used instead ForcedDespawn() when not need despawn unit

        std::deque<uint32> m_damage_counters[MAX_DAMAGE_COUNTERS];
        int32 m_damage_counter_timer;
        uint32 GetDamageCounterInPastSecs(uint32 secs, int type);

        void RemoveRootsAndSnares(ShapeshiftForm form = FORM_NONE, SpellAuraHolderPtr except = SpellAuraHolderPtr(NULL));

        // hack here
        time_t m_boneShieldCooldown;

        CustomSpellData m_nextCustomSpellData;

        bool wildHuntMarker;
        bool soulburnMarker;

    protected:
        explicit Unit ();

        void _UpdateSpells(uint32 time);
        void _UpdateAutoRepeatSpell();

        uint32 m_attackTimer[MAX_ATTACK];

        float m_createStats[MAX_STATS];

        ObjectGuid m_attackingGuid;

        DeathState m_deathState;

        SpellAuraHolderMap m_spellAuraHolders;
        SpellAuraHolderQueue m_deletedHolders;

        // Store Auras for which the target must be tracked
        TrackedAuraTargetMap m_trackedAuraTargets[MAX_TRACKED_AURA_TYPES];

        SingleCastSpellTargetMap m_singleCastSpellTargets;  // casted by unit single per-caster auras

        GuidList m_dynObjGuids;

        typedef std::list<GameObject*> GameObjectList;
        GameObjectList m_gameObj;
        typedef std::map<uint32, ObjectGuid> WildGameObjectMap;
        WildGameObjectMap m_wildGameObjs;
        bool m_isSorted;
        uint32 m_transform;

        AuraList m_modAuras[TOTAL_AURAS];
        float m_auraModifiersGroup[UNIT_MOD_END][MODIFIER_TYPE_END];
        float m_weaponDamage[MAX_ATTACK][2];
        bool m_canModifyStats;

        //std::list< spellEffectPair > AuraSpells[TOTAL_AURAS];  // TODO: use this if ok for mem
        VisibleAuraMap m_visibleAuras;

        float m_speed_rate[MAX_MOVE_TYPE];

        CharmInfo *m_charmInfo;


        MotionMaster i_motionMaster;

        uint32 m_reactiveTimer[MAX_REACTIVE];
        uint32 m_regenTimer;
        uint32 m_focusRegenTimer;
        uint32 m_holyPowerRegenTimer;
        float m_powerFraction[MAX_STORED_POWERS];

        // Frozen Mod
        bool m_spoofSamePlayerFaction : 1;
        // Frozen Mod

        VehicleKitPtr m_pVehicleKit;
        VehicleKitPtr m_pVehicle;

        void DisableSpline();
        bool m_isCreatureLinkingTrigger;
        bool m_isSpawningLinked;

    private:
        void CleanupDeletedHolders(bool force = false);
        void UpdateSplineMovement(uint32 t_diff);

        // player or player's pet
        float GetCombatRatingReduction(CombatRating cr) const;
        uint32 GetCombatRatingDamageReduction(CombatRating cr, float rate, float cap, uint32 damage) const;

        Unit* _GetTotem(TotemSlot slot) const;              // for templated function without include need
        Pet* _GetPet(ObjectGuid guid) const;                // for templated function without include need

        // Wrapper called by DealDamage when a creature is killed
        void JustKilledCreature(Creature* victim, Player* responsiblePlayer);

        uint32 m_state;                                     // Even derived shouldn't modify
        uint32 m_CombatTimer;

        Spell* m_currentSpells[CURRENT_MAX_SPELL];
        uint32 m_castCounter;                               // count casts chain of triggered spells for prevent infinity cast crashes

        UnitVisibility m_Visibility;
        WorldLocation m_last_notified_position;
        bool m_AINotifyScheduled;

        Diminishing m_Diminishing;
        // Manage all Units threatening us
        ThreatManager m_ThreatManager;
        // Manage all Units that are threatened by us
        HostileRefManager* m_HostileRefManager;

        FollowerRefManager m_FollowingRefManager;

        ComboPointHolderSet m_ComboPointHolders;
        ObjectGuid m_comboTargetGuid;
        int8 m_comboPoints;

        uint32 m_originalFaction;

        GuidSet m_groupPets;
        GuidSet m_guardianPets;

        ObjectGuid m_TotemSlot[MAX_TOTEM_SLOT];
        UnitStateMgr m_stateMgr;

        ObjectGuid m_fixateTargetGuid;                      //< Stores the Guid of a fixated target

        SpellCooldowns m_spellCooldowns;

    private:                                                // Error traps for some wrong args using
        // this will catch and prevent build for any cases when all optional args skipped and instead triggered used non boolean type
        // no bodies expected for this declarations
        template <typename TR>
        void CastSpell(Unit* Victim, uint32 spell, TR triggered);
        template <typename TR>
        void CastSpell(Unit* Victim, SpellEntry const* spell, TR triggered);
        template <typename TR>
        void CastCustomSpell(Unit* Victim, uint32 spell, int32 const* bp0, int32 const* bp1, int32 const* bp2, TR triggered);
        template <typename SP, typename TR>
        void CastCustomSpell(Unit* Victim, SpellEntry const* spell, int32 const* bp0, int32 const* bp1, int32 const* bp2, TR triggered);
        template <typename TR>
        void CastSpell(float x, float y, float z, uint32 spell, TR triggered);
        template <typename TR>
        void CastSpell(float x, float y, float z, SpellEntry const* spell, TR triggered);
        template <typename TR>
        void CastSpell(WorldLocation const& loc, uint32 spell, TR triggered);
        template <typename TR>
        void CastSpell(WorldLocation const& loc, SpellEntry const* spell, TR triggered);
};

template<typename Func>
void Unit::CallForAllControlledUnits(Func const& func, uint32 controlledMask)
{
    if (controlledMask & CONTROLLED_PET)
    {
        if (!m_groupPets.empty())
        {
            GuidSet groupPetsCopy = GetPets();  // Original list may be modified in this function
            for (GuidSet::const_iterator itr = groupPetsCopy.begin(); itr != groupPetsCopy.end(); ++itr)
            {
                if (Pet* pet = _GetPet(*itr))
                    func(pet);
            }
        }
    }

    if (controlledMask & CONTROLLED_MINIPET)
    {
        if (Unit* mini = (Unit*)GetMiniPet())
            func(mini);
    }

    if (controlledMask & CONTROLLED_GUARDIANS)
    {
        for (GuidSet::const_iterator itr = m_guardianPets.begin(); itr != m_guardianPets.end();)
        {
            if (Pet* guardian = _GetPet(*(itr++)))
                func(guardian);
        }
    }

    if (controlledMask & CONTROLLED_CHARM)
    {
        if (Unit* charm = GetCharm())
            func(charm);
    }

    if (controlledMask & CONTROLLED_TOTEMS)
    {
        for (int i = 0; i < MAX_TOTEM_SLOT; ++i)
        {
            if (Unit* totem = _GetTotem(TotemSlot(i)))
                func(totem);
        }
    }
}

template<typename Func>
bool Unit::CheckAllControlledUnits(Func const& func, uint32 controlledMask) const
{
    if (controlledMask & CONTROLLED_PET)
    {
        if (!m_groupPets.empty())
        {
            for (GuidSet::const_iterator itr = m_groupPets.begin(); itr != m_groupPets.end();)
                if (Pet const* pet = _GetPet(*(itr++)))
                    if (func(pet))
                        return true;
        }
    }

    if (controlledMask & CONTROLLED_MINIPET)
    {
        if (Unit* mini = (Unit*)GetMiniPet())
            if (func(mini))
                return true;
    }

    if (controlledMask & CONTROLLED_GUARDIANS)
    {
        for (GuidSet::const_iterator itr = m_guardianPets.begin(); itr != m_guardianPets.end();)
            if (Pet const* guardian = _GetPet(*(itr++)))
                if (func(guardian))
                    return true;
    }

    if (controlledMask & CONTROLLED_CHARM)
    {
        if (Unit const* charm = GetCharm())
            if (func(charm))
                return true;
    }

    if (controlledMask & CONTROLLED_TOTEMS)
    {
        for (int i = 0; i < MAX_TOTEM_SLOT; ++i)
            if (Unit const* totem = _GetTotem(TotemSlot(i)))
                if (func(totem))
                    return true;
    }

    return false;
}

#endif
