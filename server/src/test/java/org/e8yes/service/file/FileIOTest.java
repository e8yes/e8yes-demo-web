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

import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.exceptions.JWTVerificationException;
import com.google.protobuf.ByteString;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.nio.ByteBuffer;
import java.sql.SQLException;
import java.util.Date;
import org.e8yes.constant.FileStreamConstants;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.exception.ResourceMissingException;
import org.e8yes.fsprovider.FileAccessLocation;
import org.e8yes.fsprovider.FileSystemProviderInterface;
import org.e8yes.service.EncryptionSource;
import org.e8yes.service.FileAccessMode;
import org.e8yes.service.FileDescriptor;
import org.e8yes.service.FileFormat;
import org.e8yes.service.FileTokenAccess;
import org.e8yes.service.identity.Identity;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class FileIOTest {

  @BeforeEach
  public void setUp() throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Test);
  }

  @AfterEach
  public void tearDown() throws Exception {
    Initializer.cleanUp();
  }

  private ByteBuffer generateFileChunk(int start, int len) {
    ByteBuffer buf = ByteBuffer.allocate(len);
    for (int i = start; i < start + len; i++) {
      buf.put((byte) i);
    }
    buf.flip();
    return buf;
  }

  @Test
  public void writeAndReadTest()
      throws JWTVerificationException, AccessDeniedException, IOException, IllegalArgumentException,
          SQLException, IllegalAccessException, NoSuchMethodException, InstantiationException,
          InvocationTargetException, ResourceMissingException {
    FileSystemProviderInterface fsProvider = Initializer.environmentContext().fileSystemProvider();
    JWTVerifier jwtVerifier =
        Initializer.environmentContext().authorizationJwtProvider().jwtverifier();
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    Identity viewer = new Identity(/*userId=*/ 1L, /*groupNames=*/ null);
    FileAccessLocation location = new FileAccessLocation(/*path=*/ "/users/1/drive/test_file.bin");

    FileAccessValidator.FileAccessToken writeOnlyToken =
        FileAccessValidator.signAccessToken(
            viewer,
            location,
            FileAccessMode.FAM_WRITE,
            Initializer.environmentContext().authorizationJwtProvider().algorithm());

    FileDescriptor desc =
        FileDescriptor.newBuilder()
            .setEncryptionSource(EncryptionSource.ESRC_SECURITY_KEY)
            .setFileTokenAccess(
                FileTokenAccess.newBuilder()
                    .setAccessToken(ByteString.copyFrom(writeOnlyToken.jwtToken)))
            .build();

    // Write to file.
    FileIO.FileChunkWriter fileWriter = new FileIO.FileChunkWriter(fsProvider, jwtVerifier, dbConn);

    ByteBuffer chunk0 =
        generateFileChunk(/*start=*/ 12, /*len=*/ FileStreamConstants.CHUNK_SIZE_LIMIT);
    ByteBuffer chunk1 =
        generateFileChunk(/*start=*/ 53, /*len=*/ FileStreamConstants.CHUNK_SIZE_LIMIT);
    ByteBuffer chunk2 = generateFileChunk(/*start=*/ 0, /*len=*/ 100);

    fileWriter.writeNextChunk(viewer, desc, chunk0);
    fileWriter.writeNextChunk(viewer, desc, chunk1);
    fileWriter.writeNextChunk(viewer, desc, chunk2);

    fileWriter.close();

    // Read back from the same file.
    FileAccessValidator.FileAccessToken readOnlyToken =
        FileAccessValidator.signAccessToken(
            viewer,
            location,
            FileAccessMode.FAM_READ,
            Initializer.environmentContext().authorizationJwtProvider().algorithm());
    desc =
        FileDescriptor.newBuilder()
            .setFileTokenAccess(
                FileTokenAccess.newBuilder()
                    .setAccessToken(ByteString.copyFrom(readOnlyToken.jwtToken)))
            .build();

    FileIO.readFile(
        viewer,
        desc,
        fsProvider,
        jwtVerifier,
        dbConn,
        new FileIO.FileChunkListener() {
          @Override
          public void onReady(FileEntity metadata) {
            Assertions.assertEquals("/users/1/drive/test_file.bin", metadata.path.value());
            Assertions.assertEquals(
                (Long) (FileStreamConstants.CHUNK_SIZE_LIMIT * 2L + 100L),
                metadata.storage_size.value());
            Assertions.assertEquals(
                (Integer) EncryptionSource.ESRC_SECURITY_KEY.getNumber(),
                metadata.encryption_key_source.value());
            Assertions.assertEquals(
                (Integer) FileFormat.FFMT_UNKNOWN.getNumber(), metadata.format.value());
            Assertions.assertTrue(
                (new Date().getTime() - metadata.created_at.value().getTime()) < 10 * 1000 * 1000);
            Assertions.assertTrue(
                (new Date().getTime() - metadata.last_modified_at.value().getTime())
                    < 10 * 1000 * 1000);
          }

          @Override
          public void onComplete() {}

          @Override
          public void processNextChunk(ByteBuffer chunkData, int chunkNumber) {
            switch (chunkNumber) {
              case 0:
                Assertions.assertArrayEquals(chunk0.array(), chunkData.array());
                break;
              case 1:
                Assertions.assertArrayEquals(chunk1.array(), chunkData.array());
                break;
              case 2:
                Assertions.assertArrayEquals(chunk2.array(), chunkData.array());
                break;
              default:
                Assertions.assertFalse(true);
                break;
            }
          }
        });
  }
}
