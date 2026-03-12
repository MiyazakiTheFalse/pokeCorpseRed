# RocketOps Chapter Mechanic Spec (Chapters 1–3)

This spec defines a single reusable **RocketOps** command system that appears in each Giovanni memory chapter with escalating stakes but unchanged interface flow.

## Core player-facing command set

RocketOps commands available to the player:

1. **Secure Route**
2. **Deploy Agent**
3. **Destroy Data**
4. **Extract Staff**

These four commands are always presented in the same order in the RocketOps terminal menu.

---

## Base interaction flow (shared across Chapters 1–3)

Every RocketOps interaction uses the same script pipeline:

1. Player interacts with a RocketOps terminal object/event.
2. `Special_RocketOps_OpenTerminal` validates chapter mode and opens the command menu.
3. Player selects one of the 4 commands.
4. `Special_RocketOps_ValidateCommandContext` checks command eligibility at the current trigger type.
5. On pass: call command script and apply command state deltas.
6. On fail: show command-specific failure branch text and return to menu or close terminal.
7. `Special_RocketOps_WritebackState` commits vars/flags and refreshes map state.

### Shared script interfaces

- `Special_RocketOps_OpenTerminal`
- `Special_RocketOps_ValidateCommandContext`
- `EventScript_RocketOps_Command_SecureRoute`
- `EventScript_RocketOps_Command_DeployAgent`
- `EventScript_RocketOps_Command_DestroyData`
- `EventScript_RocketOps_Command_ExtractStaff`
- `Special_RocketOps_WritebackState`

### Shared state contract

- `VAR_ROCKETOPS_CHAPTER` — current chapter context (1, 2, 3)
- `VAR_ROCKETOPS_ALERT` — operation heat/alert tier
- `VAR_ROCKETOPS_PROGRESS` — chapter objective progress score
- `FLAG_ROCKETOPS_TERMINAL_UNLOCKED`
- `FLAG_ROCKETOPS_COMMAND_COOLDOWN`

Command implementations can set chapter-local flags, but the menu flow and validation hook remain unchanged.

---

## Command definitions (base behavior)

| Command | Trigger location type | Script call | Resulting flags/state changes | Failure branch text/outcome |
|---|---|---|---|---|
| **Secure Route** | Chokepoint/control node (gate room, corridor lock, stair guard tile) | `EventScript_RocketOps_Command_SecureRoute` | Sets `FLAG_ROCKETOPS_ROUTE_SECURED`; `VAR_ROCKETOPS_ALERT -= 1` (min 0); `VAR_ROCKETOPS_PROGRESS += 1` | **Text:** “Route cannot be secured from this terminal.” **Outcome:** no state change; return to command menu. |
| **Deploy Agent** | Surveillance-capable node (camera room, comms relay, office terminal) | `EventScript_RocketOps_Command_DeployAgent` | Sets `FLAG_ROCKETOPS_AGENT_DEPLOYED`; stores target in `VAR_ROCKETOPS_AGENT_TARGET`; `VAR_ROCKETOPS_PROGRESS += 1` | **Text:** “No valid insertion lane available.” **Outcome:** no state change; return to command menu. |
| **Destroy Data** | Archive/data-center node (server room, records terminal, lab storage) | `EventScript_RocketOps_Command_DestroyData` | Sets `FLAG_ROCKETOPS_DATA_DESTROYED`; clears chapter intel flag; `VAR_ROCKETOPS_ALERT += 1`; `VAR_ROCKETOPS_PROGRESS += 2` | **Text:** “Data core is mirrored; purge denied.” **Outcome:** trigger local reinforcement spawn OR lock nearby door for one cycle, then return to menu. |
| **Extract Staff** | Extraction node (roof helipad, loading dock, secure elevator, evac tunnel) | `EventScript_RocketOps_Command_ExtractStaff` | Sets `FLAG_ROCKETOPS_STAFF_EXTRACTED`; grants chapter-safe heal/resupply marker; `VAR_ROCKETOPS_PROGRESS += 2` | **Text:** “Extraction window closed. Hostiles too close.” **Outcome:** `VAR_ROCKETOPS_ALERT += 1`; close terminal interaction. |

---

## Chapter-specific variants (same interface, escalating context)

The command list and flow are unchanged. Only chapter variant handlers and outcomes differ.

## Chapter 1 (Rocket Hideout): onboarding

- Variant suffix: `_C1`
- Goal: teach command-to-location matching with low punishment.

### Variants
- `EventScript_RocketOps_Command_SecureRoute_C1`: unlocks one safe hallway loop.
- `EventScript_RocketOps_Command_DeployAgent_C1`: reveals one hidden switch/objective marker.
- `EventScript_RocketOps_Command_DestroyData_C1`: removes one trainer backup wave.
- `EventScript_RocketOps_Command_ExtractStaff_C1`: opens temporary heal point near hub.

### Failure tuning
- Failures mostly informational; no persistent lockouts.
- `VAR_ROCKETOPS_ALERT` increases are capped to +1 per failed interaction cycle.

## Chapter 2 (Silph Co.): pressure and routing

- Variant suffix: `_C2`
- Goal: require deliberate sequencing across multi-floor objectives.

### Variants
- `EventScript_RocketOps_Command_SecureRoute_C2`: changes one warp/door graph to shorten objective path.
- `EventScript_RocketOps_Command_DeployAgent_C2`: marks hostile patrol path and reduces ambush odds.
- `EventScript_RocketOps_Command_DestroyData_C2`: removes a keycard requirement but raises alert.
- `EventScript_RocketOps_Command_ExtractStaff_C2`: evacuates scientist NPC and grants access code var.

### Failure tuning
- Some failures cause temporary patrol escalation (`VAR_ROCKETOPS_ALERT` spikes).
- Failed `Destroy Data` can trigger one optional miniboss encounter gate.

## Chapter 3 (Viridian Gym): endgame execution

- Variant suffix: `_C3`
- Goal: apply mastered system under high-stakes constraints.

### Variants
- `EventScript_RocketOps_Command_SecureRoute_C3`: seals flanking entry, forcing boss-approach lane.
- `EventScript_RocketOps_Command_DeployAgent_C3`: deploys decoy that delays elite guard activation timer.
- `EventScript_RocketOps_Command_DestroyData_C3`: permanently removes one boss support mechanic.
- `EventScript_RocketOps_Command_ExtractStaff_C3`: extracts hostage/staff unit needed for best chapter resolution flag.

### Failure tuning
- Failures can permanently consume a chapter opportunity flag.
- Failed `Extract Staff` sets a degraded ending bit for chapter resolution.

---

## Implementation rule: stable UI, pluggable chapter handlers

To keep the mechanic reusable, command dispatch must be table-driven:

- Base menu key -> shared validator -> chapter variant script pointer.
- Example dispatch table key: `(chapterId, commandId) -> variantScript`.

This guarantees:

- no chapter-specific menu branches,
- no change to button flow or prompt order,
- chapter complexity added only through variant script behavior and state deltas.

---

## Acceptance checklist

- [ ] RocketOps terminal in Chapters 1–3 presents identical 4-command menu.
- [ ] All 4 commands enforce trigger-type validation before execution.
- [ ] Every command has explicit failure text + outcome branch.
- [ ] Chapter variants alter outcomes only; they do not alter base interface flow.
- [ ] Save/load preserves RocketOps vars/flags without chapter desync.
