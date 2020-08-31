TEMPLATE = subdirs
SUBDIRS = \
    query_runner/query_runner.pro \
    query_runner_test/orm/query_completion_test \
    query_runner_test/orm/data_collection_test \
    query_runner_test/resultset/pq_result_set_test \
    query_runner_test/connection/pq_connection_test \
    query_runner_test/sql_query_builder_test \
    _test_query_runner/_test_sql_runner/_test_sql_runner.pro

CONFIG += ordered
