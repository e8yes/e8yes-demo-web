TEMPLATE = subdirs
SUBDIRS = \
    common/message_queue_common.pro \
    message_queue/message_queue_service.pro \
    publisher \
    subscriber/subscriber_service.pro

CONFIG += ordered
