import suffixtree

def main():
    build_strings = ["green$", "end$"]

    result: int = suffixtree.build(build_strings=build_strings)
    print(result)
    matches: list[tuple[int, int]] = suffixtree.query(query_string="en")
    print(matches)

if __name__ == '__main__':
    main()