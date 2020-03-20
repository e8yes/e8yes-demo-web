package org.e8yes.connection;

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
import org.e8yes.environment.DatabaseInfo;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.service.identity.dao.mappers.DatabaseMapper;
import org.e8yes.sql.connection.BasicConnectionReservoir;
import org.e8yes.sql.connection.ConnectionFactory;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** */
public class DatabaseConnection {

  private static SqlSessionFactory fact;
  private static EnvironmentContext envCtx;
  private static ConnectionReservoirInterface demowebDbConnections;

  public static void init(EnvironmentContext ctx) {
    envCtx = ctx;

    DatabaseInfo info = DatabaseInfo.get(ctx);
    ConnectionFactory factory =
        new ConnectionFactory(
            ConnectionFactory.ConnectionType.JDBC,
            info.hostName,
            info.port,
            info.dbName,
            info.userName,
            info.password);
    demowebDbConnections = new BasicConnectionReservoir(factory);

    DataSource dataSource;
    switch (envCtx.mode) {
      case Prod:
        {
          dataSource =
              new PooledDataSource(
                  "org.postgresql.Driver",
                  "jdbc:postgresql://localhost:5432/demoweb",
                  "postgres",
                  "password");
          break;
        }
      default:
      case Test:
        {
          dataSource =
              new PooledDataSource(
                  "org.postgresql.Driver",
                  "jdbc:postgresql://localhost:5432/demowebtest",
                  "postgres",
                  "password");
          break;
        }
    }

    Environment environment =
        new Environment(envCtx.mode.toString(), new JdbcTransactionFactory(), dataSource);

    Configuration config = new Configuration(environment);
    config.addMapper(DatabaseMapper.class);

    SqlSessionFactoryBuilder builder = new SqlSessionFactoryBuilder();
    fact = builder.build(config);
  }

  public static ConnectionReservoirInterface demoweb() {
    assert (demowebDbConnections != null);
    return demowebDbConnections;
  }

  public static SqlSession openSession() {
    return fact.openSession();
  }

  public static void closeSession(SqlSession sess) {
    sess.close();
  }

  public static void deleteAllData() throws SQLException {
    if (envCtx.mode == EnvironmentContext.Mode.Test) {
      SqlSession sess = openSession();
      DatabaseMapper mapper = sess.getMapper(DatabaseMapper.class);
      List<String> tbNames = mapper.getAllTableNames();
      for (String tbName : tbNames) {
        sess.getConnection().createStatement().executeUpdate("DELETE FROM " + tbName + " CASCADE ");
      }
      sess.commit(true);
    } else {
      throw new AccessControlException("Can do this in non-testing environment.");
    }
  }
}
