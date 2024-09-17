import json
from pathlib import Path

import suffixtree


project_dir = Path(__file__).resolve().parent.parent
build_strings = []


def load_data() -> list[str]:
    test_db_path = Path(project_dir, 'data', 'random.json')
    if test_db_path.exists():
        test_data = json.loads(test_db_path.read_text())
        build_strings = []
        for item in test_data:
            build_strings.append(item['title'] + "$")
            build_strings.append(item['description'] + "$")
    return build_strings


def benchmark():
    build_strings = ["green$", "end$"]
    query_strings = ["en", "qwerty"]
    iterations = [10, 1000]

    print("Building suffix tree...")
    result: int = suffixtree.build(build_strings=build_strings)
    print("Suffix tree built.")
    print(result)

    suffixtree.benchmark(build_strings=build_strings, query_strings=query_strings, iterations=iterations)


def query():
    build_strings = load_data()
    query_string = "local"
    print(f"\nUsing test db. Size: {len(build_strings)}\n")

    suffixtree.build(build_strings=build_strings)
    matches = suffixtree.query(query_string=query_string)

    print(f'Query: \"{query_string}\"')
    print("RESULTS")
    for match in matches:
        string_id, offset, length = match
        print(f'\"{build_strings[string_id][offset:(offset + length)]}\": {match}')


if __name__ == '__main__':
    query()