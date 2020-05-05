#include <QtTest>

// add necessary includes here

class create_user_test : public QObject
{
    Q_OBJECT

public:
    create_user_test();
    ~create_user_test();

private slots:
    void test_case1();

};

create_user_test::create_user_test()
{

}

create_user_test::~create_user_test()
{

}

void create_user_test::test_case1()
{

}

QTEST_APPLESS_MAIN(create_user_test)

#include "tst_create_user_test.moc"
