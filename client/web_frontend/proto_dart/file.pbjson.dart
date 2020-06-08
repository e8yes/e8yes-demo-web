///
//  Generated code. Do not modify.
//  source: file.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const FileFormat$json = const {
  '1': 'FileFormat',
  '2': const [
    const {'1': 'FFMT_UNKNOWN', '2': 0},
    const {'1': 'FFMT_IMAGE_JPEG', '2': 1},
    const {'1': 'FFMT_IMAGE_PNG', '2': 2},
    const {'1': 'FFMT_VIDEO_MPEG4', '2': 3},
    const {'1': 'FFMT_VIDEO_GIF', '2': 4},
    const {'1': 'FFMT_VIDEO_OGV', '2': 5},
    const {'1': 'FFMT_AUDIO_MP3', '2': 6},
    const {'1': 'FFMT_AUDIO_OGG', '2': 7},
  ],
};

const FileAccessMode$json = const {
  '1': 'FileAccessMode',
  '2': const [
    const {'1': 'FAM_READ', '2': 0},
    const {'1': 'FAM_WRITE', '2': 1},
    const {'1': 'FAM_READWRITE', '2': 2},
  ],
};

const FileChunk$json = const {
  '1': 'FileChunk',
  '2': const [
    const {'1': 'chunk_number', '3': 1, '4': 1, '5': 5, '10': 'chunkNumber'},
    const {'1': 'data', '3': 2, '4': 1, '5': 12, '10': 'data'},
  ],
};

const SignableFileAccess$json = const {
  '1': 'SignableFileAccess',
  '2': const [
    const {'1': 'expiry_timestamp', '3': 1, '4': 1, '5': 3, '10': 'expiryTimestamp'},
    const {'1': 'viewer_id', '3': 2, '4': 1, '5': 3, '10': 'viewerId'},
    const {'1': 'file_path', '3': 3, '4': 1, '5': 9, '10': 'filePath'},
    const {'1': 'access_mode', '3': 4, '4': 1, '5': 14, '6': '.e8.FileAccessMode', '10': 'accessMode'},
  ],
};

const FileTokenAccess$json = const {
  '1': 'FileTokenAccess',
  '2': const [
    const {'1': 'access_token', '3': 1, '4': 1, '5': 12, '10': 'accessToken'},
  ],
};

const FileDirectAccess$json = const {
  '1': 'FileDirectAccess',
  '2': const [
    const {'1': 'path', '3': 2, '4': 1, '5': 9, '10': 'path'},
    const {'1': 'access_mode', '3': 3, '4': 1, '5': 14, '6': '.e8.FileAccessMode', '10': 'accessMode'},
  ],
};

const FileDescriptor$json = const {
  '1': 'FileDescriptor',
  '2': const [
    const {'1': 'file_token_access', '3': 1, '4': 1, '5': 11, '6': '.e8.FileTokenAccess', '9': 0, '10': 'fileTokenAccess'},
    const {'1': 'file_direct_access', '3': 2, '4': 1, '5': 11, '6': '.e8.FileDirectAccess', '9': 0, '10': 'fileDirectAccess'},
    const {'1': 'file_name', '3': 3, '4': 1, '5': 9, '10': 'fileName'},
    const {'1': 'file_format', '3': 4, '4': 1, '5': 14, '6': '.e8.FileFormat', '10': 'fileFormat'},
    const {'1': 'encryption_source', '3': 5, '4': 1, '5': 14, '6': '.e8.EncryptionSource', '10': 'encryptionSource'},
    const {'1': 'storage_size', '3': 6, '4': 1, '5': 3, '10': 'storageSize'},
    const {'1': 'created_at_timestamp', '3': 7, '4': 1, '5': 3, '10': 'createdAtTimestamp'},
    const {'1': 'last_modified_at_timestamp', '3': 8, '4': 1, '5': 3, '10': 'lastModifiedAtTimestamp'},
  ],
  '8': const [
    const {'1': 'AccessMethod'},
  ],
};

