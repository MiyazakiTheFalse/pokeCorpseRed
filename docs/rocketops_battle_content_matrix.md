# RocketOps Giovanni Memory Battle Content Matrix (Concrete)

This document converts chapter-level faction intent into implementation-ready battle content.
It defines exact trainer rosters, map/event triggers, Chapter 2 branch deltas, difficulty-equivalence bounds,
and boss-to-completion gate wiring.

## Difficulty-equivalence contract

- **Mandatory trainer encounters per chapter:**
  - Chapter 1: **4** (3 grunts + Giovanni)
  - Chapter 2: **3** baseline + **1 branch encounter** (4 total)
  - Chapter 3: **4** (2 gatekeepers + final defense + Giovanni)
- **Allowed branch variance (Chapter 2):**
  - Encounter count delta between branches: **0** (hard requirement).
  - Total opposing-Pokémon count delta: **<= 1**.
  - Highest enemy level delta: **<= 1**.
  - Aggregate level sum delta: **<= 4**.

---

## Chapter 1 (Rocket Hideout B4F)

### Act 1 — Perimeter suppression

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C1_A1_GRUNT_ALPHA` | `TRAINER_TEAM_ROCKET_GRUNT_18` | Single | `AI_SCRIPT_ROCKET_GRUNT_STANDARD` | DROWZEE Lv29 (`Hypnosis`,`Confusion`,`Headbutt`,`Poison Gas`); MACHOP Lv29 (`Karate Chop`,`Low Kick`,`Focus Energy`,`Leer`) |
| `ROCKETOPS_C1_A1_GRUNT_BRAVO` | `TRAINER_TEAM_ROCKET_GRUNT_17` | Single | `AI_SCRIPT_ROCKET_GRUNT_STANDARD` | KOFFING Lv28 (`Tackle`,`Smog`,`Selfdestruct`,`Sludge`); ZUBAT Lv28 (`Wing Attack`,`Bite`,`Supersonic`,`Confuse Ray`) |

### Act 2 — Barrier axis hold

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C1_A2_GRUNT_CHARLIE` | `TRAINER_TEAM_ROCKET_GRUNT_16` | Single | `AI_SCRIPT_ROCKET_GRUNT_STANDARD` | RATICATE Lv28 (`Hyper Fang`,`Quick Attack`,`Tail Whip`,`Focus Energy`); KOFFING Lv28 (`Smog`,`Tackle`,`Sludge`,`Selfdestruct`) |

### Act 3 — Boss confrontation

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C1_A3_BOSS_GIOVANNI` | `TRAINER_BOSS_GIOVANNI` | Single | `AI_SCRIPT_BOSS_GIOVANNI_C1` | ONIX Lv30 (`Rock Tomb`,`Bind`,`Screech`,`Rock Throw`); RHYHORN Lv32 (`Stomp`,`Fury Attack`,`Scary Face`,`Rock Blast`); KANGASKHAN Lv34 (`Mega Punch`,`Bite`,`Tail Whip`,`Rage`) |

### Chapter 1 map/event trigger bindings

| Encounter ID | Map | Trigger mode | Trigger script/event | Coordinate |
|---|---|---|---|---|
| `ROCKETOPS_C1_A1_GRUNT_ALPHA` | `MAP_ROCKET_HIDEOUT_B4F` | Object sight battle | `RocketHideout_B4F_EventScript_Grunt1` | `(4,2)` |
| `ROCKETOPS_C1_A1_GRUNT_BRAVO` | `MAP_ROCKET_HIDEOUT_B4F` | Object interact/sight battle | `RocketHideout_B4F_EventScript_Grunt3` | `(19,14)` |
| `ROCKETOPS_C1_A2_GRUNT_CHARLIE` | `MAP_ROCKET_HIDEOUT_B4F` | Object interact/sight battle | `RocketHideout_B4F_EventScript_Grunt2` | `(16,14)` |
| `ROCKETOPS_C1_A3_BOSS_GIOVANNI` | `MAP_ROCKET_HIDEOUT_B4F` | Object interaction | `RocketHideout_B4F_EventScript_Giovanni` | `(19,4)` |

---

## Chapter 2 (Silph Co. 11F)

### Act 1 — Penthouse entry

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C2_A1_GRUNT_ALPHA` | `TRAINER_TEAM_ROCKET_GRUNT_40` | Single | `AI_SCRIPT_ROCKET_GRUNT_STANDARD` | CUBONE Lv36 (`Bone Club`,`Headbutt`,`Leer`,`Focus Energy`); MAROWAK Lv38 (`Bone Club`,`Rock Slide`,`Focus Energy`,`Thrash`) |

### Act 2 — Branch fork encounter (branch-conditional)

