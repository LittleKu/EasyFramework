#!/usr/bin/env python3
import os
import sys
import subprocess
import argparse
from pathlib import Path


def apply_patch(patch_file):
    """Apply a single patch file using git apply."""
    try:
        result = subprocess.run(['git', 'apply', '--check', str(patch_file)],
                                capture_output=True, text=True)
        if result.returncode == 0:
            # If check passes, apply the patch
            subprocess.run(['git', 'apply', str(patch_file)], check=True)
            print(f"Successfully applied patch: {patch_file}")
            return True
        else:
            print(f"Failed to apply patch {patch_file}: {result.stderr}")
            return False
    except subprocess.CalledProcessError as e:
        print(f"Error applying patch {patch_file}: {str(e)}")
        return False


def process_patch_directory(directory):
    """Process all .patch files in the specified directory."""
    directory_path = Path(directory)
    if not directory_path.exists():
        print(f"Error: Directory '{directory}' does not exist.")
        return False

    if not directory_path.is_dir():
        print(f"Error: '{directory}' is not a directory.")
        return False

    patch_files = list(directory_path.glob('*.patch'))
    if not patch_files:
        print(f"No .patch files found in directory: {directory}")
        return False

    success_count = 0
    for patch_file in patch_files:
        if apply_patch(patch_file):
            success_count += 1

    print(
        f"\nSummary: Successfully applied {success_count} out of {len(patch_files)} patches.")
    return success_count > 0


def main():
    parser = argparse.ArgumentParser(
        description='Apply git patches from a specified directory.')
    parser.add_argument(
        'directory', help='Directory containing patch files to apply')
    args = parser.parse_args()

    if not process_patch_directory(args.directory):
        sys.exit(1)


if __name__ == '__main__':
    main()
