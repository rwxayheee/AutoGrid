import re
import os
from pathlib import Path

# List of macros to watch for
GENERIC_MACROS = {
    "min", "max", "TRUE", "FALSE", "stop", "constant",
    "lookup", "index", "equal", "sq", "sq_hyp", "random_sign", "CLOCKS_PER_SEC"
}

MACRO_RE = re.compile(r'#define\s+([a-zA-Z_][a-zA-Z0-9_]*)\b')

def scan_file(file_path):
    flagged = []
    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
        for lineno, line in enumerate(f, 1):
            m = MACRO_RE.match(line)
            if m:
                macro = m.group(1)
                if macro in GENERIC_MACROS:
                    flagged.append((lineno, macro, line.strip()))
    return flagged

def scan_project(root_dir):
    all_flags = {}
    for path in Path(root_dir).rglob("*.[ch]*"):
        if path.suffix in [".c", ".cpp", ".h", ".hpp"]:
            flagged = scan_file(path)
            if flagged:
                all_flags[path] = flagged
    return all_flags

def suggest_rename(macro):
    return f"AD4_{macro}"

if __name__ == "__main__":
    root = "."  # or set to your source root
    results = scan_project(root)

    if not results:
        print("✅ No problematic macros found.")
    else:
        print("🚨 Problematic macros found:\n")
        for file, flags in results.items():
            print(f"\n📄 {file}")
            for lineno, macro, line in flags:
                suggestion = suggest_rename(macro)
                print(f"  Line {lineno:>4}: `{line}`  → consider renaming to `{suggestion}`")

        print("\n✨ Tip: Use `#undef` before including <windows.h> or add `#define NOMINMAX` in config.")

