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

#ifndef MESSAGECHANNELSERVICEIMPL_H
#define MESSAGECHANNELSERVICEIMPL_H

#include <grpcpp/grpcpp.h>

#include "demoweb_service/demoweb/proto_cc/service_message_channel.grpc.pb.h"

namespace e8 {

/**
 * @brief The MessageChannelServiceImpl class Service for message channel management.
 */
class MessageChannelServiceImpl : public MessageChannelService::Service {
  public:
    MessageChannelServiceImpl() = default;
    ~MessageChannelServiceImpl() override = default;

    grpc::Status CreateMessageChannel(grpc::ServerContext *context,
                                      CreateMessageChannelRequest const *request,
                                      CreateMessageChannelResponse *response) override;
};

} // namespace e8

#endif // MESSAGECHANNELSERVICEIMPL_H
