from pathlib import Path

import distutils.sysconfig as dsc
from distutils.sysconfig import get_config_vars as default_get_config_vars
from setuptools import setup, Extension
from setuptools.command.build import build
from setuptools.command.build_ext import build_ext


project_dir = Path(Path(__file__).resolve().parent.parent.parent)
source_paths = [
    Path(project_dir, 'src', 'suffix_tree.cpp'),
    Path(project_dir, 'pymodule', 'setup', 'suffixtreemodule.cpp')
]
include_paths = [
    Path(project_dir, 'include'),
    Path(project_dir, 'src'),
]
build_path = Path(project_dir, 'pymodule', 'build')


# Override compiler flags
def prune_flags(item_string):
    if not item_string.startswith('-'):
        return item_string
    
    print(f'\nOld flags:\n{item_string}')
        
    flags = item_string.split(' ')
    # flags = [flag for flag in flags if flag != '-g']
    flags.append('-std=c++17')

    print(f'\nNew flags:\n{" ".join(flags)}\n')
    
    return ' '.join(flags)


def custom_config_vars(*args):
    result = default_get_config_vars(*args)
    if isinstance(result, list):
        result = [prune_flags(item) for item in result]
    # if isinstance(result, dict):
    #     for key, value in result.items():
    #         print(f'{key}:: {value}')
    return result


# Override build command
class BuildCommand(build):
    def initialize_options(self):
        build.initialize_options(self)
        self.build_base = str(build_path)


# Override build_ext command
class BuildExtCommand(build_ext):
    def initialize_options(self):
        build_ext.initialize_options(self)
        self.build_base = str(build_path)


def main():
    dsc.get_config_vars = custom_config_vars

    setup(
        ext_modules=[
            Extension(
                name='suffixtreemodule',
                sources=[str(source_path) for source_path in source_paths],
                include_dirs=[str(include_path) for include_path in include_paths],
                language="c++",
                extra_compile_args=["-W"],
            )
        ],
        cmdclass={
            'build': BuildCommand,
            'build_ext': BuildExtCommand,
        }
    )

if __name__ == '__main__':
    main()