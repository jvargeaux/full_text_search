import suffixtree
from pprint import pprint

def main():
    build_string = "greennerdgreen$"

    result: int = suffixtree.build(build_string="greennerdgreen$")
    print(result)
    matches: list[int] = suffixtree.query(query_string="gr")
    print(matches)

if __name__ == '__main__':
    main()