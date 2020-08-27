///
//  Generated code. Do not modify.
//  source: node.proto
//
// @dart = 2.3
// ignore_for_file: camel_case_types,non_constant_identifier_names,library_prefixes,unused_import,unused_shown_name,return_of_invalid_type

const NodeFunction$json = const {
  '1': 'NodeFunction',
  '2': const [
    const {'1': 'NDF_INVALID', '2': 0},
    const {'1': 'NDF_DISTRIBUTOR', '2': 1},
    const {'1': 'NDF_FILE_STORE', '2': 2},
    const {'1': 'NDF_MESSAGE_QUEUE', '2': 3},
    const {'1': 'NDF_TASK_EXECUTOR', '2': 4},
    const {'1': 'NDF_RELATIONAL_STORE', '2': 5},
    const {'1': 'NDF_LOAD_BALANCER', '2': 6},
    const {'1': 'NDF_IMAGE_BUILDER', '2': 7},
  ],
};

const NodeStatus$json = const {
  '1': 'NodeStatus',
  '2': const [
    const {'1': 'NDS_INVALID', '2': 0},
    const {'1': 'NDS_INITIALIZING', '2': 1},
    const {'1': 'NDS_READY', '2': 2},
    const {'1': 'NDS_UNAVALIABLE', '2': 3},
  ],
};

const NodeState$json = const {
  '1': 'NodeState',
  '2': const [
    const {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'ip_address', '3': 2, '4': 1, '5': 12, '10': 'ipAddress'},
    const {'1': 'status', '3': 3, '4': 1, '5': 14, '6': '.e8.NodeStatus', '10': 'status'},
    const {'1': 'functions', '3': 4, '4': 3, '5': 14, '6': '.e8.NodeFunction', '10': 'functions'},
    const {'1': 'function_ports', '3': 5, '4': 3, '5': 5, '10': 'functionPorts'},
  ],
};

const NodeStateRevision$json = const {
  '1': 'NodeStateRevision',
  '2': const [
    const {'1': 'revision_epoch', '3': 1, '4': 1, '5': 3, '10': 'revisionEpoch'},
    const {'1': 'nodes', '3': 3, '4': 3, '5': 11, '6': '.e8.NodeStateRevision.NodesEntry', '10': 'nodes'},
    const {'1': 'delta_operations', '3': 4, '4': 3, '5': 11, '6': '.e8.NodeStateRevision.DeltaOperationsEntry', '10': 'deltaOperations'},
  ],
  '3': const [NodeStateRevision_NodesEntry$json, NodeStateRevision_DeltaOperationsEntry$json],
};

const NodeStateRevision_NodesEntry$json = const {
  '1': 'NodesEntry',
  '2': const [
    const {'1': 'key', '3': 1, '4': 1, '5': 9, '10': 'key'},
    const {'1': 'value', '3': 2, '4': 1, '5': 11, '6': '.e8.NodeState', '10': 'value'},
  ],
  '7': const {'7': true},
};

const NodeStateRevision_DeltaOperationsEntry$json = const {
  '1': 'DeltaOperationsEntry',
  '2': const [
    const {'1': 'key', '3': 1, '4': 1, '5': 9, '10': 'key'},
    const {'1': 'value', '3': 2, '4': 1, '5': 14, '6': '.e8.DeltaOperation', '10': 'value'},
  ],
  '7': const {'7': true},
};

