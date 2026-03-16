# Giovanni Campaign Stability Validation Pass

This pass validates script/state wiring for:

1. RocketOps, branching, save toggling, movement authority.
2. Chapter 1–3 progression continuity and no dead-end objective states.
3. Chapter 2 branch continuity through Chapter 3 and Viridian return guards.
4. Disbandment and Giovanni battle trigger chain reliability.
5. Softlock/warp/flag-order regression coverage on fail/reset/load paths.

## Execution

```bash
python3 tools/validate_giovanni_campaign_stability.py
```

## Result

- **PASS (12/12 checks)**

## Coverage Summary

- **Core systems**
  - RocketOps command dispatch is chapter-aware for all 4 commands and writes back state.
  - Save blocking path is wired from `StartCB_Save1` into Giovanni memory save gating with checkpoint eligibility.
  - Movement authority forced-run path is wired via `VAR_GIO_AUTHORITY_PACING` and reset hooks.

- **Narrative completion (Chapters 1–3)**
  - Chapter hub warp endpoints exist for all chapter targets.
  - Chapter 2 act chain includes lock-message fallback instead of dead-end failure.
  - Chapter 3 objective cadence gates progression and routes to Giovanni battle when complete.

- **Branch continuity**
  - Both Chapter 2 branch outcomes (`DESTROY_DATA` and `EXTRACT_STAFF`) are consumed in Chapter 3 defense/dialogue logic.
  - Viridian return path includes shared warp precedence resolution and desync fallback.

- **Finale triggers**
  - Rocket disbandment flag is set before chapter completion/restore chain.
  - The chain proceeds into the Giovanni leader battle script trigger.

- **Regression (fail/reset/load)**
  - Whiteout and bootstrap-on-load both include restore-or-abort+reconcile fallback logic.
  - Script abort path invokes abort + restore + reconcile for cleanup ordering.

## Narrative QA acceptance criteria

- [ ] New mission dialogue depicts war-linked content only as aftermath, policy response, or memory residue.
- [ ] No new dialogue stages direct war scenes or flashbacks.
- [ ] Sensitive chapter dialogue nodes are tagged with `@ NARRATIVE_REVIEW:WAR_SENSITIVE` in chapter `text.inc` files.
- [ ] Structured validation includes narrative-policy checks for chapter text content.