| Branch | Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|---|
| **Corridor Control Branch** (`Secure Route` / `Hold Position`) | `ROCKETOPS_C2_A2_ADMIN_LOCKDOWN` | `TRAINER_ROCKETOPS_C2_ADMIN_LOCKDOWN` | Single | `AI_SCRIPT_ROCKET_ADMIN_CONTROL` | GOLBAT Lv39 (`Wing Attack`,`Bite`,`Confuse Ray`,`Poison Fang`); WEEZING Lv40 (`Sludge`,`Smokescreen`,`Haze`,`Selfdestruct`) |
| **Data Purge Branch** (`Open Route`) | `ROCKETOPS_C2_A2_ADMIN_PURGE` | `TRAINER_ROCKETOPS_C2_ADMIN_PURGE` | Single | `AI_SCRIPT_ROCKET_ADMIN_AGGRO` | HYPNO Lv39 (`Psychic`,`Headbutt`,`Disable`,`Meditate`); MAGNETON Lv40 (`Shock Wave`,`Supersonic`,`Thunder Wave`,`SonicBoom`) |

### Act 3 — Boss antechamber and boss

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C2_A3_GRUNT_BRAVO` | `TRAINER_TEAM_ROCKET_GRUNT_41` | Single | `AI_SCRIPT_ROCKET_GRUNT_STANDARD` | MACHOKE Lv37 (`Karate Chop`,`Seismic Toss`,`Focus Energy`,`Leer`); DROWZEE Lv37 (`Confusion`,`Headbutt`,`Disable`,`Hypnosis`) |
| `ROCKETOPS_C2_A3_BOSS_GIOVANNI` | `TRAINER_BOSS_GIOVANNI_2` | Single | `AI_SCRIPT_BOSS_GIOVANNI_C2` | NIDORINO Lv37 (`Horn Attack`,`Poison Sting`,`Helping Hand`,`Fury Attack`); RHYHORN Lv37 (`Rock Blast`,`Scary Face`,`Stomp`,`Fury Attack`); KANGASKHAN Lv35 (`Mega Punch`,`Bite`,`Tail Whip`,`Rage`); NIDOQUEEN Lv41 (`Body Slam`,`Bite`,`Earthquake`,`Tail Whip`) |

### Chapter 2 map/event trigger bindings

| Encounter ID | Map | Trigger mode | Trigger script/event | Coordinate |
|---|---|---|---|---|
| `ROCKETOPS_C2_A1_GRUNT_ALPHA` | `MAP_SILPH_CO_11F` | Object sight battle | `SilphCo_11F_EventScript_Grunt1` | `(16,12)` |
| `ROCKETOPS_C2_A2_ADMIN_LOCKDOWN` | `MAP_SILPH_CO_11F` | Coord trigger (door lane) | `SilphCo_11F_EventScript_C2BranchLockdownBattle` | `(9,13)` |
| `ROCKETOPS_C2_A2_ADMIN_PURGE` | `MAP_SILPH_CO_11F` | Coord trigger (server lane) | `SilphCo_11F_EventScript_C2BranchPurgeBattle` | `(4,4)` |
| `ROCKETOPS_C2_A3_GRUNT_BRAVO` | `MAP_SILPH_CO_11F` | Object battle | `SilphCo_11F_EventScript_Grunt2` | `(2,19)` |
| `ROCKETOPS_C2_A3_BOSS_GIOVANNI` | `MAP_SILPH_CO_11F` | Coord trigger to boss scene | `SilphCo_11F_EventScript_GiovanniTriggerLeft/Right` | `(6,2)` / `(7,2)` |

### Chapter 2 branch add/remove matrix

| Branch outcome | Removed encounters | Added encounters | Net encounter count |
|---|---|---|---|
| Corridor Control Branch | Removes `ROCKETOPS_C2_A2_ADMIN_PURGE` | Adds `ROCKETOPS_C2_A2_ADMIN_LOCKDOWN` | 4 mandatory total |
| Data Purge Branch | Removes `ROCKETOPS_C2_A2_ADMIN_LOCKDOWN` | Adds `ROCKETOPS_C2_A2_ADMIN_PURGE` | 4 mandatory total |

Difficulty check (mandatory only): both branches = 2 Pokémon in branch fight, same Lv cap (40), aggregate level totals are equivalent within contract.

---

## Chapter 3 (Viridian Gym)

### Act 1 — East lane gatekeepers

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C3_A1_GATEKEEPER_JASON` | `TRAINER_TAMER_JASON` | Single | `AI_SCRIPT_GYM_TRAINER_MID` | SANDSLASH Lv43 (`Slash`,`Poison Sting`,`Sand Attack`,`Swift`); ARBOK Lv43 (`Bite`,`Glare`,`Screech`,`Acid`) |
| `ROCKETOPS_C3_A1_GATEKEEPER_WARREN` | `TRAINER_COOLTRAINER_WARREN` | Single | `AI_SCRIPT_GYM_TRAINER_MID` | NIDORINA Lv43 (`Body Slam`,`Bite`,`Poison Fang`,`Growl`); NIDORINO Lv43 (`Horn Attack`,`Poison Sting`,`Helping Hand`,`Double Kick`) |

### Act 2 — Final defense gauntlet

