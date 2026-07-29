#!/usr/bin/env python3
"""Fail when game data paths collide once case is folded.

VFS looks paths up case-insensitively (vfs::Path::Less), so two entries in one
directory whose names differ only in case are a single entry as far as the game
is concerned, and which one wins depends on the order the directory happened to
be read in. NTFS made that unrepresentable, so it never came up; a release
assembled on Linux can produce it, and a Linux checkout can hold it.

Every language overlay is merged onto gamedir the way a release does, and any
pair of paths that folds together is reported. Directories count: a new file
under Data/Mercedt/ collides with Data/MercEdt/ even when no file does.
"""

import subprocess
import sys
from pathlib import PurePosixPath

GAMEDIR = "gamedir"
OVERLAYS = "gamedir-languages"


def tracked(prefix: str) -> list[str]:
    """Paths git tracks under `prefix`, relative to it."""
    listing = subprocess.run(
        ["git", "ls-files", "-z", prefix], capture_output=True, text=True, check=True
    ).stdout
    return [path[len(prefix) + 1 :] for path in listing.split("\0") if path]


def with_parents(paths: list[str]) -> set[str]:
    """Every path plus each directory leading to it."""
    entries = set()
    for path in paths:
        parts = PurePosixPath(path).parts
        entries.update("/".join(parts[:depth]) for depth in range(1, len(parts) + 1))
    return entries


def collisions(paths: list[str]) -> dict[str, set[str]]:
    """Folded path -> the spellings sharing it, for those spelled more than one way."""
    spellings: dict[str, set[str]] = {}
    for entry in with_parents(paths):
        spellings.setdefault(entry.lower(), set()).add(entry)
    return {folded: s for folded, s in spellings.items() if len(s) > 1}


def languages() -> list[str]:
    listing = subprocess.run(
        ["git", "ls-tree", "--name-only", "-d", "HEAD", f"{OVERLAYS}/"],
        capture_output=True,
        text=True,
        check=True,
    ).stdout
    return sorted(line.split("/")[-1] for line in listing.split() if line)


def main() -> int:
    gamedir = tracked(GAMEDIR)
    trees = [(GAMEDIR, gamedir)]
    trees += [
        (overlay, gamedir + tracked(f"{OVERLAYS}/{overlay}"))
        for overlay in languages()
    ]

    failed = False
    for name, paths in trees:
        found = collisions(paths)
        if not found:
            print(f"ok    {name}")
            continue
        failed = True
        print(f"FAIL  {name}: {len(found)} paths spelled more than one way")
        for folded in sorted(found)[:20]:
            print("        " + "  vs  ".join(sorted(found[folded])))
        if len(found) > 20:
            print(f"        ... and {len(found) - 20} more")

    if failed:
        print(
            "\nGame data is looked up case-insensitively. Rename the offending paths"
            "\nso each one is spelled exactly one way across gamedir and the overlay."
        )
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
