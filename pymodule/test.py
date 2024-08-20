import json
from pathlib import Path

import suffixtree


project_dir = Path(__file__).resolve().parent.parent


def main():
    build_strings = ["green$", "end$"]
    test_db_path = Path(project_dir, 'data', 'random.json')
    # if test_db_path.exists():
    #     test_data = json.loads(test_db_path.read_text())
    #     build_strings = []
    #     for item in test_data:
    #         build_strings.append(item['title'] + "$")
    #         build_strings.append(item['description'] + "$")
    
    query_strings = ["en", "qwerty"]
    iterations = [10, 1000]

    # print("Building suffix tree...")
    # result: int = suffixtree.build(build_strings=build_strings)
    # print("Suffix tree built.")
    # print(result)
    # matches: list[tuple[int, int]] = suffixtree.query(query_string=query_string)
    # print(matches)

    suffixtree.benchmark(build_strings=build_strings, query_strings=query_strings, iterations=iterations)

if __name__ == '__main__':
    main()