| Encounter ID | Trainer constant/type | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C3_A2_FINAL_DEFENSE` | Scripted wild gauntlet (`ViridianCity_Gym_EventScript_MemoryFinalDefenseEncounter`) | 3 sequential singles | `AI_SCRIPT_MEMORY_GAUNTLET_STANDARD` | GOLBAT Lv45 (`Wing Attack`,`Bite`,`Confuse Ray`,`Poison Fang`); MAROWAK Lv46 (`Bone Club`,`Rock Slide`,`Thrash`,`Focus Energy`); HYPNO Lv46 (`Psychic`,`Headbutt`,`Hypnosis`,`Dream Eater`) |

### Act 3 — Boss resolution

| Encounter ID | Trainer constant | Battle type | AI | Party (species / level / exact moves) |
|---|---|---|---|---|
| `ROCKETOPS_C3_A3_BOSS_GIOVANNI` | `TRAINER_LEADER_GIOVANNI` | Single | `AI_SCRIPT_BOSS_GIOVANNI_C3` | RHYHORN Lv45 (`Earthquake`,`Rock Blast`,`Scary Face`,`Megahorn`); DUGTRIO Lv42 (`Earthquake`,`Slash`,`Sand Tomb`,`Aerial Ace`); NIDOQUEEN Lv44 (`Earthquake`,`Body Slam`,`Poison Fang`,`Ice Beam`); NIDOKING Lv45 (`Earthquake`,`Thrash`,`Megahorn`,`Rock Tomb`); RHYDON Lv50 (`Earthquake`,`Rock Slide`,`Megahorn`,`Scary Face`) |

### Chapter 3 map/event trigger bindings

| Encounter ID | Map | Trigger mode | Trigger script/event | Coordinate |
|---|---|---|---|---|
| `ROCKETOPS_C3_A1_GATEKEEPER_JASON` | `MAP_VIRIDIAN_CITY_GYM` | Object battle | `ViridianCity_Gym_EventScript_Jason` | `(10,10)` |
| `ROCKETOPS_C3_A1_GATEKEEPER_WARREN` | `MAP_VIRIDIAN_CITY_GYM` | Object battle | `ViridianCity_Gym_EventScript_Warren` | `(13,7)` |
| `ROCKETOPS_C3_A2_FINAL_DEFENSE` | `MAP_VIRIDIAN_CITY_GYM` | Script branch trigger | `ViridianCity_Gym_EventScript_MemoryFinalDefenseEncounter` | invoked from transition cadence |
| `ROCKETOPS_C3_A3_BOSS_GIOVANNI` | `MAP_VIRIDIAN_CITY_GYM` | Object interaction | `ViridianCity_Gym_EventScript_Giovanni` | `(2,2)` |

---

## Chapter boss completion-gate wiring

Boss roles implemented in scripts:
- Chapter 1: **Tunnel Resistance Leader** (`TRAINER_ROCKETOPS_RANGER_TUNNEL_LEADER`)
- Chapter 2: **Rocket Financial Administrator** (`TRAINER_ROCKETOPS_AGENT_ADMIN` / `TRAINER_ROCKETOPS_TECHNICIAN_ADMIN`, branch-dependent)
- Chapter 3: **Security Taskforce Captain** (`TRAINER_ROCKETOPS_ACE_TASKFORCE_CAPTAIN`)


| Chapter boss | Boss script | Completion gate logic | Required objective condition before completion is written |
|---|---|---|---|
| `ROCKETOPS_C1_A3_BOSS_GIOVANNI` | `RocketHideout_B4F_EventScript_Giovanni` | `RocketHideout_B4F_EventScript_SetMemoryCeladonOutcome` → `CompleteGiovanniMemoryModeChapter1` | `FLAG_ROCKET_SUPPLY_NETWORK_ESTABLISHED` must be set. |
| `ROCKETOPS_C2_A3_BOSS_GIOVANNI` | `SilphCo_11F_EventScript_BattleGiovanni` | `SilphCo_11F_EventScript_SetMemorySaffronOutcome` → `CompleteGiovanniMemoryModeChapter2` | `FLAG_SILPH_INFILTRATION_PREPARED` must be set. |
| `ROCKETOPS_C3_A3_BOSS_GIOVANNI` | `ViridianCity_Gym_EventScript_GiovanniMemoryTransitionCadence` + `ViridianCity_Gym_EventScript_GiovanniStartBattle` | `SetGiovanniMemoryModeChapter3Complete` then restore/reconcile path, then `trainerbattle_single TRAINER_LEADER_GIOVANNI` | Must pass objective bundle checks (`FLAG_ROCKET_DATA_DESTROYED`, `FLAG_ROCKET_EVACUATION_COMPLETE`, `FLAG_GIO_MEM_CH3_ACT4_DECISION_COMPLETE`, escort + final-defense flags, and `VAR_GIO_ACT >= 4`). |



### Required vs optional designation

- All chapter progression encounters listed in this matrix are **Required** and set their corresponding chapter gate flags on victory.
- Optional encounters are outside this matrix and do not set chapter completion-gate flags.

### Branch contract validation

- Chapter 2 branch fights are both mandatory, each with exactly **2 Pokémon**, level cap **40**, and aggregate levels **79** (39+40) per branch, satisfying the contract bounds.
