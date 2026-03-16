# Giovanni Memory Warp Priority

This document defines the single warp-resolution order for any tile that can be used by both normal play and Giovanni Memory mode.

## Priority rule

For every dual-purpose warp tile, resolve in this order:

1. **Mode gate**: If Giovanni Memory mode is not active, allow the normal warp.
2. **Mode variable gate**: If memory mode is active but `VAR_MODE_GIOVANNI_MEMORY` is not TRUE, route to the chapter hub fallback.
3. **Chapter gate**: If `VAR_CHAPTER_ID` does not match the chapter for this map, route to the chapter hub fallback.
4. **Memory guard behavior**: If mode and chapter are valid, block the exit with a chapter-specific message and warp to the local memory fallback tile.
5. **Normal destination**: Only reached when memory mode is not active.

This script-first order must be implemented on coordinate triggers placed on the same coordinates as the warp tile.

## Audited dual-purpose warp tiles

- `CeladonCity_GameCorner`: `(9,13)`, `(10,13)`, `(11,13)`
- `UndergroundPath_NorthSouthTunnel`: `(4,3)`, `(3,60)`
- `RocketHideout_B4F`: `(11,15)`, `(20,23)`, `(21,23)`
- `SilphCo_11F`: `(7,2)`, `(2,5)`, `(13,3)`
- `ViridianCity_Gym`: `(16,22)`, `(17,22)`, `(18,22)`
- `DiglettsCave_NorthEntrance`: `(4,6)`
- `DiglettsCave_SouthEntrance`: `(4,6)`

## Canonical chapter hubs (validated)

Validated against:
- `include/constants/map_groups.h` canonical map constants.
- `data/maps/<MapName>/` asset directories.

| Chapter | Canonical map constant | Coordinates | Constant exists | Map directory exists |
| --- | --- | --- | --- | --- |
| 1 | `MAP_ROCKET_HIDEOUT_B4F` | `(19, 6)` | Yes | `data/maps/RocketHideout_B4F/` |
| 2 | `MAP_SILPH_CO_11F` | `(6, 14)` | Yes | `data/maps/SilphCo_11F/` |
| 3 | `MAP_VIRIDIAN_CITY_GYM` | `(17, 21)` | Yes | `data/maps/ViridianCity_Gym/` |

## Canonical mapping table (contract name -> map constant)

All contract-facing aliases and shorthand names must resolve to exact constants already used by scripts/C.

| Contract name / alias | Canonical constant |
| --- | --- |
| `ROCKET_HIDEOUT_BACK_ROOM` / `BACK_ROOM` | `MAP_ROCKET_HIDEOUT_B4F` |
| `DIGLETT_CAVE` | `MAP_DIGLETTS_CAVE_NORTH_ENTRANCE` (chapter-1 route anchor) |
| `SILPH_CO_BASEMENT` | `MAP_SILPH_CO_11F` |
| `VIRIDIAN_GYM` | `MAP_VIRIDIAN_CITY_GYM` |

These substitutions align with the Giovanni runtime map constants in `src/field_specials.c` and shared chapter hub warps in `data/scripts/giovanni_shared_warps.inc`.

## Act endpoint primary/fallback policy

Each act endpoint uses a primary coordinate with a fallback coordinate on a canonical map so map edits cannot strand restart/restore flow.

| Chapter/checkpoint band | Primary endpoint | Fallback endpoint |
| --- | --- | --- |
| Chapter 1 (`checkpoint >= 0`) | `MAP_ROCKET_HIDEOUT_B4F (18,6)` | `MAP_ROCKET_HIDEOUT_B4F (19,6)` |
| Chapter 2 (`checkpoint >= 0`) | `MAP_SILPH_CO_11F (6,13)` | `MAP_SILPH_CO_11F (6,14)` |
| Chapter 3 (`checkpoint >= 0`) | `MAP_VIRIDIAN_CITY_GYM (17,20)` | `MAP_VIRIDIAN_CITY_GYM (17,21)` |
| Chapter 3 (`checkpoint >= 1`) | `MAP_VIRIDIAN_CITY_GYM (9,15)` | `MAP_VIRIDIAN_CITY_GYM (17,21)` |
| Chapter 3 (`checkpoint >= 2`) | `MAP_VIRIDIAN_CITY_GYM (11,7)` | `MAP_VIRIDIAN_CITY_GYM (17,21)` |

Runtime selection validates primary coordinates against current target map layout bounds and automatically falls back when invalid.

### Substitutions

- No map-name substitutions were required; all proposed hub maps exist.
- One coordinate normalization was applied for Chapter 3 hub wiring: older script branches used `(17, 20)`, but authoritative canonical hub is `(17, 21)`.

## Deterministic script entrypoints

Use these shared labels as authoritative hub entrypoints:

- `EventScript_GiovanniHub_WarpByChapter`
- `EventScript_GiovanniHub_WarpChapter1`
- `EventScript_GiovanniHub_WarpChapter2`
- `EventScript_GiovanniHub_WarpChapter3`
