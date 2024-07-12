import suffixtree
from pprint import pprint

def main():
    build_string = "greennerdgreen$"
    result = suffixtree.build(build_string=build_string)
    print(result)
    # result = suffixtree.query(query_string="een")
    # print(result)

if __name__ == '__main__':
    main()