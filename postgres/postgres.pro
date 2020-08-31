TEMPLATE = subdirs
SUBDIRS = \
    query_runner/query_runner.pro \
    query_runner_test/orm/query_completion_test \
    query_runner_test/orm/data_collection_test \
    _test_query_runner/_test_connection/_test_pq_connection/_test_pq_connection.pro \
    _test_query_runner/_test_resultset/_test_pq_result_set/_test_pq_result_set.pro \
    _test_query_runner/_test_sql_runner/_test_sql_runner.pro \
    _test_query_runner/_test_sql_query_builder/_test_sql_query_builder.pro

CONFIG += ordered
