package org.e8yes.environment;

import java.security.AccessControlException;
import java.sql.SQLException;
import java.util.Set;
import org.e8yes.sql.SqlQueryBuilder;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.BasicConnectionReservoir;
import org.e8yes.sql.connection.ConnectionFactory;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** */
public class DatabaseConnection {

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
  }

  public static ConnectionReservoirInterface demoweb() {
    assert (demowebDbConnections != null);
    return demowebDbConnections;
  }

  public static void deleteAllData() throws SQLException {
    if (envCtx.mode == EnvironmentContext.Mode.Test) {
      SqlRunner runner = new SqlRunner();
      Set<String> tableNames =
          runner.withConnectionReservoir(DatabaseConnection.demoweb()).tables();
      SqlQueryBuilder constraint = new SqlQueryBuilder().queryPiece("CASCADE");
      for (String tbName : tableNames) {
        runner.runDelete(tbName, constraint);
      }
    } else {
      throw new AccessControlException("Can do this in non-testing environment.");
    }
  }
}
