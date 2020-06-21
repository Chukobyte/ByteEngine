# byte_logo.py

from sample_projects.pacman.scripts.script_class import ScriptClass


class ByteLogo(ScriptClass):
    def __create__(self):
        # print(f"__create__ = {self}\n")
        pass

    def __destroy__(self):
        print(f"__destroy__ = {self}\n")

    def __delete__(self):
        super().__delete__()
