///
//  Generated code. Do not modify.
//  source: service_file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

import 'file.pbjson.dart' as $1;

const UploadFileRequest$json = const {
  '1': 'UploadFileRequest',
  '2': const [
    const {'1': 'file_descriptor', '3': 1, '4': 1, '5': 11, '6': '.e8.FileDescriptor', '10': 'fileDescriptor'},
    const {'1': 'current_chunk', '3': 2, '4': 1, '5': 11, '6': '.e8.FileChunk', '10': 'currentChunk'},
  ],
};

const UploadFileResponse$json = const {
  '1': 'UploadFileResponse',
};

const DownloadFileRequest$json = const {
  '1': 'DownloadFileRequest',
  '2': const [
    const {'1': 'file_descriptor', '3': 1, '4': 1, '5': 11, '6': '.e8.FileDescriptor', '10': 'fileDescriptor'},
  ],
};

const DownloadFileResponse$json = const {
  '1': 'DownloadFileResponse',
  '2': const [
    const {'1': 'file_descriptor', '3': 1, '4': 1, '5': 11, '6': '.e8.FileDescriptor', '10': 'fileDescriptor'},
    const {'1': 'current_chunk', '3': 2, '4': 1, '5': 11, '6': '.e8.FileChunk', '10': 'currentChunk'},
  ],
};

const FileServiceBase$json = const {
  '1': 'FileService',
  '2': const [
    const {'1': 'Upload', '2': '.e8.UploadFileRequest', '3': '.e8.UploadFileResponse', '5': true},
    const {'1': 'Download', '2': '.e8.DownloadFileRequest', '3': '.e8.DownloadFileResponse', '6': true},
  ],
};

const FileServiceBase$messageJson = const {
  '.e8.UploadFileRequest': UploadFileRequest$json,
  '.e8.FileDescriptor': $1.FileDescriptor$json,
  '.e8.FileTokenAccess': $1.FileTokenAccess$json,
  '.e8.FileDirectAccess': $1.FileDirectAccess$json,
  '.e8.FileChunk': $1.FileChunk$json,
  '.e8.UploadFileResponse': UploadFileResponse$json,
  '.e8.DownloadFileRequest': DownloadFileRequest$json,
  '.e8.DownloadFileResponse': DownloadFileResponse$json,
};

