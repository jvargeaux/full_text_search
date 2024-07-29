# Full Text Search

This project aims at comparing algorithms and libraries to find the best options for full text search in a multi-langual environment.

The goal is to have the possibility to search in a PostgreSQL database for queries (ranked by relevance). The requirement is to eventually have a Python API (could use IPC, latency is not an issue).

Interesting options:

* PostgreSQL's Full Text Search (Alphabet only) : https://www.postgresql.org/docs/current/textsearch.html
* Mecab : https://taku910.github.io/mecab
* Groonga : https://groonga.org
* PGronnga : https://pgroonga.github.io
* Kakasi : http://kakasi.namazu.org/index.html.en


Unicode emoji search:
https://github.com/unicode-org/cldr-json/blob/main/cldr-json/cldr-annotations-full/annotations/en/annotations.json


## Python Module

### Build

Install the python module which will trigger build scripts.
```bash
pip3 install ./pymodule
```


### Test

Check the Python module is working.
```bash
python3 pymodule/test.py
```

Test Output:
```bash
0         # result of build(), 0 = no error
[0, 9]    # result of query(), list of matching indices (int)
```


## Python Usage

```python
import suffixtree

# Build
suffixtree.build(build_string="asdf$")

# Query
matches: list[int] = suffixtree.query(query_string="df")
print(matches)

# Output
# [2]
```