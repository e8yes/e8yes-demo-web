#include <QtTest>

#include "sql/connection/pq_connection.h"

class pq_connection_test : public QObject {
    Q_OBJECT

  public:
    pq_connection_test();
    ~pq_connection_test();

  private slots:
    void connection_state_test();
    void update_and_query_test();
};

pq_connection_test::pq_connection_test() {}

pq_connection_test::~pq_connection_test() {}

void pq_connection_test::connection_state_test() {}

void pq_connection_test::update_and_query_test() {}

QTEST_APPLESS_MAIN(pq_connection_test)

#include "tst_pq_connection_test.moc"
