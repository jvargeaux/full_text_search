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


def results_by_query(results: dict, query: list[str]) -> dict:
    return [value for key, value in results.items() if key in query]


def miss_results():
    pass


def average(_list: list) -> float:
    return sum(_list) / len(_list) if len(_list) != 0 else 0


def main():
    # Load data
    data = [
        # {
        #     'path': Path(project_dir, 'data', 'random.json'),
        #     'keys': ['title', 'description'],
        #     'query_hit': ['urna'],
        #     'query_miss': [],
        # },
        {
            'path': Path(project_dir, 'data', 'tiny.json'),
            'keys': ['title', 'description'],
            'query_hit': ['is', 'video'],
            'query_miss': ['qwerty', 'xyz']
        },
        {
            'path': Path(project_dir, 'data', 'small.json'),
            'keys': ['title', 'description'],
            'query_hit': ['ipsum', 'integer'],
            'query_miss': ['qwerty', 'xyz']
        },
        {
            'path': Path(project_dir, 'data', 'medium.json'),
            'keys': ['title', 'description'],
            'query_hit': ['ipsum', 'integer'],
            'query_miss': ['qwerty', 'xyz']
        },
        {
            'path': Path(project_dir, 'data', 'large.json'),
            'keys': ['title', 'description'],
            'query_hit': ['ipsum', 'integer'],
            'query_miss': ['qwerty', 'xyz']
        },
        {
            'path': Path(project_dir, 'data', 'massive.json'),
            'keys': ['title', 'description'],
            'query_hit': ['ipsum', 'integer'],
            'query_miss': ['qwerty', 'xyz']
        }
    ]
    iterations = [10]
    for tree in data:
        try:
            tree['build_strings'] = load_build_strings(data_path=tree['path'], keys=tree['keys'])
            tree['size'] = sum([len(build_string) - 1 for build_string in tree['build_strings']])
        except IOError:
            print(f'\nFile \"{tree["path"]}\" not found. Exiting.\n')
            return
        tree['results'] = suffixtree.benchmark(build_strings=tree['build_strings'],
                                               query_strings=tree['query_hit'] + tree['query_miss'],
                                               iterations=iterations)

    # Prepare data
    tree_sizes = [tree['size'] for tree in data]
    last_iteration = iterations[-1]
    raw_hit = [average(results_by_query(results=tree['results']['raw_sequential'][last_iteration], query=tree['query_hit'])) for tree in data]
    raw_miss = [average(results_by_query(results=tree['results']['raw_sequential'][last_iteration], query=tree['query_miss'])) for tree in data]
    suffix_tree_hit = [average(results_by_query(results=tree['results']['suffix_tree'][last_iteration], query=tree['query_hit'])) for tree in data]
    suffix_tree_miss = [average(results_by_query(results=tree['results']['suffix_tree'][last_iteration], query=tree['query_miss'])) for tree in data]

    # Plot results
    RAW_HIT_COLOR = '#0e3ec3'
    RAW_MISS_COLOR = '#aabffb'
    SUFFIX_TREE_HIT_COLOR = '#03b258'
    SUFFIX_TREE_MISS_COLOR = '#8cebba'
    fig, ax = plt.subplots()
    ax.plot(tree_sizes, raw_hit, label=f'Raw Sequential HIT x {last_iteration}', color=RAW_HIT_COLOR)
    ax.plot(tree_sizes, raw_miss, label=f'Raw Sequential MISS x {last_iteration}', color=RAW_MISS_COLOR)
    ax.plot(tree_sizes, suffix_tree_hit, label=f'Suffix Tree HIT x {last_iteration}', color=SUFFIX_TREE_HIT_COLOR)
    ax.plot(tree_sizes, suffix_tree_miss, label=f'Suffix Tree MISS x {last_iteration}', color=SUFFIX_TREE_MISS_COLOR)
    ax.legend()
    ax.scatter(tree_sizes, raw_hit, label=f'Raw Sequential HIT x {last_iteration}', color=RAW_HIT_COLOR)
    ax.scatter(tree_sizes, raw_miss, label=f'Raw Sequential MISS x {last_iteration}', color=RAW_MISS_COLOR)
    ax.scatter(tree_sizes, suffix_tree_hit, label=f'Suffix Tree HIT x {last_iteration}', color=SUFFIX_TREE_HIT_COLOR)
    ax.scatter(tree_sizes, suffix_tree_miss, label=f'Suffix Tree MISS x {last_iteration}', color=SUFFIX_TREE_MISS_COLOR)
    ax.set_xscale('log')
    # ax.set_yscale('log')
    ax.set_xlabel('Size of Original Text (characters)')
    ax.set_ylabel('Time (ms)')
    plt.show()


if __name__ == '__main__':
    main()