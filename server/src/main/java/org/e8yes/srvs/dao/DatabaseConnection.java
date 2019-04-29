package org.e8yes.srvs.dao;

import java.security.AccessControlException;
import java.sql.SQLException;
import java.util.List;
import javax.sql.DataSource;
import org.apache.ibatis.datasource.pooled.PooledDataSource;
import org.apache.ibatis.mapping.Environment;
import org.apache.ibatis.session.Configuration;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.ibatis.transaction.jdbc.JdbcTransactionFactory;
import org.e8yes.srvs.EnvironmentContext;
import org.e8yes.srvs.dao.mappers.AUserGroupMapper;
import org.e8yes.srvs.dao.mappers.AUserMapper;
import org.e8yes.srvs.dao.mappers.DatabaseMapper;

/**
 *
 * @author daviswen
 */
public class DatabaseConnection {

        private static SqlSessionFactory fact;
        private static EnvironmentContext env_ctx;

        public static void
                init(EnvironmentContext ctx) {
                env_ctx = ctx;

                DataSource dataSource;
                switch (env_ctx.mode) {
                        case Prod: {
                                dataSource = new PooledDataSource("org.postgresql.Driver",
                                                                  "jdbc:postgresql://localhost:5432/demoweb",
                                                                  "postgres",
                                                                  "password");
                                break;
                        }
                        default:
                        case Test: {
                                dataSource = new PooledDataSource("org.postgresql.Driver",
                                                                  "jdbc:postgresql://localhost:5432/demowebtest",
                                                                  "postgres",
                                                                  "password");
                                break;
                        }
                }

                Environment environment = new Environment(env_ctx.mode.toString(),
                                                          new JdbcTransactionFactory(),
                                                          dataSource);

                Configuration config = new Configuration(environment);
                config.addMapper(DatabaseMapper.class);
                config.addMapper(AUserMapper.class);
                config.addMapper(AUserGroupMapper.class);

                SqlSessionFactoryBuilder builder = new SqlSessionFactoryBuilder();
                fact = builder.build(config);
        }

        public static SqlSession
                openSession() {
                return fact.openSession();
        }

        public static void
                closeSession(SqlSession sess) {
                sess.close();
        }

        public static void
                deleteAllData() throws SQLException {
                if (env_ctx.mode == EnvironmentContext.Mode.Test) {
                        SqlSession sess = openSession();
                        DatabaseMapper mapper = sess.getMapper(DatabaseMapper.class);
                        List<String> tbNames = mapper.getAllTableNames();
                        for (String tbName : tbNames) {
                                sess.getConnection()
                                        .createStatement()
                                        .executeUpdate("DELETE FROM " + tbName + " CASCADE ");
                        }
                        sess.commit(true);
                } else {
                        throw new AccessControlException("Can do this in non-testing environment.");
                }
        }
}
