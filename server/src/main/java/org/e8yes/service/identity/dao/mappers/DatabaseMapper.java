package org.e8yes.service.identity.dao.mappers;

import java.util.List;
import org.apache.ibatis.annotations.Select;

/**
 * Database reflection.
 *
 * @author davis
 */
public interface DatabaseMapper {

  @Select(
      "SELECT "
          + "     tb.table_name "
          + "FROM "
          + "     information_schema.tables tb "
          + "WHERE "
          + "     tb.table_schema='public' ")
  public List<String> getAllTableNames();
}
