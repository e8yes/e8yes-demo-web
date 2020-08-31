TEMPLATE = subdirs
SUBDIRS = \
    query_runner/query_runner.pro \
    _test_query_runner/_test_orm/_test_query_completion/_test_query_completion.pro \
    _test_query_runner/_test_orm/_test_data_collection/_test_data_collection.pro \
    _test_query_runner/_test_connection/_test_pq_connection/_test_pq_connection.pro \
    _test_query_runner/_test_resultset/_test_pq_result_set/_test_pq_result_set.pro \
    _test_query_runner/_test_sql_runner/_test_sql_runner.pro \
    _test_query_runner/_test_sql_query_builder/_test_sql_query_builder.pro

CONFIG += ordered
