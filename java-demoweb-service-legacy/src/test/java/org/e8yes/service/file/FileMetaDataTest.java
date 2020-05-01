/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, b ut WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */
package org.e8yes.service.file;

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.Calendar;
import java.util.Date;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.ResourceMissingException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.service.EncryptionSource;
import org.e8yes.service.FileDescriptor;
import org.e8yes.service.FileFormat;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class FileMetaDataTest {

  @BeforeEach
  public void setUp() throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Test);
  }

  @AfterEach
  public void tearDown() throws Exception {
    Initializer.cleanUp();
  }

  @Test
  public void addMetadataTest()
      throws IllegalArgumentException, SQLException, IllegalAccessException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();
    FileAccessLocation location = new FileAccessLocation(/*path=*/ "/users/1/drive/test_file.jpg");

    FileEntity file =
        FileMetaData.addMetaDataForFile(location, EncryptionSource.ESRC_SECURITY_KEY, dbConn);
    Assertions.assertEquals("/users/1/drive/test_file.jpg", file.path.value());
    Assertions.assertEquals((Long) 0L, file.storage_size.value());
    Assertions.assertEquals((Integer) FileFormat.FFMT_IMAGE_JPEG.getNumber(), file.format.value());
    Assertions.assertEquals(
        (Integer) EncryptionSource.ESRC_SECURITY_KEY.getNumber(),
        file.encryption_key_source.value());
    Assertions.assertTrue(
        new Date().getTime() - file.created_at.value().getTime() < 10 * 1000 * 1000);
    Assertions.assertTrue(
        new Date().getTime() - file.last_modified_at.value().getTime() < 10 * 1000 * 1000);
  }

  @Test
  public void updateStorageSizeTest()
      throws IllegalArgumentException, SQLException, IllegalAccessException, NoSuchMethodException,
          InstantiationException, InvocationTargetException, ResourceMissingException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();
    FileAccessLocation location = new FileAccessLocation(/*path=*/ "/users/1/drive/test_file.jpg");
    FileMetaData.addMetaDataForFile(location, EncryptionSource.ESRC_SECURITY_KEY, dbConn);

    FileMetaData.updateFileSize(location, 100L, dbConn);

    FileEntity retrieved = FileMetaData.retrieveFileMetadata(location, dbConn);
    Assertions.assertEquals((Long) 100L, retrieved.storage_size.value());
  }

  @Test
  public void retrieveMetadataTest()
      throws IllegalArgumentException, SQLException, IllegalAccessException, NoSuchMethodException,
          InstantiationException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();
    FileAccessLocation location = new FileAccessLocation(/*path=*/ "/users/1/drive/test_file.jpg");

    FileEntity file =
        FileMetaData.addMetaDataForFile(location, EncryptionSource.ESRC_SECURITY_KEY, dbConn);
    FileEntity retrieved = FileMetaData.retrieveFileMetadata(location, dbConn);
    Assertions.assertEquals(file.path.value(), retrieved.path.value());
    Assertions.assertEquals(file.storage_size.value(), retrieved.storage_size.value());
    Assertions.assertEquals(file.format.value(), retrieved.format.value());
    Assertions.assertEquals(
        file.encryption_key_source.value(), retrieved.encryption_key_source.value());
    Assertions.assertEquals(
        file.created_at.value().getTime(), retrieved.created_at.value().getTime());
    Assertions.assertEquals(
        file.last_modified_at.value().getTime(), retrieved.last_modified_at.value().getTime());
  }

  @Test
  public void createFileDescriptorBuilderTest() {
    FileEntity file = new FileEntity();
    file.path.assign("/users/1/drive/test_file.jpg");
    file.storage_size.assign(100L);
    file.format.assign(FileFormat.FFMT_AUDIO_MP3.getNumber());
    file.encryption_key_source.assign(EncryptionSource.ESRC_SECURITY_KEY.getNumber());

    Calendar today = Calendar.getInstance();
    Calendar yesterday = Calendar.getInstance();
    yesterday.add(Calendar.DAY_OF_YEAR, -1);
    file.created_at.assign(yesterday.getTime());
    file.last_modified_at.assign(today.getTime());

    FileDescriptor.Builder builder = FileMetaData.createFileDescriptorBuilder(file);
    FileDescriptor desc = builder.build();
    Assertions.assertEquals("test_file.jpg", desc.getFileName());
    Assertions.assertEquals(FileFormat.FFMT_AUDIO_MP3, desc.getFileFormat());
    Assertions.assertEquals(100L, desc.getStorageSize());
    Assertions.assertEquals(yesterday.getTime().getTime(), desc.getCreatedAtTimestamp());
    Assertions.assertEquals(today.getTime().getTime(), desc.getLastModifiedAtTimestamp());
  }
}
