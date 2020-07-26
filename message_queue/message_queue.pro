TEMPLATE = subdirs
SUBDIRS = \
    message_queue/message_queue_service.pro \
    message_queue/message_queue_service_main.pro \
    publisher/publisher.pro \
    subscriber/subscriber_service.pro \
    subscriber/subscriber_service_main.pro

CONFIG += ordered
