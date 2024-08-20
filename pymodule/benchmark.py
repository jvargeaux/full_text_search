import json
from pathlib import Path

import matplotlib.pyplot as plt
import suffixtree


project_dir = Path(__file__).resolve().parent.parent


def pretty_print(_object: any) -> None:
    def _format(_object: any, level: int = 0) -> any:
        if isinstance(_object, dict):
            formatted = ''
            for key, value in _object.items():
                formatted += f'\n{chr(9) * level}{key}: {_format(value, level + 1)}'
            return formatted
        else:
            return f'{_object}'
    print(_format(_object)[1:])  # Delete first \n character


def load_build_strings(data_path: Path, keys: list[str]) -> list[str]:
    test_data = json.loads(data_path.read_text())
    build_strings = []
    for item in test_data:
        for key in keys:
            build_strings.append(item[key] + "$")
    return build_strings


def main():
    # Config
    iterations = [10, 1000]
    trees = {}


    # Tiny suffix tree
    build_strings = ["green$", "end$"]
    query_strings = ["en", "qwerty"]

    trees['tiny'] = {
        'build_size': sum([len(build_string) - 1 for build_string in build_strings]),
        'results': suffixtree.benchmark(build_strings=build_strings, query_strings=query_strings, iterations=iterations)
    }
    pretty_print(trees['tiny'])


    # Medium suffix tree
    try:
        build_strings = load_build_strings(data_path=Path(project_dir, 'data', 'sample.json'), keys=['title', 'description'])
    except IOError:
        print('File not found. Exiting.')
        return
    query_strings = ["video", "qwerty"]

    trees['medium'] = {
        'build_size': sum([len(build_string) - 1 for build_string in build_strings]),
        'results': suffixtree.benchmark(build_strings=build_strings, query_strings=query_strings, iterations=iterations)
    }
    pretty_print(trees['medium'])


    # Large suffix tree
    try:
        build_strings = load_build_strings(data_path=Path(project_dir, 'data', 'random.json'), keys=['title', 'description'])
    except IOError:
        print('File not found. Exiting.')
        return
    query_strings = ["vestibulum", "qwerty"]

    trees['large'] = {
        'build_size': sum([len(build_string) - 1 for build_string in build_strings]),
        'results': suffixtree.benchmark(build_strings=build_strings, query_strings=query_strings, iterations=iterations)
    }
    pretty_print(trees['large'])


    # Prepare data
    last_iteration = iterations[-1]
    hit_raw = [
        trees['tiny']['results']['raw_sequential'][last_iteration]['en'],
        trees['medium']['results']['raw_sequential'][last_iteration]['video'],
        trees['large']['results']['raw_sequential'][last_iteration]['vestibulum'],
    ]
    hit_suffixtree = [
        trees['tiny']['results']['suffix_tree'][last_iteration]['en'],
        trees['medium']['results']['suffix_tree'][last_iteration]['video'],
        trees['large']['results']['suffix_tree'][last_iteration]['vestibulum'],
    ]
    miss_raw = [
        trees['tiny']['results']['raw_sequential'][last_iteration]['qwerty'],
        trees['medium']['results']['raw_sequential'][last_iteration]['qwerty'],
        trees['large']['results']['raw_sequential'][last_iteration]['qwerty'],
    ]
    miss_suffixtree = [
        trees['tiny']['results']['suffix_tree'][last_iteration]['qwerty'],
        trees['medium']['results']['suffix_tree'][last_iteration]['qwerty'],
        trees['large']['results']['suffix_tree'][last_iteration]['qwerty'],
    ]

    # hit_query = 'en'
    # for tree in trees.values():
    #     hit_raw.append(tree['results']['raw_sequential'][last_iteration][hit_query])
    #     hit_suffixtree.append(tree['results']['suffix_tree'][last_iteration][hit_query])

    # miss_query = 'qwerty'
    # for tree in trees.values():
    #     miss_raw.append(tree['results']['raw_sequential'][last_iteration][miss_query])
    #     miss_suffixtree.append(tree['results']['suffix_tree'][last_iteration][miss_query])


    # Plot results
    fig, ax = plt.subplots()
    build_sizes = [tree['build_size'] for tree in trees.values()]
    ax.plot(build_sizes, hit_raw, label=f'Raw Sequential HIT x {last_iteration}', color='blue')
    ax.plot(build_sizes, miss_raw, label=f'Raw Sequential MISS x {last_iteration}', color='orange')
    ax.plot(build_sizes, hit_suffixtree, label=f'Suffix Tree HIT x {last_iteration}', color='green')
    ax.plot(build_sizes, miss_suffixtree, label=f'Suffix Tree MISS x {last_iteration}', color='red')
    ax.set_xlabel('Size of Original Text (characters)')
    ax.set_ylabel('Time (ms)')
    ax.legend()
    plt.show()


if __name__ == '__main__':
    main()