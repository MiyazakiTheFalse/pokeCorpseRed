#!/usr/bin/env python3
from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]


@dataclass
class Check:
    section: str
    name: str
    ok: bool
    detail: str


def read(rel: str) -> str:
    return (ROOT / rel).read_text(encoding="utf-8")


def has_all(text: str, patterns: list[str]) -> tuple[bool, list[str]]:
    missing = [p for p in patterns if re.search(p, text, flags=re.S) is None]
    return (len(missing) == 0, missing)


def get_labeled_blocks(text: str) -> dict[str, str]:
    matches = list(re.finditer(r"(?m)^([A-Za-z0-9_]+::)\n", text))
    blocks: dict[str, str] = {}
    for i, m in enumerate(matches):
        start = m.start()
        end = matches[i + 1].start() if i + 1 < len(matches) else len(text)
        blocks[m.group(1)] = text[start:end]
    return blocks


def main() -> int:
    checks: list[Check] = []

    rocketops = read("data/scripts/rocketops.inc")
    start_menu = read("src/start_menu.c")
    field_specials = read("src/field_specials.c")
    avatar = read("src/field_player_avatar.c")
    viridian_gym = read("data/maps/ViridianCity_Gym/scripts.inc")
    silph11 = read("data/maps/SilphCo_11F/scripts.inc")
    shared_warps = read("data/scripts/giovanni_shared_warps.inc")
    viridian_city = read("data/maps/ViridianCity/scripts.inc")
    chapter1_text = read("data/maps/RocketHideout_B4F/text.inc")
    chapter2_text = read("data/maps/SilphCo_11F/text.inc")
    chapter3_text = read("data/maps/ViridianCity_Gym/text.inc")

    # 1) Core systems
    ok, missing = has_all(
        rocketops,
        [
            r"EventScript_RocketOps_Command_SecureRoute::.*VAR_GIO_CHAPTER, 1.*VAR_GIO_CHAPTER, 2.*VAR_GIO_CHAPTER, 3",
            r"EventScript_RocketOps_Command_DeployAgent::.*VAR_GIO_CHAPTER, 1.*VAR_GIO_CHAPTER, 2.*VAR_GIO_CHAPTER, 3",
            r"EventScript_RocketOps_Command_DestroyData::.*VAR_GIO_CHAPTER, 1.*VAR_GIO_CHAPTER, 2.*VAR_GIO_CHAPTER, 3",
            r"EventScript_RocketOps_Command_ExtractStaff::.*VAR_GIO_CHAPTER, 1.*VAR_GIO_CHAPTER, 2.*VAR_GIO_CHAPTER, 3",
            r"specialvar VAR_RESULT, Special_RocketOps_ApplyCommandEffect.*special Special_RocketOps_WritebackState",
        ],
    )
    checks.append(Check("Core systems", "RocketOps command dispatch + writeback", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        field_specials,
        [
            r"bool8 IsGiovanniMemorySaveBlocked\(void\)",
            r"static bool8 IsGiovanniBetweenActsSaveAllowed\(void\)",
            r"snapshot->checkpointSaveAllowed",
        ],
    )
    checks.append(Check("Core systems", "Save toggling data gate", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        start_menu,
        [
            r"StartCB_Save1\(void\)",
            r"IsGiovanniMemorySaveBlocked\(\)",
            r"gText_GiovanniMemorySaveBlocked",
        ],
    )
    checks.append(Check("Core systems", "Save gate wired in start menu", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        avatar + "\n" + field_specials,
        [
            r"IsGiovanniAuthorityPacingForcedRunActive\(\)",
            r"VarSet\(VAR_GIO_AUTHORITY_PACING, enabled\)",
            r"RunGiovanniMemoryModeResetHooks\(u8 chapterId\)",
        ],
    )
    checks.append(Check("Core systems", "Movement authority forced-run control", ok, "" if ok else f"missing={missing}"))

    # 2) Chapters 1-3 playable (static: chapter warps + progression hooks + objective fail messaging)
    ok, missing = has_all(
        shared_warps,
        [
            r"EventScript_GiovanniHub_WarpChapter1::",
            r"EventScript_GiovanniHub_WarpChapter2::",
            r"EventScript_GiovanniHub_WarpChapter3::",
        ],
    )
    checks.append(Check("Chapter progression", "Hub warp endpoints exist for chapters 1-3", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        silph11,
        [
            r"SilphCo_11F_EventScript_Act1_Start::",
            r"SilphCo_11F_EventScript_Act2_Start::",
            r"SilphCo_11F_EventScript_Act3_Start::",
            r"SilphCo_11F_EventScript_Act4_Start::",
            r"SilphCo_11F_EventScript_ActLocked::",
            r"SilphCo_11F_Text_RocketReport_ObjectivesIncomplete",
        ],
    )
    checks.append(Check("Chapter progression", "Chapter 2 full act chain and lock messaging", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        viridian_gym,
        [
            r"ViridianCity_Gym_EventScript_MemorySpaceMarker::",
            r"ViridianCity_Gym_EventScript_GiovanniMemoryTransitionCadence::",
            r"ViridianCity_Gym_EventScript_GiovanniMemoryObjectivesIncomplete::",
            r"ViridianCity_Gym_EventScript_GiovanniStartBattle::",
        ],
    )
    checks.append(Check("Chapter progression", "Chapter 3 objective chain to final battle", ok, "" if ok else f"missing={missing}"))

    # 3) Chapter 2 branch outcomes through chapter 3 and Viridian return.
    ok, missing = has_all(
        viridian_gym,
        [
            r"goto_if_set FLAG_GIO_BRANCH_EXTRACT_STAFF, ViridianCity_Gym_EventScript_MemoryFinalDefenseEncounter_ExtractStaff",
            r"goto_if_set FLAG_GIO_BRANCH_DESTROY_DATA, ViridianCity_Gym_EventScript_MemoryFinalDefenseEncounter_DestroyData",
            r"ViridianCity_Gym_EventScript_MemoryObjectiveLiveBranchDialogue_DestroyData::",
            r"ViridianCity_Gym_EventScript_MemoryObjectiveLiveBranchDialogue_ExtractStaff::",
        ],
    )
    checks.append(Check("Branch continuity", "Both chapter 2 branches propagate into chapter 3 scenes", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        viridian_city,
        [
            r"ViridianCity_EventScript_MemoryExitGuard::",
            r"EventScript_GiovanniSharedWarp_ResolvePrecedence",
            r"ViridianCity_EventScript_MemoryDesyncReturnToHub",
        ],
    )
    checks.append(Check("Branch continuity", "Viridian return guard and fallback warp path", ok, "" if ok else f"missing={missing}"))

    # 4) Disbandment + Red vs Giovanni trigger reliability.
    ok, missing = has_all(
        viridian_gym,
        [
            r"setflag FLAG_ROCKET_DISBANDED",
            r"specialvar VAR_RESULT, SetGiovanniMemoryModeChapter3Complete",
            r"goto ViridianCity_Gym_EventScript_GiovanniStartBattle",
            r"trainerbattle_single TRAINER_LEADER_GIOVANNI",
        ],
    )
    checks.append(Check("Finale triggers", "Disbandment flag and Giovanni battle chain", ok, "" if ok else f"missing={missing}"))

    # 5) softlock/warp/flag-order regression on fail/reset/load paths
    ok, missing = has_all(
        field_specials,
        [
            r"bool8 HandleGiovanniMemoryModeWhiteout\(void\)",
            r"bool8 HandleGiovanniMemoryModeBootstrapOnLoad\(void\)",
            r"AbortGiovanniMemoryMode\(\);\s*ReconcileGiovanniMemoryModeOutcome\(\);",
            r"RunGiovanniMemoryModeResetHooks\(0\)",
        ],
    )
    checks.append(Check("Regression", "Whiteout/load fallback teardown + reconciliation", ok, "" if ok else f"missing={missing}"))

    ok, missing = has_all(
        viridian_gym,
        [
            r"ViridianCity_Gym_EventScript_AbortGiovanniMemoryMode::",
            r"special AbortGiovanniMemoryMode",
            r"special RestoreGiovanniMemoryModeSnapshot",
            r"special ReconcileGiovanniMemoryModeOutcome",
        ],
    )
    checks.append(Check("Regression", "Script abort route clears and reconciles", ok, "" if ok else f"missing={missing}"))


    # 6) Narrative policy checks (chapter dialogue constraints)
    sensitive_labels = [
        "RocketHideout_B4F_Text_MemorySpaceMarker::",
        "RocketHideout_B4F_Text_MemoryChapterIntro::",
        "RocketHideout_B4F_Text_ChapterReveal_InfrastructureControl::",
        "RocketHideout_B4F_Text_GruntReport_ObjectiveLive::",
        "SilphCo_11F_Text_MemorySpaceMarker::",
        "SilphCo_11F_Text_MemoryChapter3EmergencyBriefing::",
        "SilphCo_11F_Text_MemoryChapter3ObjectiveInit::",
        "SilphCo_11F_Text_ChapterReveal_EconomicControl::",
        "SilphCo_11F_Text_GruntReport_ObjectiveLive::",
        "ViridianCity_Gym_Text_MemorySpaceMarker::",
        "ViridianCity_Gym_Text_ChapterReveal_SystemCollapse::",
        "ViridianCity_Gym_Text_GiovanniMemoryBeat_WarReference::",
        "ViridianCity_Gym_Text_GiovanniMemoryBeat_WarFallback::",
    ]
    chapter_text_all = chapter1_text + "\n" + chapter2_text + "\n" + chapter3_text
    marker = "@ NARRATIVE_REVIEW:WAR_SENSITIVE"
    missing_markers = []
    for label in sensitive_labels:
        idx = chapter_text_all.find(label)
        if idx == -1:
            missing_markers.append(f"missing_label:{label}")
            continue
        pre = chapter_text_all[max(0, idx - 200):idx]
        if marker not in pre:
            missing_markers.append(label)
    checks.append(Check(
        "Narrative policy",
        "Sensitive chapter dialogue nodes are review-marked",
        len(missing_markers) == 0,
        "" if len(missing_markers) == 0 else f"missing={missing_markers}",
    ))

    forbidden_scene_tokens = [
        "flashback",
        "on the battlefield",
        "front line",
        "during the war",
        "war began",
    ]
    lower_all = chapter_text_all.lower()
    violations = [tok for tok in forbidden_scene_tokens if tok in lower_all]
    checks.append(Check(
        "Narrative policy",
        "No direct war scenes or flashbacks in chapter text",
        len(violations) == 0,
        "" if len(violations) == 0 else f"found={violations}",
    ))

    allowed_war_context_tokens = ["aftermath", "policy", "memory", "echo", "report", "residue"]
    war_context_issues = []
    for label, block in get_labeled_blocks(chapter_text_all).items():
        if re.search(r"\bwar\b", block, flags=re.I) is None:
            continue
        if all(token not in block.lower() for token in allowed_war_context_tokens):
            war_context_issues.append(label)
    checks.append(Check(
        "Narrative policy",
        "War references are constrained to aftermath/policy/memory-residue framing",
        len(war_context_issues) == 0,
        "" if len(war_context_issues) == 0 else f"labels={war_context_issues}",
    ))

    failures = [c for c in checks if not c.ok]

    print("Giovanni campaign structured validation")
    print("=" * 44)
    current = None
    for c in checks:
        if c.section != current:
            current = c.section
            print(f"\n[{current}]")
        status = "PASS" if c.ok else "FAIL"
        extra = f" ({c.detail})" if c.detail else ""
        print(f"- {status}: {c.name}{extra}")

    print(f"\nSummary: {len(checks)-len(failures)}/{len(checks)} checks passed")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
