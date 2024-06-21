# Full Text Search

This project aims at comparing algorithms and libraries to find the best options for full text search in a multi-langual environment.

The goal is to have the possibility to search in a PostgreSQL database for queries (ranked by relevance). The requirement is to eventually have a Python API (could use IPC, latency is not an issue).

Interesting options:

* PostgreSQL's Full Text Search (Alphabet only) : https://www.postgresql.org/docs/current/textsearch.html
* Mecab : https://taku910.github.io/mecab
* Groonga : https://groonga.org
* PGronnga : https://pgroonga.github.io
* Kakasi : http://kakasi.namazu.org/index.html.en
