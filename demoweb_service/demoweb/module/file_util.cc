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

#include <optional>
#include <string>

#include "demoweb_service/demoweb/module/file_util.h"
#include "proto_cc/file.pb.h"

namespace e8 {

std::optional<std::string> FileFormatSuffix(FileFormat file_format) {
    switch (file_format) {
    case FFMT_UNKNOWN:
        return std::nullopt;
    case FFMT_AUDIO_MP3:
        return "mp3";
    case FFMT_AUDIO_OGG:
        return "ogg";
    case FFMT_IMAGE_PNG:
        return "png";
    case FFMT_VIDEO_GIF:
        return "gif";
    case FFMT_VIDEO_OGV:
        return "ogv";
    case FFMT_IMAGE_JPEG:
        return "jpeg";
    case FFMT_VIDEO_MPEG4:
        return "mp4";
    default:
        return std::nullopt;
    }
}

} // namespace e8
