from subprocess import run
from pathlib import Path
import os
from rich.console import Console
from rich.markup import escape
from builtins import print as builtin_print
import shutil

if __name__ == "__main__":
    console = Console(emoji=False)

    def print(msg):
        console.print(msg)

    here = Path(__file__).parent.absolute()
    for subitem in here.iterdir():
        if not subitem.is_file() and "-" in subitem.name:
            print(f"Building {subitem.name}")

            sub_projects = [Path(i).parent for i in subitem.rglob("Makefile")]
            for project in sub_projects:
                print(f"\t- Building {project.name}")
                result = run("make -r -j --output-sync=target --no-print-directory", cwd=project, shell=True, capture_output=True)
                if result.returncode != 0:
                    print("[red]Failed![/red]\n")
                    print(escape(result.stderr.decode()))
                    exit(result.returncode)
                

            print("Packaging...")
            build_folder = Path(here) / "build"
            dest = build_folder / Path(f"{subitem.name}/build")
            dest.mkdir(exist_ok=True, parents=True)

            for project in sub_projects:
                for i in (project / "build").iterdir():
                    if i.is_file() and i.suffix == ".bin" and "_app" not in i.name:
                        print(f"Copying {i} to {dest}")
                        shutil.copy(i, dest)