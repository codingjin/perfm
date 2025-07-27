#!/usr/bin/env python3
import argparse
from pathlib import Path
import pandas as pd
import sys


def find_common_csvs(base_dir: Path):
    """Return the set of .csv filenames that exist in *all* first-level subdirs."""
    subdirs = [d for d in base_dir.iterdir() if d.is_dir()]
    if not subdirs:
        print(f"No subfolders in {base_dir}", file=sys.stderr)
        sys.exit(1)

    # gather sets of csv names in each subdir
    csv_sets = []
    for d in subdirs:
        names = {p.name for p in d.glob("*.csv")}
        csv_sets.append(names)

    # only keep files present in every subdir
    common = set.intersection(*csv_sets)
    return sorted(common), subdirs

def merge_csvs_for_file(filename: str, subdirs: list[Path], out_dir: Path):
    """Read filename from each subdir, concat, and write to out_dir/filename."""
    dfs = []
    for d in subdirs:
        path = d / filename
        if path.exists():
            df = pd.read_csv(path)
            # tag rows if you want to remember origin:
            #df["source_folder"] = d.name
            dfs.append(df)
        else:
            # Shouldn't happen if we filtered common, but just in case:
            print(f"Warning: {path} not found", file=sys.stderr)

    if not dfs:
        return

    # concatenate along rows
    merged = pd.concat(dfs, ignore_index=True)
    out_path = out_dir / filename
    merged.to_csv(out_path, index=False)
    print(f"→ Wrote merged file: {out_path}")


def main():
    parser = argparse.ArgumentParser(
        description="Merge identical CSV filenames across subfolders."
    )
    parser.add_argument(
        "--modelname", 
        required=True, 
        help="Model name (e.g. llama3)"
    )
    args = parser.parse_args()
    basename = args.modelname

    common_csvs, subdirs = find_common_csvs(Path(basename))
    if not common_csvs:
        print("No common CSV files found across all subfolders.", file=sys.stderr)
        sys.exit(1)
    print(f"Found {len(common_csvs)} common CSV(s): {common_csvs}\nMerging…")
    for fname in common_csvs:
        merge_csvs_for_file(fname, subdirs, Path(basename))

if __name__ == "__main__":
    main()






