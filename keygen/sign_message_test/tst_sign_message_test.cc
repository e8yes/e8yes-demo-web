#include <QtTest>
#include <optional>
#include <string>

#include "keygen/key_generator_interface.h"
#include "keygen/persistent_key_generator.h"
#include "keygen/sign_message.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

class sign_message_test : public QObject {
    Q_OBJECT

  public:
    sign_message_test();
    ~sign_message_test();

  private slots:
    void successful_encode_and_decode_message_test();
    void encode_and_decode_disrupted_message_test();
};

sign_message_test::sign_message_test() {}

sign_message_test::~sign_message_test() {}

e8::ConnectionFactory CreateConnectionFactory() {
    e8::ConnectionFactory factory(e8::ConnectionFactory::PQ,
                                  /*host_name=*/"localhost",
                                  /*db_name=*/"demoweb");
    return factory;
}

void sign_message_test::successful_encode_and_decode_message_test() {
    e8::ConnectionFactory fact = CreateConnectionFactory();
    e8::BasicConnectionReservoir db_conns(fact);
    e8::ClearAllTables(&db_conns);

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");
    e8::KeyGeneratorInterface::Key key_set = key_gen.KeyOf(
        "successful_encode_and_decode_message_test", e8::KeyGeneratorInterface::RSA_4096_BITS);

    std::string message({1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3});
    std::string signed_message = e8::SignMessage(message, key_set.key);
    QVERIFY(!signed_message.empty());

    std::optional<std::string> decoded_message =
        e8::DecodeSignedMessage(signed_message, key_set.public_key.value());
    QVERIFY(decoded_message.has_value());
    QVERIFY(decoded_message.value() == message);
}

void sign_message_test::encode_and_decode_disrupted_message_test() {
    e8::ConnectionFactory fact = CreateConnectionFactory();
    e8::BasicConnectionReservoir db_conns(fact);
    e8::ClearAllTables(&db_conns);

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");
    e8::KeyGeneratorInterface::Key key_set = key_gen.KeyOf(
        "encode_and_decode_disrupted_message_test", e8::KeyGeneratorInterface::RSA_4096_BITS);

    std::string message({1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3});
    std::string signed_message = e8::SignMessage(message, key_set.key);
    QVERIFY(!signed_message.empty());

    signed_message[0]++;

    std::optional<std::string> decoded_message =
        e8::DecodeSignedMessage(signed_message, key_set.public_key.value());
    QVERIFY(!decoded_message.has_value());
}

QTEST_APPLESS_MAIN(sign_message_test)

#include "tst_sign_message_test.moc"
