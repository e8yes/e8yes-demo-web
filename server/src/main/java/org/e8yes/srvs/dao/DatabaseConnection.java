package org.e8yes.srvs.dao;

import javax.sql.DataSource;
import org.apache.ibatis.datasource.pooled.PooledDataSource;
import org.apache.ibatis.mapping.Environment;
import org.apache.ibatis.session.Configuration;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.ibatis.transaction.jdbc.JdbcTransactionFactory;
import org.e8yes.srvs.dao.mappers.AUserMapper;

/**
 *
 * @author daviswen
 */
public class DatabaseConnection {

        private static SqlSessionFactory fact;
        private static AUserMapper auserMapper;

        private static void
                init() {
                DataSource dataSource = new PooledDataSource("org.postgresql.Driver",
                                                             "jdbc:postgresql://localhost:5432/demoweb",
                                                             "postgres",
                                                             "password");

                Environment environment = new Environment("Development",
                                                          new JdbcTransactionFactory(),
                                                          dataSource);

                Configuration config = new Configuration(environment);
                config.addMapper(AUserMapper.class);

                SqlSessionFactoryBuilder builder = new SqlSessionFactoryBuilder();
                fact = builder.build(config);
        }

        public static SqlSession
                getSession() {
                if (fact == null) {
                        init();
                }
                return fact.openSession();
        }

        public static AUserMapper
                getAUserMapper() {
                if (auserMapper == null) {
                        auserMapper = getSession().getMapper(AUserMapper.class);
                }
                return auserMapper;
        }
}
