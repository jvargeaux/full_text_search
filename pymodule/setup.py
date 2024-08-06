from pathlib import Path

import distutils.sysconfig as dsc
from distutils.sysconfig import get_config_vars as default_get_config_vars
from setuptools import setup, Extension


project_dir = Path(__file__).resolve().parent.parent
source_paths = [
    Path(project_dir, 'src', 'suffix_tree.cpp'),
    Path(project_dir, 'benchmark', 'benchmark.cpp'),
    Path(project_dir, 'pymodule', 'suffixtree.cpp')
]
include_paths = [
    Path(project_dir, 'include'),
    Path(project_dir, 'src'),
]
build_path = Path(project_dir, 'pymodule', 'build')
if not build_path.exists():
    build_path.mkdir(parents=True)


# Override compiler flags
# def prune_flags(item_string):
#     if not item_string.startswith('-'):
#         return item_string
    
#     print(f'\nOld flags:\n{item_string}')
        
#     flags = item_string.split(' ')
#     # flags = [flag for flag in flags if flag != '-g']
#     flags.append('-std=c++17')

#     print(f'\nNew flags:\n{" ".join(flags)}\n')
    
#     return ' '.join(flags)


# def custom_config_vars(*args):
#     result = default_get_config_vars(*args)
#     if isinstance(result, list):
#         result = [prune_flags(item) for item in result]
#     # if isinstance(result, dict):
#     #     for key, value in result.items():
#     #         print(f'{key}:: {value}')
#     return result


def main():
    # dsc.get_config_vars = custom_config_vars

    setup(
        ext_modules=[
            Extension(
                name='suffixtree',
                sources=[str(source_path) for source_path in source_paths],
                include_dirs=[str(include_path) for include_path in include_paths],
                language="c++",
                extra_compile_args=["-W", "-std=c++17"],
            )
        ]
    )

if __name__ == '__main__':
    main()