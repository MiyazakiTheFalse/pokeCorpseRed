# Giovanni Memory Mode Flag/Var Flow (Concrete Bindings)

This document translates policy-level wording into concrete implementation hooks for save gating, forced run pacing, and exit/reset behavior.

## 1) Save gating path and act-based enable/disable binding

### Start menu path
- Save requests route through `StartCB_Save1` in `src/start_menu.c`.
- `StartCB_Save1` calls `IsGiovanniMemorySaveBlocked()` before entering the normal save dialog path.
  - If blocked, it shows `gText_GiovanniMemorySaveBlocked` and returns to start menu input.
  - If not blocked, it prepares save flow and may show the framing text via `IsGiovanniMemorySaveFramingAllowed()`.

### Exact save-gate binding
- The memory-mode save gate is implemented in `IsGiovanniMemorySaveBlocked()` (`src/field_specials.c`):
  - Hard mode gate: `FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE` must be set.
  - Runtime checkpoint gate: `IsGiovanniBetweenActsSaveAllowed()` must be true.
- `IsGiovanniBetweenActsSaveAllowed()` requires all of the following:
  1. `FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE == TRUE`
  2. `snapshot->checkpointSaveAllowed == TRUE`
  3. Current location map group/num equals checkpoint map group/num
  4. Current location x/y equals checkpoint x/y

### Act-level concrete var/flag used to toggle save allowance
- The concrete allow/deny bit is **`snapshot->checkpointSaveAllowed`** in `gSaveBlock3Ptr->giovanniMemorySnapshot`.
- It is written via `SnapshotGiovanniActCheckpointState(saveAllowedAtCheckpoint)` and therefore by:
  - `SaveGiovanniCheckpointPositionAndStage(..., saveAllowedAtCheckpoint)`
  - `SaveGiovanniCheckpointPositionAndStageAtMap(..., saveAllowedAtCheckpoint)`
- Current act progression (`VAR_GIO_ACT`) is synchronized through `SetGiovanniCampaignProgress(...)` and `WriteGiovanniActCheckpointFromCurrentState()`, but the direct save enable/disable switch is `checkpointSaveAllowed` at the active checkpoint snapshot.

## 2) Running-shoes / movement-speed control path and Giovanni forced-run binding

### Movement speed path
- Overworld non-bike movement goes through `PlayerNotOnBikeMoving` in `src/field_player_avatar.c`.
- Run speed is chosen by the run branch that calls `PlayerRun(...)` / `PlayerRunSlow(...)` and sets `PLAYER_AVATAR_FLAG_DASH`.

### Existing run controls
- Standard run input check: `(heldKeys & B_BUTTON)`.
- Existing global run enable gate: `FlagGet(FLAG_SYS_B_DASH)`.
- Existing terrain/stamina gates: `!IsRunningDisallowed(...)` and `ChaseStamina_CanUseRunStep()`.

### Giovanni forced always-run binding
- The run-branch condition is:
  - `((heldKeys & B_BUTTON) || VarGet(VAR_GIO_AUTHORITY_PACING) == TRUE)`
  - plus the normal run gates above.
- Therefore Giovanni mode forces always-run by setting:
  - **`VAR_GIO_AUTHORITY_PACING = TRUE`**, making run independent of B-button hold.
- `VAR_GIO_AUTHORITY_PACING` is set centrally in `RunGiovanniMemoryModeResetHooks(chapterId)` as:
  - `VarSet(VAR_GIO_AUTHORITY_PACING, chapterId != 0)`

## 3) Reset hooks on all Giovanni exits (save gate + pacing)

## Single reset point
- Both systems reset through `RunGiovanniMemoryModeResetHooks(0)`.
- That call sets:
  - `VAR_MODE_GIOVANNI_MEMORY = FALSE`
  - `VAR_CHAPTER_ID = 0`
  - `VAR_GIO_AUTHORITY_PACING = FALSE`
- Since save gating depends on `FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE`, exits also clear that flag as part of the relevant exit routine.

### Exit path bindings
- **Normal completion exit**
  - Script path (`data/maps/ViridianCity_Gym/scripts.inc`) calls:
    1. `SetGiovanniMemoryModeChapter3Complete`
    2. `RestoreGiovanniMemoryModeSnapshot`
    3. `ReconcileGiovanniMemoryModeOutcome`
  - `RestoreGiovanniMemoryModeSnapshot()` performs core teardown:
    - clears `FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE`
    - calls `RunGiovanniMemoryModeResetHooks(0)`
- **Abort exit**
  - `AbortGiovanniMemoryMode()` calls `RunGiovanniMemoryModeResetHooks(0)` directly.
  - Gym abort script then also calls `RestoreGiovanniMemoryModeSnapshot` + `ReconcileGiovanniMemoryModeOutcome` for full cleanup and state reconciliation.
- **Whiteout exit/recovery**
  - `HandleGiovanniMemoryModeWhiteout()`:
    - If chapter 3 complete, it attempts `RestoreGiovanniMemoryModeSnapshot()` (normal completion-style teardown).
    - If restore fails, it executes `AbortGiovanniMemoryMode()` + `ReconcileGiovanniMemoryModeOutcome()` fallback.
- **Load recovery exit/recovery**
  - `HandleGiovanniMemoryModeBootstrapOnLoad()` mirrors whiteout handling:
    - chapter-3-complete path attempts `RestoreGiovanniMemoryModeSnapshot()`;
    - on failure, runs `AbortGiovanniMemoryMode()` + `ReconcileGiovanniMemoryModeOutcome()`.

Result: all listed exits route through `Restore...` and/or `Abort...` + `Reconcile...`, each of which invokes `RunGiovanniMemoryModeResetHooks(0)` directly or during reconciliation, guaranteeing pacing reset and memory-mode var reset.

## 4) Concrete bindings to use in the flag-flow spec

When writing the policy/flag-flow spec, bind terms to these concrete implementation points:

- **“Giovanni mode active gate”** → `FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE`
- **“Act checkpoint save-enable bit”** → `giovanniMemorySnapshot.checkpointSaveAllowed`
- **“Start menu save gate callback”** → `StartCB_Save1` → `IsGiovanniMemorySaveBlocked()`
- **“Between-acts save eligibility”** → `IsGiovanniBetweenActsSaveAllowed()` (flag + saveAllowed bit + map/x/y match)
- **“Always-run authority bit”** → `VAR_GIO_AUTHORITY_PACING`
- **“Movement speed decision point”** → `PlayerNotOnBikeMoving` run branch condition
- **“Unified reset hook”** → `RunGiovanniMemoryModeResetHooks(0)`
- **“Exit teardown APIs”** → `RestoreGiovanniMemoryModeSnapshot`, `AbortGiovanniMemoryMode`, `ReconcileGiovanniMemoryModeOutcome`, plus whiteout/load wrappers (`HandleGiovanniMemoryModeWhiteout`, `HandleGiovanniMemoryModeBootstrapOnLoad`)
