/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <grpcpp/grpcpp.h>

#include "demoweb_service/demoweb/proto_cc/service_file.grpc.pb.h"
#include "demoweb_service/demoweb/proto_cc/service_file.pb.h"
#include "demoweb_service/demoweb/service/file_service.h"

namespace e8 {

grpc::Status FileServiceImpl::Upload(grpc::ServerContext *context,
                                     grpc::ServerReader<UploadFileRequest> *reader,
                                     UploadFileResponse *response) {}

grpc::Status FileServiceImpl::Download(grpc::ServerContext *context,
                                       DownloadFileRequest const *request,
                                       grpc::ServerWriter<DownloadFileResponse> *writer) {}

} // namespace e8
