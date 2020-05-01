TEMPLATE = subdirs
SUBDIRS = \
    demoweb/demowebservice.pro \
    demoweb/demowebmain.pro \
    demoweb_test/util/trie_map_test \
    demoweb_test/util/lru_hash_map_test \
    demoweb_test/sql/orm/query_completion_test \
    demoweb_test/sql/orm/data_collection_test \
    demoweb_test/sql/resultset/pq_result_set_test \
    demoweb_test/sql/connection/pq_connection_test \
    demoweb_test/sql/sql_query_builder_test \
    demoweb_test/sql/sql_runner_test

CONFIG += ordered
