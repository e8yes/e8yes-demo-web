package org.e8yes.srvs.dao.mappers.handlers;

import java.sql.Array;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.apache.ibatis.type.BaseTypeHandler;
import org.apache.ibatis.type.JdbcType;
import org.apache.ibatis.type.MappedJdbcTypes;

/**
 *
 * @author davis
 */
@MappedJdbcTypes(JdbcType.ARRAY)
public class IntegerListTypeHandler extends BaseTypeHandler<List> {

        @Override
        public void
                setNonNullParameter(PreparedStatement ps,
                                    int col,
                                    List arr,
                                    JdbcType jdbcType) throws SQLException {
                Connection conn = ps.getConnection();
                ps.setArray(col, conn.createArrayOf("int", arr.toArray()));
        }

        @Override
        public List<Integer>
                getNullableResult(ResultSet rs,
                                  String colName) throws SQLException {
                Array arr = rs.getArray(colName);
                if (arr != null) {
                        Integer[] intArr = (Integer[]) arr.getArray();
                        return Arrays.asList(intArr);
                } else {
                        return new ArrayList();
                }
        }

        @Override
        public List<Integer>
                getNullableResult(ResultSet rs,
                                  int col) throws SQLException {
                Array arr = rs.getArray(col);
                if (arr != null) {
                        Integer[] intArr = (Integer[]) arr.getArray();
                        return Arrays.asList(intArr);
                } else {
                        return new ArrayList();
                }
        }

        @Override
        public List<Integer>
                getNullableResult(CallableStatement cs,
                                  int col) throws SQLException {
                Array arr = cs.getArray(col);
                if (arr != null) {
                        Integer[] intArr = (Integer[]) arr.getArray();
                        return Arrays.asList(intArr);
                } else {
                        return new ArrayList();
                }
        }

}
